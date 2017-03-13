/*****************88
 * 摘要:
 * 2015/8/10:完善远本转换。
 * 2015/11/17: 改变IO检测模式，由纯延时形式，变成采样点样式。
 * 将远方检测IO与本地合并。
***/
#include "SwtichCondition.h"
#include "../Header.h"
#include "DeviceParameter.h"

//本地合闸条件： 分位 && 合闸信号 && 电压满足 && 本地控制
#define HEZHA_CONDITION()    \
                ((ACTIVE_STATE ==FEN_WEI_INPUT) && (ACTIVE_STATE ==HE_INPUT) &&      \
                ( GetCapVolatageState()) && (ACTIVE_STATE !=YUAN_BEN_INPUT))
//本地分闸条件： 合位 && 分闸信号 && 电压满足 && 本地控制
#define FENZHA_CONDITION()   \
                 (ACTIVE_STATE ==HE_WEI_INPUT) && (ACTIVE_STATE ==FEN_INPUT) &&      \
                 (GetCapVolatageState()) && (ACTIVE_STATE !=YUAN_BEN_INPUT)
//远方合闸条件： 分位 && 远方合闸信号 && 电压满足 && 远方控制
#define YUAN_HE_CONDITION() \
                 (ACTIVE_STATE ==FEN_WEI_INPUT) && (ACTIVE_STATE ==HE_YUAN_INPUT) &&  \
                 (GetCapVolatageState()) && (ACTIVE_STATE ==YUAN_BEN_INPUT)
//远方分闸闸条件： 合位 && 远方分闸信号 && 电压满足 && 远方控制
#define YUAN_FEN_CONDITION()  \
                ((ACTIVE_STATE ==HE_WEI_INPUT) && (ACTIVE_STATE ==FEN_YUAN_INPUT) &&  \
                 (GetCapVolatageState()) && (ACTIVE_STATE ==YUAN_BEN_INPUT))

//本地错误条件: 合位和分位同时成立 或 同时不成立
#define ERROR_CONDITION()    \
              (((ACTIVE_STATE ==HE_WEI_INPUT) && (ACTIVE_STATE == FEN_WEI_INPUT)) ||  \
              ((ACTIVE_STATE !=HE_WEI_INPUT) && (ACTIVE_STATE != FEN_WEI_INPUT) ))

#define FENZHA_CONDITION_CONTINOUS()((ACTIVE_STATE == HE_WEI_INPUT) && (ACTIVE_STATE == FEN_INPUT))
#define YUAN_FENZHA_CONDITION_CONTINOUS() ((ACTIVE_STATE == HE_WEI_INPUT) && (ACTIVE_STATE == FEN_YUAN_INPUT))

#define STATE_HE_ZHA    0xAA11
#define STATE_FEN_ZHA   0xAA22

#define STATE_YUAN_HE_ZHA    0x5511
#define STATE_YUAN_FEN_ZHA   0x5522

#define STATE_ERROR    0xABCD

#define STATE_FEN_CONTINOUS        0xAA55
#define STATE_YUAN_FEN_CONTINOUS   0xAA77

uint16 ErrorFlag = 0; //错误标志



