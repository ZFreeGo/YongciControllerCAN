/*
 * File:   Action.h
 * Author: ZFreeGo
 *
 * Created on 2014年9月22日, 下午3:35
 */

#ifndef ACTION_H
#define	ACTION_H

#include "RtuFrame.h"
#include "tydef.h"

#define  RESET_MCU   0x01

#define      LED1_TOGLE  0x11
#define      LED2_TOGLE  0x12
#define      LED3_TOGLE  0x13
#define      LED4_TOGLE  0x14
#define      LED5_TOGLE  0x15
#define      LED6_TOGLE  0x16
#define      LED7_TOGLE  0x17
#define      LED8_TOGLE  0x18

#define      LED1_ON  0x19
#define      LED2_ON  0x1A
#define      LED3_ON  0x1B
#define      LED4_ON  0x1C
#define      LED5_ON  0x1D
#define      LED6_ON  0x1E
#define      LED7_ON  0x1F
#define      LED8_ON  0x20


 #define     LED1_OFF  0x21
 #define     LED2_OFF  0x22
 #define     LED3_OFF  0x23
 #define     LED4_OFF  0x24
 #define     LED5_OFF  0x25
 #define     LED6_OFF  0x26
 #define     LED7_OFF  0x27
#define      LED8_OFF  0x28

#define      HEZHA  0x30
#define      FENZHA  0x31
#define      WRITE_HEZHA_TIME  0x32 //合闸时间
#define      WRITE_FENZHA_TIME  0x33 //分闸时间

#define      TURN_ON_INT0 0x34
#define      TURN_OFF_INT0 0x35
#define      READ_HEZHA_TIME  0x36 //合闸时间
#define      READ_FENZHA_TIME  0x37 //分闸时间

#define      YONGCI_TONGBU  0x80
#define      YONGCI_TONGBU_RESET  0x81
#define      YONGCI_WAIT_HE_ACTION 0x85 //永磁等待合闸命令

#define      ACK  0xFA


#ifdef	__cplusplus
extern "C" {
#endif

void ExecuteFunctioncode(frameRtu* pRtu);

//void SendSampleData(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ACTION_H */

