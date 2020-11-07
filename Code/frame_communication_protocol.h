#ifndef __FRAME_COMMUNICATION_PROTOCOL__
#define __FRAME_COMMUNICATION_PROTOCOL__

// Include standard header files
#include<stdio.h>
#include<stdint.h>

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

#define FRAME_BIT_POS                 0
#define FRAME_BIT_MASK                0x1F

// Create frame header
#define FRAME_BIT_MASK                0xFF
#define FRAME_HEADER(M, F)            (((M&MOTOR_BIT_MASK)<<MOTOR_BIT_MASK | \\
                                       (F&FRAME_BIT_MASK)<<FRAME_BIT_MASK) & \\
                                        FRAME_BIT_MASK)

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
// 0x1 - 1 byte, 0x4 - 4 bytes
#define REGI_ID_TARGET_MOTOR          0x0
#define REGI_ID_SIZE_TARGET_MOTOR     uint8_t

#define REGI_ID_FAULT_FLAGS           0x1
#define REGI_ID_SIZE_FAULT_FLAGS      uint32_t

#define REGI_ID_STATUS                0x2
#define REGI_ID_SIZE_STATUS           0x1

#define REGI_ID_CONTROL_MODE          0x3
#define REGI_ID_SIZE_CONTROL_MODE     0x1

#define REGI_ID_SPEED_REF             0x4
#define REGI_ID_SIZE_CONTROL_MODE     0x1

#endif //__FRAME_COMMUNICATION_PROTOCOL__