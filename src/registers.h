/*
    -- Environment --
    0x01    POWER_STATE
    0x02    I2C_ADDR
    0x03    FAN_SPEED
    0x04    PWM_FREQUENCY
    0x05

    -- Version --
    0x10    VERSION_MAJOR
    0x11    VERSION_MINOR
    0x12    VERSION_REVISION
    0x13    IDENTIFICATION_CODE

    -- Status --
    0x20    DC_VOLTAGE
    0x21    INVERTER_TEMPERATURE
    0x22    ERROR_FLAG
    0x23    ERROR_NUMBER

    -- Motor A Configuration --
    0x30    CTRL_TYPE
    0x31    MOTION_MVT
    0x32    ENCODER_RESOLUTION
    0x33    POLES_NUMBERS

    0x34    TORQUE_KP
    0x35    TORQUE_GI
    0x36    CURRENT_MAX

    0x37    SPEED_KP
    0x38    SPEED_GI
    0x39    SPEED_DZ
    0x3A    VOLTAGE_MAX

    0x3B    POSITION_KP
    0x3C    POSITION_DZ

    0x3D    GEAR_NUM
    0x3E    GEAR_DEN

    0x3F    HALL_OFFSET
    0x40    ENABLE_DESAT

    -- Motor B Configuration --
    0x50    CTRL_TYPE
    0x51    MOTION_MVT
    0x52    ENCODER_RESOLUTION
    0x53    POLES_NUMBERS

    0x54    TORQUE_KP
    0x55    TORQUE_GI
    0x56    CURRENT_MAX

    0x57    SPEED_KP
    0x58    SPEED_GI
    0x59    SPEED_DZ
    0x5A    VOLTAGE_MAX

    0x5B    POSITION_KP
    0x5C    POSITION_DZ

    0x5D    GEAR_NUM
    0x5E    GEAR_DEN

    0x5F    HALL_OFFSET
    0x60    ENABLE_DESAT

    -- Motor A Values --
    0x70    PWM
    0x71    TORQUE
    0x72    VELOCITY
    0x73    POSITION

    -- Motor B Values --
    0x80    PWM
    0x81    TORQUE
    0x82    VELOCITY
    0x83    POSITION

    -- MOVEMENT MODULE A --
    0x90    ACCELERATION
    0x91    DECELERATION
    0x92    VELOCITY
    0x93    POSITION

    0x94    START
    0x95    BUSY

    -- MOVEMENT MODULE A --
    0x90    ACCELERATION
    0x91    DECELERATION
    0x92    VELOCITY
    0x93    POSITION

    0x94    START
    0x95    BUSY

    0x96    PROFILE_TYPE

    -- MOVEMENT MODULE B --
    0xA0    ACCELERATION
    0xA1    DECELERATION
    0xA2    VELOCITY
    0xA3    POSITION

    0xA4    START
    0xA5    BUSY

    0xA6    PROFILE_TYPE

    -- ODOMETRY MODULE --
    0xB0    WHEEL_DIAMETER
    0xB1    AXIS_DISTANCE

    0xB2    X
    0xB3    Y
    0xB4    ANGLE
*/
