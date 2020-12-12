#ifndef __FRAME_COMMUNICATION_PROTOCOL_H__
#define __FRAME_COMMUNICATION_PROTOCOL_H__

#ifdef __cplusplus  
extern "C" { 
#endif
// Include standard header files


// Define the MACROS used for frame communication
// By default, motor number is zero.  The maximum number is 7
#define MOTOR_NUMBER                 0
#define MOTOR_BIT_POS                5
#define MOTOR_BIT_MASK               0x7

// Master sends these codes
#define FRAME_CODE_SET_REG            0x01
#define FRAME_CODE_GET_REG            0x02
#define FRAME_CODE_EXECUTE_CMD        0x03
#define FRAME_CODE_GET_BOARD_INFO     0x06
#define FRAME_CODE_SET_RAMP           0x07
#define FRAME_CODE_GET_REVUP_DATA     0x08
#define FRAME_CODE_SET_REVUP_DATA     0x09
#define FRAME_CODE_SET_CURRENT_REF    0x0A
#define FRAME_CODE_GET_FW_VERSION     0x0C
#define FRAME_CODE_GET_DATA_ELEMENT   0x0D
#define FRAME_CODE_CONFIG_DYN_STRUCT  0x0E

// Slave sends these codes
#define FRAME_CODE_ACK                0xF0
#define FRAME_CODE_NACK               0xFF

#define FRAME_CODE_POS                 0
#define FRAME_CODE_MASK                0x1F

// Create frame header
#define FRAME_HEADER_MASK             0xFF
#define FRAME_HEADER(M, F)            (((M&MOTOR_BIT_MASK)<<MOTOR_BIT_MASK | \
                                       (F&FRAME_CODE_MASK)<<FRAME_CODE_POS) & \
                                        FRAME_HEADER_MASK)

// Define error codes
#define ERROR_CODE_NONE               0x00
#define ERROR_CODE_BAD_FRAME_ID       0x01
#define ERROR_CODE_SET_READ_ONLY      0x02
#define ERROR_CODE_GET_WRITE_ONLY     0x03
#define ERROR_CODE_WRONG_SET          0x05
#define ERROR_CODE_WRONG_CMD          0x07
#define ERROR_CODE_OVERRUN            0x08
#define ERROR_CODE_TIMEOUT            0x09
#define ERROR_CODE_BAD_CRC            0x0A
#define ERROR_CODE_BAD_MOTOR_SELECTED 0x0B
#define ERROR_CODE_MP_NOT_ENABLED     0x0C


// Define register ids and their corresponding size
#define REG_ID_TARGET_MOTOR          0x0
#define REG_TYPE_TARGET_MOTOR        uint8_t

#define REG_ID_FAULT_FLAGS           0x1
#define REG_TYPE_FAULT_FLAGS         uint32_t

#define REG_ID_STATUS                0x2
#define REG_TYPE_STATUS              uint8_t

#define REG_ID_CONTROL_MODE          0x3
#define REG_TYPE_CONTROL_MODE        uint8_t

// Speed loop parameters
#define REG_ID_SPEED_REF             0x4
#define REG_TYPE_SPEED_REF           int32_t

#define REG_ID_SPEED_KP              0x5
#define REG_TYPE_SPEED_KP            int16_t

#define REG_ID_SPEED_KI              0x6
#define REG_TYPE_SPEED_KI            int16_t

#define REG_ID_SPEED_KD              0x7
#define REG_TYPE_SPEED_KD            int16_t

#define REG_ID_RAMP_FINAL_SPEED      0x5B
#define REG_TYPE_RAMP_FINAL_SPEED    int32_t

#define REG_ID_SPEED_KP_DIV          0x6E
#define REG_TYPE_SPEED_KP_DIV        int16_t

#define REG_ID_SPEED_KI_DIV          0x6F
#define REG_TYPE_SPEED_KI_DIV        int16_t

// FOC loop parameters
// Iq - quadrature current
#define REG_ID_TORQUE_REF            0x8
#define REG_TYPE_TORQUE_REF          int16_t

#define REG_ID_TORQUE_KP             0x9
#define REG_TYPE_TORQUE_KP           int16_t

