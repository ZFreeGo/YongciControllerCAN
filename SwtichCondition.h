/* 
 * File:   SwtichCondition.h
 * Author: LiDehai
 *
 * Created on 2015年7月13日, 下午7:45
 */

#ifndef SWTICHCONDITION_H
#define	SWTICHCONDITION_H
#include "tydef.h"


#ifdef	__cplusplus
extern "C" {
#endif

uint8  CheckSwitchAction(void);   

void CheckVoltage(void);    
uint8  CheckIOState(void);    

uint8 ContinuousCheckYuan(uint16* lastOrder); 
uint8 ContinuousCheck(uint16* lastOrder);

#ifdef	__cplusplus
}
#endif

#endif	/* SWTICHCONDITION_H */

