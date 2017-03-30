/***********************************************
*Copyright(c) 2015,FreeGo
*保留所有权利
*文件名称:Action.c
*文件标识:
*创建日期： 2015年月4月23日
*摘要:

*2015/4/23:套用历史版本框架.
*当前版本:1.0
*作者: FreeGo
*取代版本:
*作者:
*完成时间:
************************************************************/
#include "Action.h"
#include "../Header.h"

void SendAckMesssage(uint8 fun);


extern volatile uint16 HezhaTimeA;
extern volatile uint16 FenzhaTimeA;

extern volatile uint16 WaitIntOrder;
typedef union union_32_16
{
    struct
    {
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;
    } byteMat;
    uint8 byteArray[4];
    uint32 ushort;
}tris32and8;

extern uint8  volatile SendFrameData[SEND_FRAME_LEN];
/**************************************************
 *函数名： SendAckMesssage()
 *功能：  回传校验码
 *形参：  Uint16 fun 功能代码地址
 *返回值：void
****************************************************/
inline void SendAckMesssage(uint8 fun)
{
    uint16 len = 0;
    ClrWdt();
    GenRTUFrame(LOCAL_ADDRESS, ACK, &fun, 1, (uint8*)SendFrameData, (uint8 *)&len);
    ClrWdt();
    SendFrame((uint8*)SendFrameData, len);
    ClrWdt();
}
/**************************************************
 *函数名： ExecuteFunctioncode()
 *功能：  执行功能代码
 *形参：  接收帧指针 frameRtu* pRtu
 *返回值：void
****************************************************/
void ExecuteFunctioncode(frameRtu* pRtu)
{
    ClrWdt();
    //该数据帧未处理过
    if (pRtu->completeFlag == TRUE)
    {
       // LEDE ^= 1;
        if ( pRtu->funcode != YONGCI_WAIT_HE_ACTION)
        {
            SendAckMesssage( pRtu->funcode);
        }
         ClrWdt();
        switch(pRtu->funcode)
        {
            case RESET_MCU:
            {
                Reset(); //软件复位
                break;
            }
            case LED1_TOGLE:
            {
                        
                break;
            }
            case LED2_TOGLE:
            {
                //LEDB ^= 1;
                break;
            }
            case LED3_TOGLE:
            {
                //LEDC ^= 1;
                break;
            }
            case LED4_TOGLE:
            {
                //LEDD ^= 1;
                break;
            }
            case LED1_ON:
            {
                UpdateIndicateState(LED1_ON, TURN_ON);      
                break;
            }
            case LED2_ON:
            {
                UpdateIndicateState(LED2_ON, TURN_ON);      
                break;
            }
            case LED3_ON:
            {
                 UpdateIndicateState(LED3_ON, TURN_ON);
                break;
            }
            case LED4_ON:
            {
                  UpdateIndicateState(LED4_ON, TURN_ON);
                break;
            }
            case LED1_OFF:
            {
                UpdateIndicateState(LED1_OFF, TURN_OFF);
                break;
            }
            case LED2_OFF:
            {
                UpdateIndicateState(LED2_OFF, TURN_OFF);
                break;
            }
            case LED3_OFF:
            {
                UpdateIndicateState(LED3_OFF, TURN_OFF);
                break;
            }
            case LED4_OFF:
            {
                UpdateIndicateState(LED4_OFF, TURN_OFF);
                break;
            }  
            case TURN_ON_INT0:
            {
                TurnOnInt0();
                break;
            }
            case TURN_OFF_INT0:
            {
                TurnOffInt0();
                break;
            }
            case HEZHA: //立即合闸
            {
                if (!GetRunStateFlagA()) //处于空闲状态
                {
                    HeOnLock();//上锁
                    HeZhaActionA();
                }
                break;
            }
           case FENZHA: //立即分闸
            {
                 ClrWdt();
                 if (!GetRunStateFlagA()) //处于空闲状态
                {
                    FenOnLock();//上锁
                    FenZhaActionA();
                }
                break;
            }
            case WRITE_HEZHA_TIME:
            {
                 ClrWdt();
                if ( ((pRtu->pData)[3] < 101) && ((pRtu->pData)[3] > 0))
                {
                     HezhaTimeA = (pRtu->pData)[3];
                }
                break;
            }
            case WRITE_FENZHA_TIME:
            {
                 ClrWdt();
                if ( ((pRtu->pData)[3] < 101) && ((pRtu->pData)[3] > 0))
                {
                     FenzhaTimeA = (pRtu->pData)[3];
                }
                break;
            }
            case  YONGCI_WAIT_HE_ACTION:
            {
                 ClrWdt();
                WaitIntOrder = WAIT_ORDER;
                break;
            }
             default :
             {
                 break;
             }
        }

        pRtu->completeFlag = FALSE;
    }


}



/**
 * 引用帧服务
 *
 * @param  指向处理帧信息内容的指针
 * @param  数据信息长度
 * @param  mac id
 * @bref   对完整帧进行提取判断
 */
static void FrameServer(uint8* pData, uint8  len, uint8 mac_id)
{
    uint8 id = pData[0];    
    switch(id)
    {
        case 0: //合闸预制
        {
            
            break;
        }
        case 1:
        {
            break;
        }
    
    }
}