/********************************************
*函数名：  GetSwitchState()
*形参：uint16 需要检测的状态
*返回值：uint16 开关状态
*功能：获取开关状态
**********************************************/
uint16 GetSwitchState(uint16 state)
{
    ClrWdt();
    switch (state)
    {
        case STATE_HE_ZHA:
        {
            return  HEZHA_CONDITION();
            break;
        }
          case STATE_FEN_ZHA:
        {
            return  FENZHA_CONDITION();
            break;
        }
          case STATE_YUAN_HE_ZHA:
        {
            return  YUAN_HE_CONDITION();
            break;
        }    
           case STATE_YUAN_FEN_ZHA:
        {
            return  YUAN_FEN_CONDITION();
            break;
        } 
        case STATE_ERROR:
        {
            return ERROR_CONDITION();
        }
        case STATE_FEN_CONTINOUS:
        {
            
            return FENZHA_CONDITION_CONTINOUS();
        }
        case  STATE_YUAN_FEN_CONTINOUS:
        {
            return YUAN_FENZHA_CONDITION_CONTINOUS();
        }
        default : //命令错误
        {
            Reset(); //执行复位
        }
    }
    return 0;
    
}
/********************************************
*函数名：  StateCheck()
*形参：uint16 需要检测的状态
*返回值：uint16 开关状态
*功能：获取开关状态
**********************************************/
uint16 StateCheck(uint16 state)
{
    uint16 trueCn = 0;
    uint16 falseCn = 0;
    uint16 i = 0;
    ClrWdt();
    if (GetSwitchState(state))
    {
        //Delay20ms();;//延迟判断消除抖动
        trueCn = 0;
        falseCn = 0;
        for ( i = 0 ; i < 100; i++)//100*200 = 20ms 实际大于20ms的循环，这要满足 180次为真，则认为条件成立
        {
            __delay_us(100); 
            ClrWdt();
          if (GetSwitchState(state))
            {
                trueCn++;
            }
          else
          {
              falseCn++;
          }
            if (falseCn > 10)
            {
                break; //停止检测
            }

        } 
        if (trueCn > 90) //大于90次则认为指令有效 90%的采样满足要求
        {
            ClrWdt();
            return 0xFFFF;
        }

    }
    return 0;
}

/**************************************************
 *函数名：CheckSwitchAction()
 *功能: 可能存在如下几种情况：
 * 1.故障合位检测与分位检测同时有信号或者无信号，合闸按钮或者
 * 分闸按钮同时有信号，此时判断为故障状态。
 * 2.在合闸状态下，仅能接收分闸指令；分闸状态下，仅能接收合闸指令。
 *形参：  void
 *返回值：void
****************************************************/
uint8  CheckSwitchAction(void) 
{
    
        ClrWdt();
        //合位与分位同时检测 或者 都没检测到
        if (StateCheck(STATE_ERROR))
        {
            ClrWdt();
            ErrorFlag = 0xff;
            return CHECK_ERROR_STATE;
        }
        ErrorFlag = 0;
        //在以上均不成立时 检测合分闸按钮状态
        //在合位时检测 分闸 按钮
         ClrWdt();
        if (StateCheck(STATE_FEN_ZHA))
        {
            ClrWdt();
            FenOnLock();//上锁
            return CHECK_FEN_ORDER;//返回分闸命令
        }
         ClrWdt();
         //判断远分
         if (StateCheck(STATE_YUAN_FEN_ZHA))
        {
           
            ClrWdt();
            FenOnLock();//上锁
            return CHECK_FEN_ORDER;//返回分闸命令
        }
          ClrWdt();
       // 在分位时检测 合闸 按钮 且不是 欠压状态
        if (StateCheck(STATE_HE_ZHA))
        {
            ClrWdt();
            HeOnLock();//上锁
            return CHECK_HE_ORDER;//返回分闸命令
        }
        ClrWdt();
         //判断远合
        if (StateCheck(STATE_YUAN_HE_ZHA))
        {
            ClrWdt();
            HeOnLock();//上锁
            return CHECK_HE_ORDER;//返回分闸命令
        }
        //以上既不是故障 也不是 合分闸命令情况下 检测合分状态
        //分闸状态
         ClrWdt();
        if ( ACTIVE_STATE == FEN_WEI_INPUT )
        {
            return CHECK_FEN_STATE;//快速刷新，取消去抖   
        }
        //合闸状态
         ClrWdt();
         if ( ACTIVE_STATE == HE_WEI_INPUT )
        {
 
             return CHECK_HE_STATE;//快速刷新，取消去抖
              
        }
    //错状态退出
    return  0;
} 
/**************************************************
 *函数名：CheckVoltage()
 *功能:检测电压的状态
 *形参：  void
 *返回值：void
****************************************************/
void CheckVoltage(void)
{
    ClrWdt();
    if ( GetCapVolatageState()) //欠压状态
    {
     
            ClrWdt();
            UpdateIndicateState(ON_DIANYA_LED, TURN_ON);
            UpdateIndicateState(OFF_CHUNENG_RELAY, TURN_OFF);        
    }
    else
    {
         ClrWdt();
         UpdateIndicateState(OFF_DIANYA_LED, TURN_OFF);
         UpdateIndicateState(ON_CHUNENG_RELAY, TURN_ON);
    }
}

