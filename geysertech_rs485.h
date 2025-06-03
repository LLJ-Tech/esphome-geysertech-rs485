#pragma once
#include "esphome.h"

class GeyserTechRS485 : public PollingComponent {
 public:
  Sensor *geyser_temp_sensor = nullptr;
  Sensor *pv_voltage_sensor = nullptr;
  TextSensor *geyser_status_sensor = nullptr;

  GeyserTechRS485() : PollingComponent(1000) {}

  void setup() override;
  void update() override;

  void set_geyser_temp_sensor(Sensor *sensor) { geyser_temp_sensor = sensor; }
  void set_pv_voltage_sensor(Sensor *sensor) { pv_voltage_sensor = sensor; }
  void set_geyser_status_sensor(TextSensor *sensor) { geyser_status_sensor = sensor; }
};
