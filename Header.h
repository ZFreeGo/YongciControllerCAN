#ifndef _Header_H_
#define _Header_H_

#define FCY 4e6
#include <libpic30.h>
 #include "p30fxxxx.h"
#include <xc.h>

#include "tydef.h"


#include "Timer.h"
#include "Action.h"
#include "DevicdIO.h"
#include "Usart.h"
#include "RtuFrame.h"
#include "AdcSample.h"
#include "yongci.h"
#include "SwtichCondition.h"

#include "CAN.h"

#define MAIN_ADDRESS    0xF0
#define LOCAL_ADDRESS   0xA2 //双路调试控制板子地址



#define Reset() {__asm__ volatile ("RESET");}
#endif
