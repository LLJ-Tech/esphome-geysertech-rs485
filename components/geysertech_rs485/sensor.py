import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor
from esphome.const import CONF_ID, UNIT_CELSIUS, UNIT_VOLT, ICON_THERMOMETER

DEPENDENCIES = ['uart']
geysertech_rs485_ns = cg.esphome_ns.namespace('geysertech_rs485')
GeyserTechRS485 = geysertech_rs485_ns.class_('GeyserTechRS485', cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(GeyserTechRS485),
    cv.Optional('geyser_temp'): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1
    ),
    cv.Optional('pv_voltage'): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1
    ),
    cv.Optional('geyser_status'): text_sensor.text_sensor_schema(),
}).extend(cv.polling_component_schema('1s')).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if 'geyser_temp' in config:
        sens = await sensor.new_sensor(config['geyser_temp'])
        cg.add(var.set_geyser_temp_sensor(sens))
    if 'pv_voltage' in config:
        sens = await sensor.new_sensor(config['pv_voltage'])
        cg.add(var.set_pv_voltage_sensor(sens))
    if 'geyser_status' in config:
        sens = await text_sensor.new_text_sensor(config['geyser_status'])
        cg.add(var.set_geyser_status_sensor(sens))
