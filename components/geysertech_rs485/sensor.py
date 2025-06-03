# Start with Tags 
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor, uart
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_VOLT, UNIT_AMPERE, UNIT_WATT, UNIT_KILO_WATT_HOURS, ICON_THERMOMETER, ICON_POWER

DEPENDENCIES = ['uart']
geysertech_rs485_ns = cg.esphome_ns.namespace('geysertech_rs485')
GeyserTechRS485 = geysertech_rs485_ns.class_('GeyserTechRS485', cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(GeyserTechRS485),
    cv.GenerateID(uart.CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Optional('geyser_temp'): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1
    ),
    cv.Optional('pv_voltage'): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1
    ),
    cv.Optional('pv_current'): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1
    ),
    cv.Optional('grid_voltage'): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1
    ),
    cv.Optional('pv_power'): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional('solar_power_kwh'): sensor.sensor_schema(
        unit_of_measurement=UNIT_KILO_WATT_HOURS,
        accuracy_decimals=3
    ),
    cv.Optional('geyser_status'): text_sensor.text_sensor_schema(),
}).extend(cv.polling_component_schema('1s')).extend(uart.UART_COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if 'geyser_temp' in config:
        sens = await sensor.new_sensor(config['geyser_temp'])
        cg.add(var.set_geyser_temp_sensor(sens))
    if 'pv_voltage' in config:
        sens = await sensor.new_sensor(config['pv_voltage'])
        cg.add(var.set_pv_voltage_sensor(sens))
    if 'pv_current' in config:
        sens = await sensor.new_sensor(config['pv_current'])
        cg.add(var.set_pv_current_sensor(sens))
    if 'grid_voltage' in config:
        sens = await sensor.new_sensor(config['grid_voltage'])
        cg.add(var.set_grid_voltage_sensor(sens))
    if 'pv_power' in config:
        sens = await sensor.new_sensor(config['pv_power'])
        cg.add(var.set_pv_power_sensor(sens))
    if 'solar_power_kwh' in config:
        sens = await sensor.new_sensor(config['solar_power_kwh'])
        cg.add(var.set_solar_power_kwh_sensor(sens))
    if 'geyser_status' in config:
        sens = await text_sensor.new_text_sensor(config['geyser_status'])
        cg.add(var.set_geyser_status_sensor(sens))
