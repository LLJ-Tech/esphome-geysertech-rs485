#pragma once
#include "esphome.h"

class GeyserTechRS485 : public PollingComponent, public UARTDevice {
 public:
  GeyserTechRS485(UARTComponent *parent) : PollingComponent(1000), UARTDevice(parent) {}

  Sensor *geyser_temp_sensor = nullptr;
  Sensor *pv_voltage_sensor = nullptr;
  Sensor *pv_current_sensor = nullptr;
  Sensor *grid_voltage_sensor = nullptr;
  Sensor *pv_power_sensor = nullptr;
  Sensor *solar_power_kwh_sensor = nullptr;
  TextSensor *geyser_status_sensor = nullptr;

  void setup() override;
  void update() override;

  void set_geyser_temp_sensor(Sensor *sensor) { geyser_temp_sensor = sensor; }
  void set_pv_voltage_sensor(Sensor *sensor) { pv_voltage_sensor = sensor; }
  void set_pv_current_sensor(Sensor *sensor) { pv_current_sensor = sensor; }
  void set_grid_voltage_sensor(Sensor *sensor) { grid_voltage_sensor = sensor; }
  void set_pv_power_sensor(Sensor *sensor) { pv_power_sensor = sensor; }
  void set_solar_power_kwh_sensor(Sensor *sensor) { solar_power_kwh_sensor = sensor; }
  void set_geyser_status_sensor(TextSensor *sensor) { geyser_status_sensor = sensor; }
};