/**************************************************
 *函数名：CheckIOState()
 *功能:检测IO状态，并跟新状态显示
 *形参：  void
 *返回值：void
****************************************************/
uint8  CheckIOState(void)
{
    uint8 checkOrder = 0;
    
     //***********************首先检测按钮IO*******开始****************//
    //远控本地切换
    ClrWdt();
   
    checkOrder = CheckSwitchAction();
    
    
    ClrWdt();
           
    switch (checkOrder)
    {
        case CHECK_ERROR_STATE:  //检测到错误
        {
            ClrWdt();      

            //合分位指示灯熄灭
            UpdateIndicateState(OFF_HE_LED & OFF_FEN_LED, TURN_OFF);
            //故障指示灯亮，故障继电器得电动作
            UpdateIndicateState(ON_ERROR_LED | ON_ERROR_RELAY , TURN_ON);
            
            checkOrder = 0;

            break;
        }
        case CHECK_HE_ORDER: //收到合闸命令
        {
             ClrWdt();           
            HeZhaActionA();
            return 0xff;
        }
        case CHECK_FEN_ORDER: //收到分闸命令
        {
             ClrWdt();
             FenZhaActionA();
            return 0xff;
        }
        case CHECK_HE_STATE: //检测到合闸状态
        {
            ClrWdt();
            //合位指示灯亮
            UpdateIndicateState(ON_HE_LED  , TURN_ON);
            
           //关闭分闸LED，故障LED,故障继电器
            UpdateIndicateState(OFF_FEN_LED & OFF_ERROR_LED & OFF_ERROR_RELAY, TURN_OFF);
            
            
            checkOrder = 0;

             //*pstate = IDLE_ORDER;//解除闭锁
            break;
        }
         case CHECK_FEN_STATE: //检测到分闸状态
        {
            ClrWdt();           
           //分位指示灯亮
            UpdateIndicateState(ON_FEN_LED  , TURN_ON);            
           //关闭合位LED，故障LED,故障继电器
            UpdateIndicateState(OFF_HE_LED & OFF_ERROR_LED & OFF_ERROR_RELAY, TURN_OFF);
            checkOrder = 0;

            // *pstate = IDLE_ORDER;//解除闭锁
            break;
        }
        default:
        {
             ClrWdt();
            checkOrder = 0;
        }
    }

       
            return 0;
}
/**************************************************
 *函数名：ContinuousCheckYuan()
 *功能:用于远方合分闸动作完成后的立刻检测。
 *形参：  void
 *返回值：void
****************************************************/
uint8 ContinuousCheckYuan(uint16* lastOrder)
{
    uint16 i = 0;
    if (ACTIVE_STATE != YUAN_BEN_INPUT)
    {
        return 0;
    }
    if (HE_ORDER == *lastOrder)//刚执行完合闸命令
    {
         ClrWdt();
        if (ACTIVE_STATE != HE_YUAN_INPUT)
        {
             ClrWdt();
            *lastOrder = IDLE_ORDER;
            return 0;
        }
        else
        {
            i = 0;
            while(ACTIVE_STATE == HE_YUAN_INPUT)//合闸命令一直存在
            {
                 ClrWdt();
                //处于合位且收到分闸命令
                if (StateCheck(STATE_YUAN_FEN_CONTINOUS))
                {
                    
                    //返回分闸命令
                    ClrWdt();
                    FenOnLock();//上锁
                    return CHECK_FEN_ORDER;
                }
                 ClrWdt();
                __delay_ms(1);
                i++;
                //持续3s认为是错误，进行错误处理
                if (i > 3000)
                {
                    //打开错误指示灯与错误继电器
                      UpdateIndicateState(ON_ERROR_LED & OFF_ERROR_RELAY, TURN_ON);
                }
            }
             ClrWdt();
             *lastOrder = IDLE_ORDER;
             return 0;
        }
    }
     ClrWdt();
    if (FEN_ORDER == *lastOrder)//刚执行完分闸命令
    {
        
       //等待信号结束。
        i = 0;
        while(ACTIVE_STATE == FEN_YUAN_INPUT) //分闸信号一直存在
        {
             ClrWdt();
             __delay_ms(1);
              ClrWdt();
            i++;
            //持续3s认为是错误，进行错误处理
            if (i > 3000)
            {
                UpdateIndicateState(ON_ERROR_LED & OFF_ERROR_RELAY, TURN_ON);
            }
        }
        ClrWdt();
        //若此时有合闸信号，则认为为分闸信号未结束时就有 不执行动作
        //等待信号结束。
        i = 0;
        while(ACTIVE_STATE == HE_YUAN_INPUT)
        {
             ClrWdt();
             __delay_ms(1);
              ClrWdt();
            i++;
            //持续3s认为是错误，进行错误处理
            if (i > 3000)
            {
                UpdateIndicateState(ON_ERROR_LED & OFF_ERROR_RELAY, TURN_ON);
            }
        }
        *lastOrder = IDLE_ORDER;
        return 0;
    }
      *lastOrder = IDLE_ORDER;
     return 0;
    
}
/**************************************************
 *函数名：ContinuousCheck()
 *功能:用于合分闸动作完成后的立刻检测。
 *形参：  void
 *返回值：void
****************************************************/
uint8 ContinuousCheck(uint16* lastOrder)
{
    uint16 i = 0;
    ClrWdt();
    if (ACTIVE_STATE == YUAN_BEN_INPUT)
    {
        return 0;
    }
    if (HE_ORDER == *lastOrder)//刚执行完合闸命令
    {
         ClrWdt();
        if (ACTIVE_STATE != HE_INPUT)//合闸信号已经消失
        {
             ClrWdt();
            *lastOrder = IDLE_ORDER;
            return 0;
        }
        else
        {
            i = 0;
            while(ACTIVE_STATE == HE_INPUT)//合闸命令一直存在
            {
                 ClrWdt();
                //合闸条件 1处于合位 2电压足够 3有效的分闸信号 
              if (StateCheck(STATE_FEN_CONTINOUS))
                {
                    
                    //返回分闸命令
                    ClrWdt();
                    //返回分闸命令
                    ClrWdt();
                    FenOnLock();//上锁
                    return CHECK_FEN_ORDER;
                }
                 ClrWdt();
                __delay_ms(1);
                i++;
                //持续3s认为是错误，进行错误处理
                if (i > 3000)
                {
                  //  ERROR_LED = ON_LED;
                    //ERROR_OUT = OPEN_STATE;
                }
            }
             ClrWdt();
             *lastOrder = IDLE_ORDER;
             return 0;
        }
    }
     ClrWdt();
    if (FEN_ORDER == *lastOrder)//刚执行完分闸命令
    {
        
       //等待信号结束。
        i = 0;
        while(ACTIVE_STATE == FEN_INPUT)
        {
             ClrWdt();
             __delay_ms(1);
              ClrWdt();
            i++;
            //持续3s认为是错误，进行错误处理
            if (i > 3000)
            {
                //ERROR_LED = ON_LED;
                //ERROR_OUT = OPEN_STATE;
            }
        }
        ClrWdt();
        //若此时有合闸信号，则认为为分闸信号未结束时就有 不执行动作
        //等待信号结束。
        i = 0;
        while(ACTIVE_STATE == HE_INPUT)
        {
             ClrWdt();
             __delay_ms(1);
              ClrWdt();
            i++;
            //持续3s认为是错误，进行错误处理
            if (i > 3000)
            {
               // ERROR_LED = ON_LED;
                //ERROR_OUT = OPEN_STATE;
            }
        }
        *lastOrder = IDLE_ORDER;
        return 0;
    }
      *lastOrder = IDLE_ORDER;
     return 0;
    
}