#define REG_ID_TORQUE_KI             0xA
#define REG_TYPE_TORQUE_KI           int16_t

#define REG_ID_TORQUE_KD             0xB
#define REG_TYPE_TORQUE_KD           int16_t


// Id - direct current
#define REG_ID_FLUX_REF              0xC
#define REG_TYPE_FLUX_REF            int16_t

#define REG_ID_FLUX_KP               0xD
#define REG_TYPE_FLUX_KP             int16_t

#define REG_ID_FLUX_KI               0xE
#define REG_TYPE_FLUX_KI             int16_t

#define REG_ID_FLUX_KD               0xF
#define REG_TYPE_FLUX_KD             int16_t

// Other parameters of motor control library
// that are measured on-board
#define REG_ID_BUS_VOLTAGE           0x19
#define REG_TYPE_BUS_VOLTAGE         uint16_t

#define REG_ID_HEATS_TEMP            0x1A
#define REG_TYPE_HEATS_TEMP          uint16_t

#define REG_ID_MOTOR_POWER           0x1B
#define REG_TYPE_MOTOR_POWER         uint16_t

#define REG_ID_SPEED_MEAS            0x1E
#define REG_TYPE_SPEED_MEAS          int32_t

#define REG_ID_TORQUE_MEAS           0x1F
#define REG_TYPE_TORQUE_MEAS         int16_t

#define REG_ID_FLUX_MEAS             0x20
#define REG_TYPE_FLUX_MEAS           int16_t

#define REG_ID_IA_MEAS               0x23
#define REG_TYPE_IA_MEAS             int16_t

#define REG_ID_IB_MEAS               0x24
#define REG_TYPE_IB_MEAS             int16_t

#define REG_ID_IALPHA_MEAS           0x25
#define REG_TYPE_IALPHA_MEAS         int16_t

#define REG_ID_IBETA_MEAS            0x26
#define REG_TYPE_IBETA_MEAS          int16_t

#define REG_ID_IQ_MEAS               0x27
#define REG_TYPE_IQ_MEAS             int16_t

#define REG_ID_ID_MEAS               0x28
#define REG_TYPE_ID_MEAS             int16_t

#define REG_ID_VQ_MEAS               0x2B
#define REG_TYPE_VQ_MEAS             int16_t

#define REG_ID_VD_MEAS               0x2C
#define REG_TYPE_VD_MEAS             int16_t

#define REG_ID_VALPHA_MEAS           0x2D
#define REG_TYPE_VALPHA_MEAS         int16_t

#define REG_ID_VBETA_MEAS            0x2E
#define REG_TYPE_VBETA_MEAS          int16_t

#define REG_ID_MEAS_EL_ANGLE         0x2F
#define REG_TYPE_MEAS_EL_ANGL        int16_t

#define REG_ID_MEAS_ROT_SPEED        0x30
#define REG_TYPE_MEAS_ROT_SPEED      int16_t

#define REG_ID_MAX_APP_SPEED         0x3F
#define REG_TYPE_MAX_APP_SPEED       uint32_t

#define REG_ID_MIN_APP_SPEED         0x40
#define REG_TYPE_MIN_APP_SPEED       uint32_t

#define REG_ID_IQ_SPEED_MODE         0x41
#define REG_TYPE_IQ_SPEED_MODE       int16_t

// Identification numbers of control and power board
#define REG_ID_CTRBDID               0x72
#define REG_TYPE_CTRBDID             uint32_t

#define REG_ID_PWBDID                0x73
#define REG_TYPE_PWBDID              uint32_t

// Define motor commands
#define MOTOR_CMD_START_MOTOR        0x01
#define MOTOR_CMD_STOP_MOTOR         0x02
#define MOTOR_CMD_STOP_RAMP          0x03
#define MOTOR_CMD_START_STOP         0x06
#define MOTOR_CMD_FAULT_ACK          0x07
#define MOTOR_CMD_ENCODER_ALIGN      0x08
#define MOTOR_CMD_IQDREF_CLEAR       0x09





// Create function prototypes

// uint8_t set_reg(uint8_t ui8Register, uint32_t ui)

#ifdef __cplusplus 
} 
#endif 

#endif //__FRAME_COMMUNICATION_PROTOCOL_H__