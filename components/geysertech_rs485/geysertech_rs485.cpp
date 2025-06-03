#include "geysertech_rs485.h"

void GeyserTechRS485::setup() {
  ESP_LOGD("GeyserTech", "RS485 component initialized");
}

void GeyserTechRS485::update() {
  while (id(uart_rs485).available()) {
    uint8_t byte = id(uart_rs485).read();
    static uint8_t buffer[128];
    static int buffer_index = 0;
    static enum State { WAIT_FOR_0F, WAIT_FOR_F0, WAIT_FOR_31, COLLECT_PAYLOAD } state = WAIT_FOR_0F;
    static unsigned long last_byte_time = 0;
    const unsigned long TIMEOUT_MS = 1000;

    last_byte_time = millis();

    switch (state) {
      case WAIT_FOR_0F:
        if (byte == 0x0F) {
          buffer_index = 0;
          buffer[buffer_index++] = byte;
          state = WAIT_FOR_F0;
        }
        break;

      case WAIT_FOR_F0:
        if (byte == 0xF0) {
          buffer[buffer_index++] = byte;
          state = WAIT_FOR_31;
        } else {
          state = WAIT_FOR_0F;
        }
        break;

      case WAIT_FOR_31:
        if (byte == 0x31) {
          buffer[buffer_index++] = byte;
          state = COLLECT_PAYLOAD;
        } else {
          state = WAIT_FOR_0F;
        }
        break;

      case COLLECT_PAYLOAD:
        buffer[buffer_index++] = byte;
        if (buffer_index >= 52) {
          if (buffer[51] != 0x42) {
            ESP_LOGW("GeyserTech", "Invalid payload, last byte: 0x%02X", buffer[51]);
            buffer_index = 0;
            state = WAIT_FOR_0F;
            break;
          }

          uint8_t geyser_status = buffer[10];
          if (geyser_status_sensor) {
            switch (geyser_status) {
              case 0x0: geyser_status_sensor->publish_state("Off"); break;
              case 0x1: geyser_status_sensor->publish_state("Solar Heating"); break;
              case 0x2: geyser_status_sensor->publish_state("Grid Heating"); break;
              default: geyser_status_sensor->publish_state("Unknown"); break;
            }
          }

          uint32_t temp_raw = (buffer[27] << 24) | (buffer[26] << 16) | (buffer[25] << 8) | buffer[24];
          float geyser_temp = (*(float*)&temp_raw) / 1.0f;
          if (geyser_temp_sensor) geyser_temp_sensor->publish_state(geyser_temp);

          uint32_t pv_volt_raw = (buffer[31] << 24) | (buffer[30] << 16) | (buffer[29] << 8) | buffer[28];
          float pv_voltage = (*(float*)&pv_volt_raw) / 1.0f;
          if (pv_voltage_sensor) pv_voltage_sensor->publish_state(pv_voltage);

          uint32_t pv_curr_raw = (buffer[39] << 24) | (buffer[38] << 16) | (buffer[37] << 8) | buffer[36];
          float pv_current = (*(float*)&pv_curr_raw) / 395.0f;
          if (pv_current_sensor) pv_current_sensor->publish_state(pv_current);

          uint32_t grid_volt_raw = (buffer[43] << 24) | (buffer[42] << 16) | (buffer[41] << 8) | buffer[40];
          float grid_voltage = (*(float*)&grid_volt_raw) / 1.42f;
          if (grid_voltage_sensor) grid_voltage_sensor->publish_state(grid_voltage);

          float pv_power = pv_voltage * pv_current;
          if (pv_power_sensor) pv_power_sensor->publish_state(pv_power);

          uint32_t solar_power_raw = (buffer[23] << 24) | (buffer[22] << 16) | (buffer[21] << 8) | buffer[20];
          float solar_power_kwh = (*(float*)&solar_power_raw) / 1000.0f;
          if (solar_power_kwh_sensor) solar_power_kwh_sensor->publish_state(solar_power_kwh);

          ESP_LOGD("GeyserTech", "Parsed: Temp=%.1f°C, PV_V=%.1fV, PV_I=%.1fA, Grid_V=%.1fV, PV_P=%.1fW, Solar_kWh=%.3f",
                   geyser_temp, pv_voltage, pv_current, grid_voltage, pv_power, solar_power_kwh);

          buffer_index = 0;
          state = WAIT_FOR_0F;
        }
        break;
    }

    if (buffer_index > 0 && millis() - last_byte_time > TIMEOUT_MS) {
      ESP_LOGW("GeyserTech", "Timeout, incomplete message (%d bytes)", buffer_index);
      buffer_index = 0;
      state = WAIT_FOR_0F;
    }
  }
}

