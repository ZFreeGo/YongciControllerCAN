/* 
 * File:   yongci.h
 * Author: ZFreeGo
 *
 * 
 */

#ifndef YONGCI_H
#define	YONGCI_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "tydef.h"

#define ON_HE_LOCK 0x55AA
#define ON_FEN_LOCK 0xAA55    
#define OFF_LOCK 0x0000
    
//IGBT工作状态
#define HE_ORDER   0x5555 //合闸命令
#define FEN_ORDER  0xAAAA //分闸命令
#define IDLE_ORDER 0x0000 //空闲命令

#define WAIT_ORDER 0xBBBB //等待指令
#define NULL_ORDER 0x0000 //等待指令  
    
#define HEZHA_TIME_A  50 //A路 合闸时间 默认
#define FENZHA_TIME_A 30 //A路 分闸时间 默认


        //IO 检测对应状态
#define CHECK_ERROR_STATE 0x21 //错误状态
#define CHECK_HE_ORDER 0x22 //合闸动作
#define CHECK_FEN_ORDER 0x23 //分闸动作
#define CHECK_HE_STATE 0x24 //合闸状态
#define CHECK_FEN_STATE 0x25 //分闸状态

#define HE_COUNT_ADDRESS 0x007FFC60
#define FEN_COUNT_ADDRESS 0x007FFC80
    
void YongciMainTask(void);
void YongciStopCurrentA(void);
void YongciFirstInit(void);

void HeZhaActionA(void);
void FenZhaActionA(void);

void SetRunStateFlagA(uint16 newFlag);
uint16 GetRunStateFlagA();

 void HeOnLock(void);
 void FenOnLock(void);
 void OffLock(void);

void UpdateCount(uint16 states);
#ifdef	__cplusplus
}
#endif

#endif	/* YONGCI_H */

