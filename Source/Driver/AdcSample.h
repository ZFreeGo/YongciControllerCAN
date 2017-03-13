/* 
 * File:   AdcSample.h
 * Author: FreeGo
 *
 * Created on 2015年4月23日, 下午6:34
 */

#ifndef ADCSAMPLE_H
#define	ADCSAMPLE_H

#include "tydef.h"

#ifdef	__cplusplus
extern "C" {
#endif

void AdcInit(void);
uint16 SoftSampleOnce(void);
#define  NUM_CHS2SCAN 1



#ifdef	__cplusplus
}
#endif

#endif	/* ADCSAMPLE_H */

