#ifndef _DEVICEIO_H_
#define _DEVICEIO_H_
#include "tydef.h"


//595驱动 --驱动LED与继电器

#define IP595_DS       LATBbits.LATB6
#define IP595_DS_DIR   TRISBbits.TRISB6
#define IP595_SHCP     LATBbits.LATB8
#define IP595_SHCP_DIR TRISBbits.TRISB8
#define IP595_STCP     LATBbits.LATB7
#define IP595_STCP_DIR TRISBbits.TRISB7




#define TURN_ON 0xAA
#define TURN_OFF 0x55

#define RELAY2          0x01
#define RELAY1          0x02
#define LEDA            0x04
#define LEDB            0x08
#define LEDC            0x10
#define LEDD            0x20
#define LEDE            0x40
#define LEDF            0x80

#define OFF_RELAY2          0xFE
#define OFF_RELAY1          0xFD
#define OFF_LEDA            0xFB
#define OFF_LEDB            0xF7
#define OFF_LEDC            0xEF
#define OFF_LEDD            0xDF
#define OFF_LEDE            0xBF
#define OFF_LEDF            0x7F


//储能继电器
#define ON_CHUNENG_RELAY   RELAY2 
//故障继电器
#define ON_ERROR_RELAY    RELAY1  
//运行指示
#define ON_RUN_LED     LEDA
//合位指示
#define ON_HE_LED      LEDB
//分位指示
#define ON_FEN_LED     LEDC
//电压指示
#define ON_DIANYA_LED  LEDD
//故障指示
#define ON_ERROR_LED   LEDE

//储能继电器
#define OFF_CHUNENG_RELAY   OFF_RELAY2 
//故障继电器
#define OFF_ERROR_RELAY    OFF_RELAY1  
//运行指示
#define OFF_RUN_LED     OFF_LEDA
//合位指示
#define OFF_HE_LED      OFF_LEDB
//分位指示
#define OFF_FEN_LED     OFF_LEDC
//电压指示
#define OFF_DIANYA_LED  OFF_LEDD
//故障指示
#define OFF_ERROR_LED   OFF_LEDE


//电流输出同步
#define OC1_T   LATBbits.LATB4
#define OC1_T_DIR TRISBbits.TRISB4
#define OC2_T   LATBbits.LATB5
#define OC2_T_DIR TRISBbits.TRISB5
#define TONGBU_CURRENT_A OC1_T
#define TONGBU_CURRENT_B OC2_T

//光纤/光耦同步 输入
#define SYNA_IN   PORTDbits.RD1
#define SYNA_IN_DIR TRISDbits.TRISD1
#define SYNB_IN   PORTDbits.RD0
#define SYNB_IN_DIR TRISDbits.TRISD0


//IGBT 控制---A
#define ENABLE 1
#define DISABLE 0
//合闸桥臂--双IGBT
#define DRIVER_A   LATDbits.LATD3
#define DRIVER_A_DIR TRISDbits.TRISD3
#define DRIVER_D   LATEbits.LATE8
#define DRIVER_D_DIR  TRISEbits.TRISE8

//分闸桥臂--单IGBT
#define DRIVER_B   LATCbits.LATC13
#define DRIVER_B_DIR TRISCbits.TRISC13
#define DRIVER_C   LATCbits.LATC14
#define DRIVER_C_DIR TRISCbits.TRISC14


#define HEZHA_A() {DRIVER_B  = DISABLE; DRIVER_C = ENABLE;DRIVER_A = ENABLE;DRIVER_D = DISABLE; TONGBU_CURRENT_A = 1;}
#define FENZHA_A() {DRIVER_A = DISABLE; DRIVER_D  = ENABLE; DRIVER_B = ENABLE; DRIVER_C = DISABLE; TONGBU_CURRENT_A = 1;}
#define RESET_CURRENT_A() {DRIVER_A = DISABLE;DRIVER_D = ENABLE; DRIVER_B = DISABLE; DRIVER_C = ENABLE; TONGBU_CURRENT_A = 0;}







//合分位状态检测
//按钮输入标志位
#define HE_STATE 0x3E
#define FEN_STATE 0x3D
#define HE_WEI_STATE 0x3B
#define FEN_WEI_STATE 0x37
#define HE_YUAN_STATE 0x2F
#define FEN_YUAN_STATE 0x1F

//DINA0
#define HE_INPUT   PORTBbits.RB1  
#define HE_INPUT_DIR TRISBbits.TRISB1
//DINA1
#define FEN_INPUT   PORTEbits.RE0
#define FEN_INPUT_DIR TRISEbits.TRISE0
//DINA2
#define HE_WEI_INPUT   PORTEbits.RE1
#define HE_WEI_INPUT_DIR TRISEbits.TRISE1
//DINA3
#define FEN_WEI_INPUT   PORTEbits.RE2
#define FEN_WEI_INPUT_DIR TRISEbits.TRISE2
//DINA4
#define YUAN_BEN_INPUT   PORTEbits.RE3
#define YUAN_BEN_INPUT_DIR TRISEbits.TRISE3
//DINA5
#define LOCK_INPUT   PORTEbits.RE4
#define LOCK_INPUT_DIR TRISEbits.TRISE4
//DINA6
#define HE_YUAN_INPUT   PORTBbits.RB3
#define HE_YUAN_INPUT_DIR TRISBbits.TRISB3
//DINA7
#define FEN_YUAN_INPUT  PORTBbits.RB2
#define FEN_YUAN_INPUT_DIR TRISBbits.TRISB2


#define     ACTIVE_STATE  1 //激活



void InitDeviceIO(void);
//uint8 GetKeyValue(void);

void Delay20ms(void);

void TestKeys();

void InitInt0();
void TurnOnInt0();
void TurnOffInt0();

void IP595_Out(uint8 state);
void  UpdateIndicateState(uint8 port, uint8 state);
#endif