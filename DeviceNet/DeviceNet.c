/***********************************************
*Copyright(c) 2017,FreeGo
*保留所有权利
*文件名称:CAN.c
*文件标识:
*创建日期： 2017年3月6日 
*摘要:
*2017/3/6 : 移植部分DeviceNet
*当前版本:1.0
*作者: FreeGo
*取代版本:
*作者:
*完成时间:
************************************************************/
#include "DeviceNet.h"
#include "tydef.h"

//////////////////////函数申明/////////////////////////////////
void ResponseMACID(void);
void VisibleMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame);
void CycleInquireMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame);
void UnconVisibleMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame);
////////////////////连接对象变量////////////////////////////////
struct DefConnectionObj  CycleInquireConnedctionObj;//循环响应
struct DefConnectionObj  VisibleConnectionObj;   //显示连接
//////////////////DeviceNet对象变量////////////////////////////
struct DefDeviceNetClass  DeviceNet_class = {2}; 
struct DefDeviceNetObj  DeviceNetObj;
struct DefIdentifierObject  IdentifierObj; 
/////////////////////////////////////////////////////////////////
BYTE  send_buf[10];//can总线发送数组 数组第一字节为ID的高8bit，第二字节的高3bit，ID的低3bit

BYTE  SendBuffer[10];//接收缓冲数据
BYTE  ReciveBuffer[10];//接收缓冲数据
BYTE  out_Data[8];//从站输出数组
struct DefFrameData  ReciveFrame; //接收帧处理
struct DefFrameData  SendFrame; //接收帧处理

/*******************************************************************************
* 函数名:	void InitDeviceNet()
* 形参  :	null
* 返回值:    null
* 功能描述:	初始化DeviceNet所涉及的基本数据
*******************************************************************************/
void InitDeviceNet()
{
    ReciveFrame.complteFlag = 0xff;
    ReciveFrame.pBuffer = ReciveBuffer;
    SendFrame.complteFlag = 0xff;
    SendFrame.pBuffer = SendBuffer;
}

/*******************************************************************************
* 函数名:	void DeviceNetClassService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
* 形参  :	DefFrameData * pFrame，接收报文数组 
* 返回值:    	无
* 功能描述:	DeviceNet分类服务函数
            DeviceNet分类只有1个属性，可选执行Get_Attribute_Single服务，响应其版本信息
*******************************************************************************/
void DeviceNetClassService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{  
    
	if(pReciveFrame->pBuffer[1] != SVC_GET_ATTRIBUTE_SINGLE)        //不支持的服务
	{	
        //组2信息+源MAC ID(从站ID) 
        //信息id=3，表示从站响应主站的显示请求
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);        //
		pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);        //目的MAC ID(主站ID) 
		pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;  //R/R=1表示响应，SVC_ERROR_RESPONSE，错误响应服务代码
		pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;    //ERR_SERVICE_NOT_SUPPORT ,不支持的服务
		pSendFrame->pBuffer[3] = ERR_RES_INAVAIL;            //ERR_RES_INAVAIL ,对象执行服务的资源不可用(附加错误代码)
		pSendFrame->len = 4;
        //发送
		SendData(pSendFrame);                    //发送错误响应报文
		return ;
	}
	if(pReciveFrame->pBuffer[3] != 1)                               //不存在的属性
	{	
	    pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
		pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
		pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
		pSendFrame->pBuffer[2] = ERR_ID_INAVAIL;             //ERR_ID_INAVAIL ,不存在指定的类/实例/属性ID(附加错误代码)
		pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;     //ERR_NO_ADDITIONAL_DESC ,无附加描述代码
        pSendFrame->len = 4;
		//发送
		SendData(pSendFrame);                    //发送错误响应报文
		return ;
	}
	//执行显示请求
	pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);            //目的MAC ID(主站ID) 
	pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;//R/R=1表示响应，VC_Get_Attribute_Single服务代码
	pSendFrame->pBuffer[2] = DeviceNet_class.version;
	pSendFrame->pBuffer[3] = DeviceNet_class.version >> 8;   //类的版本信息
    pSendFrame->len = 4;
	//发送
	SendData(pSendFrame);                        //发送显示信息的响应报文
}
/******************************************************************************** 
*函数名 : void DeviceNetObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
*形  参 : DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收报文数组 
*返回值 :  	无
*功能描述: DeviceNet对象服务函数
********************************************************************************/
void DeviceNetObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
   
    switch( pReciveFrame->pBuffer[1])
    {
    case (SVC_GET_ATTRIBUTE_SINGLE):         //获取单个属性服务
	{        
        switch(pReciveFrame->pBuffer[3]) //实例ID
        {
            case DEVICENET_OBJ_MACID:
            {
                pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);              
                pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);    //目的MAC ID(主站ID) 
                pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;//R/R=1表示响应，SVC_GET_ATTRIBUTE_SINGLE服务代码
                pSendFrame->pBuffer[2] = DeviceNetObj.MACID;    //源MAC ID(从站ID) 
                pSendFrame->len = 3;
                //发送
                SendData(pSendFrame);                //发送显示信息的响应报文
                return ;             
            }
            case  DEVICENET_OBJ_BAUD:
            {
                pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);  
                pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F); 
                pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
                pSendFrame->pBuffer[2] = DeviceNetObj.baudrate;//从站波特率
                pSendFrame->len = 3;
                //发送
                SendData(pSendFrame);
                return;
            }
            case DEVICENET_OBJ_MASTERID:
            {
                pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);  
                pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F); 
                pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
                pSendFrame->pBuffer[2] = DeviceNetObj.assign_info.select;       //分配选择
                pSendFrame->pBuffer[3] = DeviceNetObj.assign_info.master_MACID; //主站MAC ID 
                pSendFrame->len = 4;
                //发送
                SendData(pSendFrame);
                return ;
            }
            default:
            {
                pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
                pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);       //目的MAC ID(主站ID) 
                pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE; //R/R=1表示响应，SVC_ERROR_RESPONSE，错误响应服务代码
                pSendFrame->pBuffer[2] = ERR_ID_INAVAIL;            //ERR_ID_INAVAIL ,不存在指定的类/实例/属性ID(附加错误代码)
                pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;    //ERR_NO_ADDITIONAL_DESC ,无附加描述代码
                pSendFrame->len = 4;
                //发送
                SendData(pSendFrame);                   //发送错误响应报文
                return ;
            }        
        }  
        break;
	}
	case ( SVC_SET_ATTRIBUTE_SINGLE):      //设置单个属性服务
	{
        switch (pReciveFrame->pBuffer[3])
        {
            case DEVICENET_OBJ_MACID:
            case DEVICENET_OBJ_BAUD:
            case DEVICENET_OBJ_MASTERID://不支持设置节点地址、波特率、分配选择和主站MAC ID 
            {
                pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
                pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
                pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
                pSendFrame->pBuffer[2] = ERR_PROPERTY_NOT_SET;
                pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;
                pSendFrame->len = 4;
                //发送
                SendData(pSendFrame);
                return ;              
            }
            default:
            {
                pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
                pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
                pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
                pSendFrame->pBuffer[2] = ERR_ID_INAVAIL;
                pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;
                pSendFrame->len = 4;
                //发送
                SendData(pSendFrame);
                return ;			           
            }
        }		
	}
	case (SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET)://建立连接服务代码
    case (SVC_RELEASE_GROUP2_IDENTIFIER_SET):  //释放组2连接服务
	{	
		UnconVisibleMsgService(pSendFrame);                            //非连接显式信息服务，
        break;
	}	
	default:
	{
		pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
		pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
		pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
		pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
		pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;
        pSendFrame->len = 4;
		//发送
		SendData(pSendFrame);
		return ;
	}
    }
}
/*******************************************************************************
* 函数名:	void ConnectionClassService(BYTE  *buf)
* 形  参 :	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	连接类服务函数
********************************************************************************/
void ConnectionClassService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);               //目的MAC ID(主站ID) 
	pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;         //R/R=1表示响应，SVC_ERROR_RESPONSE，错误响应服务代码
	pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;           //ERR_SERVICE_NOT_SUPPORT ,不支持的服务
	pSendFrame->pBuffer[3] = 0x01;                              //附加错误代码
    pSendFrame->len = 4;
	SendData(pSendFrame);//发送
	return ;
}
/*******************************************************************************
* 函数名:	void VisibleConnectObjService(BYTE  *buf)
* 形参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	显式信息连接服务函数
*******************************************************************************/
void VisibleConnectObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);     // 目的MAC ID(主站ID) 
    
    switch( pReciveFrame->pBuffer[1])
    {
        case SVC_SET_ATTRIBUTE_SINGLE://设置服务，#define SVC_SET_ATTRIBUTE_SINGLE	0x10
        {
            pSendFrame->pBuffer[1]= 0x80 | SVC_SET_ATTRIBUTE_SINGLE;// R/R=1表示响应，pReciveFrame->pBuffer[1]是接收报文中的服务代码
            pSendFrame->pBuffer[2] =  pReciveFrame->pBuffer[5];
            pSendFrame->pBuffer[3] =  pReciveFrame->pBuffer[6];;
            pSendFrame->len = 4;
            break;       
        }
        case SVC_GET_ATTRIBUTE_SINGLE://获取服务 
        {
            pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
            if(pReciveFrame->pBuffer[4] == 7)                   //属性ID为7，表示通过本连接发送的最大字节数为8个
            {	 			
                pSendFrame->pBuffer[2] = 0x08;
                pSendFrame->pBuffer[3] = 0;
                pSendFrame->len = 4;
            }
            else if(pReciveFrame->pBuffer[4]== 8)              //属性ID为8，表示通过本连接接收的最大字节数为8个
            { 
                pSendFrame->pBuffer[2] = 0x08;
                pSendFrame->pBuffer[3] = 0;
                 pSendFrame->len = 4;
            }
            else
            {
                pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
                pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
                pSendFrame->pBuffer[3] = 0x01;
                pSendFrame->len = 4;
            }
            break;
        }
        default:
        {
            pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
            pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
            pSendFrame->pBuffer[3] = 0x01;
            pSendFrame->len = 4;
            break;
        }            
    }  	
	//发送
	SendData(pSendFrame);
}
/********************************************************************************
* 函数名:	void CycInquireConnectObjService(BYTE  *buf)
* 形  参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	轮询信息连接实例服务函数
********************************************************************************/
void CycInquireConnectObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);	
	if(pReciveFrame->pBuffer[1] == SVC_SET_ATTRIBUTE_SINGLE)
	{
		pSendFrame->pBuffer[1]= 0x80 | SVC_SET_ATTRIBUTE_SINGLE;
        pSendFrame->pBuffer[2] = pReciveFrame->pBuffer[5];
        pSendFrame->pBuffer[3] = pReciveFrame->pBuffer[6];
		CycleInquireConnedctionObj.state = 4;
	}
	else if(pReciveFrame->pBuffer[1] == SVC_GET_ATTRIBUTE_SINGLE)
	{
		pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
		if(pReciveFrame->pBuffer[4] == 7)//属性ID为7，表示通过本连接发送的最大字节数
		{		
			pSendFrame->pBuffer[2] = 0x08;
			pSendFrame->pBuffer[3] = 0;
             pSendFrame->len = 4;
		}
		else if(pReciveFrame->pBuffer[4] == 8)//属性ID为8，表示通过本连接接收的最大字节数
		{
			pSendFrame->pBuffer[2] = 0x08;
			pSendFrame->pBuffer[3] = 0;
             pSendFrame->len = 4;
		}
	}
	else
	{
		pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
		pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
		pSendFrame->pBuffer[3] = 0x01;
        pSendFrame->len = 4;
	}
	SendData(pSendFrame);
}

/*******************************************************************************
* 函数名:	void IdentifierClassService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
* 形参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	标识符类服务, 不支持任何服务，错误响应
********************************************************************************/
void IdentifierClassService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
	pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
	pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
	pSendFrame->pBuffer[3] = 0x01;
     pSendFrame->len = 4;
	SendData(pSendFrame);
	return ;
}
/*******************************************************************************
* 函数名:	void IdentifierObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
* 形  参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	标识符对象服务函数，响应主站有关标示符的请求
********************************************************************************/   
void IdentifierObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	if(pReciveFrame->pBuffer[1] == SVC_GET_ATTRIBUTE_SINGLE)
	{
		pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
		pSendFrame->pBuffer[0] = pReciveFrame->pBuffer[0] & 0x7F;  
		pSendFrame->pBuffer[1]= 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
        
        switch(pReciveFrame->pBuffer[4])
        {
            case IDENTIFIER_OBJ_SUPPLIERID:  //属性ID1，表示用数字标识各供应商
            {
                pSendFrame->pBuffer[2] = IdentifierObj.providerID;
			    pSendFrame->pBuffer[3] = IdentifierObj.providerID >> 8;
                pSendFrame->len = 4;
			    SendData(pSendFrame);                
                break;
            }
            case IDENTIFIER_OBJ_TYPE://属性ID2，表示产品通用类型说明
            {
                pSendFrame->pBuffer[2] = IdentifierObj.device_type;
			    pSendFrame->pBuffer[3] = IdentifierObj.device_type >> 8;
                pSendFrame->len = 4;
			    SendData(pSendFrame);
                break;
            }
            case IDENTIFIER_OBJ_CODE://属性ID3，表示产品代码
            {
                pSendFrame->pBuffer[2] = IdentifierObj.product_code;
			    pSendFrame->pBuffer[3] = IdentifierObj.product_code >> 8;
                pSendFrame->len = 4;
			    SendData(pSendFrame);
                break;
            }
            case IDENTIFIER_OBJ_VERSION://属性ID4，表示产品版本
            {
                pSendFrame->pBuffer[2] = IdentifierObj.version.major_ver;
			    pSendFrame->pBuffer[3] = IdentifierObj.version.minor_ver;
                pSendFrame->len = 4;
			    SendData(pSendFrame);
                break;
            }
             case IDENTIFIER_OBJ_STATUES://属性ID5，表示设备状态概括
             {
                 pSendFrame->pBuffer[2] = IdentifierObj.device_state;
			     pSendFrame->pBuffer[3] = IdentifierObj.device_state >> 8;
                 pSendFrame->len = 4;
			     SendData(pSendFrame);
                 break;
             }
            case IDENTIFIER_OBJ_SERIALNUM://属性ID6，表示设备序列号
            {
             	pSendFrame->pBuffer[2] = IdentifierObj.serialID;
			    pSendFrame->pBuffer[3] = IdentifierObj.serialID >> 8;
                pSendFrame->pBuffer[4] = IdentifierObj.serialID >> 16;
                pSendFrame->pBuffer[5] = IdentifierObj.serialID >> 24;
                pSendFrame->len = 6;
                SendData(pSendFrame);
                break;
            }
            case IDENTIFIER_OBJ_NAME://属性ID7，表示产品名称
            {
                pSendFrame->pBuffer[2] = IdentifierObj.product_name.length;
                pSendFrame->pBuffer[3] = IdentifierObj.product_name.ucdata[0];
                pSendFrame->pBuffer[4] = IdentifierObj.product_name.ucdata[1];
                pSendFrame->pBuffer[5]= IdentifierObj.product_name.ucdata[2];
                pSendFrame->pBuffer[6] = IdentifierObj.product_name.ucdata[3];
                pSendFrame->pBuffer[7] = IdentifierObj.product_name.ucdata[4];
                 pSendFrame->len = 8;
                SendData(pSendFrame);
                break;
            }
            default://不支持的服务
            {            
                pSendFrame->pBuffer[1]= 0x80 | SVC_ERROR_RESPONSE;
                pSendFrame->pBuffer[2] = ERR_PROPERTY_NOT_SET;
                pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;
                pSendFrame->len = 4;
                SendData(pSendFrame);
                break;
            }
        }
    }

}
/********************************************************************************
* 函数名:	void RountineClassObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
* 形参:	无
* 返回值:    	无
* 功能描述:	信息路由器服务，不支持任何服务，错误响应
********************************************************************************/  
void RountineClassObjService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
	pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
	pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
	pSendFrame->pBuffer[3] = 0x01;
    pSendFrame->len = 4;
	SendData(pSendFrame);
	return ;	
}
/******************************************************************************* 
* 函数名:	void InitVisibleConnectionObj(void)
* 形  参:	无
* 返回值:    	无
* 功能描述:	显式信息连接配置函数
********************************************************************************/   
void InitVisibleConnectionObj(void)
{
	VisibleConnectionObj.state = 0x03;	                //已建立状态
	VisibleConnectionObj.instance_type = 0x00;	        //显式信息连接
	VisibleConnectionObj.transportClass_trigger = 0x83;	//传输分类3
	VisibleConnectionObj.produced_connection_id = 0xFF;	
	VisibleConnectionObj.consumed_connection_id = 0xFF;	
	VisibleConnectionObj.initial_comm_characteristics = 0x22;	//通过信息组1生产,通过信息组2消费
	VisibleConnectionObj.produced_connection_size = 0xFF;	    
	VisibleConnectionObj.consumed_connection_size = 0xFF;		//最大接受FF字节
	VisibleConnectionObj.expected_packet_rate = 0x09C4;		    //必须设定期望包速率
	VisibleConnectionObj.watchdog_timeout_action = 1;		    //转为超时状态
	VisibleConnectionObj.produced_connection_path_length = 0;	
	VisibleConnectionObj.consumed_connection_path_length = 0xFF;	
	VisibleConnectionObj.produced_inhibit_time = 0;		        //无时间间隔
}
/******************************************************************************* 
* 函数名:	void InitCycleInquireConnectionObj(void)
* 返回值:    	无
* 功能描述:	I/O轮询连接配置函数
* 形参:	无
********************************************************************************/    
void InitCycleInquireConnectionObj(void)
{
	CycleInquireConnedctionObj.state = 0x01;	                //配置状态 
	CycleInquireConnedctionObj.instance_type = 0x01;	        //I/O连接
	CycleInquireConnedctionObj.transportClass_trigger = 0x82;	//传输分类2
	CycleInquireConnedctionObj.produced_connection_id = 0xFF;	
	CycleInquireConnedctionObj.consumed_connection_id = 0xFF;	
	CycleInquireConnedctionObj.initial_comm_characteristics = 0x01;	//通过信息组1生产,通过信息组2消费
	CycleInquireConnedctionObj.produced_connection_size = 0xFF;	    
	CycleInquireConnedctionObj.consumed_connection_size = 0xFF;	    
	CycleInquireConnedctionObj.expected_packet_rate = 0;		        //设定期望包速率
	CycleInquireConnedctionObj.watchdog_timeout_action = 0;		    //转为超时状态
	CycleInquireConnedctionObj.produced_connection_path_length = 0xFF;	
	CycleInquireConnedctionObj.consumed_connection_path_length = 0xFF;	
	CycleInquireConnedctionObj.produced_inhibit_time = 0;		        //无时间间隔
}
/*******************************************************************************  
** 函数名:	void CANFrameFilter(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
** 形参:	    DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收的报文数组 
** 返回值:    	无
** 功能描述:	CAN信息过滤器函数，提取帧ID1和帧ID2中的信息，
                仅限组2设备，并对信息进行分类处理
*******************************************************************************/
void CANFrameFilter(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
    BYTE mac = GET_GROUP2_MAC(pReciveFrame->ID);
    BYTE function = GET_GROUP2_FUNCTION(pReciveFrame->ID);
	
	if(mac == DeviceNetObj.MACID)  //仅限组2设备
	{	        
        switch( function)
        {
            case GROUP2_REPEAT_MACID:  //重复MAC ID检查信息
            {          
                if (mac == DeviceNetObj.MACID)
                {
                    ResponseMACID(pReciveFrame, 0x80);       //重复MACID检查响应函数,应答，物理端口为0
                }
                return; //MACID不匹配,丢弃
            }
            case GROUP2_VSILBLE_ONLY2: //1100 0000：非连接显示请求信息，预定义主从连接
            {                
				UnconVisibleMsgService(pReciveFrame, pSendFrame);    //非连接显式信息服务 
                return;
            }
            case GROUP2_POLL_ACK: //主站I/O轮询命令/状态变化/循环信息
            {     
                CycleInquireMsgService(pReciveFrame, pSendFrame);     // I/O轮询信息服务
                return ;    
            }
            case GROUP2_VSILBLE:  //主站显式请求信息
            {               
				VisibleMsgService(pReciveFrame, pSendFrame);        //显式信息服务
                break;
            }
        }
	}
}
/******************************************************************************
* 函数名:	void ResponseMACID(void)
* 形参:	无
* 返回值:    	无
* 功能描述:	检查重复MACID响应函数
******************************************************************************/
void ResponseMACID(DefFrameData* pSendFrame, BYTE config)
{                        //重复MACID检查
    pSendFrame->ID =  MAKE_GROUP2_ID( GROUP2_REPEAT_MACID, DeviceNetObj.MACID); 
	pSendFrame->pBuffer[0] = config;	                        //请求/响应标志=1，表示响应，端口号0
	pSendFrame->pBuffer[1]= IdentifierObj.providerID;	//制造商ID低字节
	pSendFrame->pBuffer[2] = IdentifierObj.providerID >> 8;	//制造商ID高字节
	pSendFrame->pBuffer[3] = IdentifierObj.serialID;	                    //序列号低字节
	pSendFrame->pBuffer[4] = IdentifierObj.serialID >> 8;                //序列号中间字节1
	pSendFrame->pBuffer[5] = IdentifierObj.serialID >>16;          //序列号中间字节2
	pSendFrame->pBuffer[6] = IdentifierObj.serialID >>24;	//序列号高字节
    pSendFrame->len = 7;
	SendData(pSendFrame);                      //发送报文
}
/*******************************************************************************
* 函数名:	BYTE CheckMACID(void)
* 功能描述:	主动检查重复MACID函数。
* 形参:	无
* 返回值:       TRUE    网络上有和自己重复的地址
                FALSE   网络上没有和自己重复的地址   	
*******************************************************************************/
BYTE CheckMACID(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{	
    int sendCount = 0;
 
    do
    {
         pReciveFrame->complteFlag = 0;
           //发送请求
        ResponseMACID( pSendFrame, 0);
        StartOverTimer();//启动超时定时器
        while( IsOverTime())
        {
            if ( pReciveFrame->complteFlag)//判断是否有未发送的数据
            {
                BYTE mac = GET_GROUP2_MAC(pReciveFrame->ID);
                BYTE function = GET_GROUP2_FUNCTION(pReciveFrame->ID);
                if (function == GROUP2_REPEAT_MACID)
                {                  
                    if (mac == DeviceNetObj.MACID)
                    {
                          return TRUE; //只要有MACID一致，无论应答还是发出，均认为重复                  
                    }
                }                
                else
                {
                    continue;
                }
            }
        }
        sendCount ++;        
    }
    while(++sendCount < 2);
    
	return FALSE;	//没有重复地址
}
/********************************************************************************
** 函数名:	void CheckAllocateCode(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
** 形参:	DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收的报文数组 
** 返回值:      BYTE 0-溢出 非零为检测通过 	
** 功能描述:j检测非连接显式信息服务设置连接代码
********************************************************************************/
BYTE CheckAllocateCode(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{  
    //如果已分配主站,则检查是否来自同一主站
    if((IdentifierObj.device_state & 0x01) && (*(buf + 7) != DeviceNetObj.assign_info.master_MACID))	//验证主站
    {	//不是来自当前主站，错误响应
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
        pSendFrame->pBuffer[2] = ERR_OBJECT_STATE_INFLICT;
        pSendFrame->pBuffer[3] = 0x01;
        pSendFrame->len = 4;
        SendData(pSendFrame);  //发送报文
        return 0;	
    }
    //仅限组2无连接信息，这个报文指向DeviceNet对象，其类ID为3。在每个DeviceNet的物理连接中只有一个DeviceNet类的实例，因此实例ID为1
    if(pReciveFrame->pBuffer[2] != 3 || pReciveFrame->pBuffer[3] != 1)	//验证类ID和实例ID
    {   //验证类ID和实例ID错误，错误响应
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);       
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);//#define SVC_ERROR_RESPONSE		0x14
        pSendFrame->pBuffer[2] = ERR_PROPERTY_VALUE_INAVAIL;
        pSendFrame->pBuffer[3] = ERR_NO_ADDITIONAL_DESC;
        pSendFrame->len = 4;
        SendData(pSendFrame);//发送报文
        return 0;	
    }
    if(pReciveFrame->pBuffer[4] == 0)	//验证分配选择字节
    {//配置字节为零，主站没有配置从站，错误响应
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);     
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
        pSendFrame->pBuffer[2] = (0x80 | ERR_PROPERTY_VALUE_INAVAIL);
        pSendFrame->pBuffer[3] = 0x02;
        pSendFrame->len = 4;
        SendData(pSendFrame);//发送报文
        return 0;
    }
    if(pReciveFrame->pBuffer[4] & ~(CYC_INQUIRE | VISIBLE_MSG | 0x04))  
    {//如果不是轮询配置、显示连接、位选通，错误响应
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);     
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
        pSendFrame->pBuffer[2] = (0x80 | ERR_RES_INAVAIL);
        pSendFrame->pBuffer[3] = 0x02;
         pSendFrame->len = 4;
        //发送
        SendData(pSendFrame);
        return 0;
    }
    
    return 0xff;
}
    
/********************************************************************************
** 函数名:	void CheckReleaseCode(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
** 形参:	DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收的报文数组 
** 返回值:      BYTE 0-溢出 非零为检测通过 	
** 功能描述:检测非连接显式信息服务释放连接代码
********************************************************************************/
BYTE CheckReleaseCode(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{  
    if((pReciveFrame->pBuffer[4] == 0) || ((pReciveFrame->pBuffer[4] & 0x3F) != 0))   //如果配置字节为0
    {	
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);      
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_RELEASE_GROUP2_IDENTIFIER_SET);
        pSendFrame->len = 2;
        SendData(4, send_buf);
        return 0;
    }
    if(pReciveFrame->pBuffer[4] & ~(CYC_INQUIRE | VISIBLE_MSG | 0x04))//不支持的连接，错误响应
    {	
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);   
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
        pSendFrame->pBuffer[2] = ERR_RES_INAVAIL;
        pSendFrame->pBuffer[3] = 0x02;
        pSendFrame->len = 4;
        SendData(pSendFrame);
        return 0;
    }
    if(pReciveFrame->pBuffer[4] & DeviceNetObj.assign_info.select)//连接不存在，错误响应
    {	
        pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);   
        pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
        pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
        pSendFrame->pBuffer[2] = ERR_EXISTED_MODE;
        pSendFrame->pBuffer[3] = 0x02;
        pSendFrame->len = 4;
        SendData(pSendFrame);
        return 0;
    }
    return 0xff;
}


/********************************************************************************
** 函数名:	void UnconVisibleMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
** 形参:	DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收的报文数组 
** 返回值:      无 	
** 功能描述:	非连接显式信息服务函数，主站用该报文命令从站配置连接
********************************************************************************/
void UnconVisibleMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{

    if(pReciveFrame->pBuffer[1] == SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET)//pReciveFrame->pBuffer[1]是收到的服务代码
	                                                          //如果是建立连接服务代码 
                                                              
	{
        if (!CheckAllocateCode(buf))
        {          
            //检测未通过返回
            return;
        }        
        
		DeviceNetObj.assign_info.master_MACID = pReciveFrame->pBuffer[5];  //主站告诉从站：主站的地址
		DeviceNetObj.assign_info.select |= pReciveFrame->pBuffer[4];       //配置字节
        

		if(pReciveFrame->pBuffer[4] & CYC_INQUIRE)                          //分配I/O轮询连接
		{	
			InitCycleInquireConnectionObj();                       //I/O轮询连接配置函数
			CycleInquireConnedctionObj.produced_connection_id = 0x7800 | (DeviceNetObj.MACID << 5);//	produced_connection_id
			CycleInquireConnedctionObj.consumed_connection_id = 0x80A0 | (DeviceNetObj.MACID << 8);// consumed_connection_id
 	        //成功执行响应
			pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);   
			pSendFrame->pBuffer[0] = pReciveFrame->pBuffer[0] & 0x7F;   // 目的MAC ID(主站ID) 
			pSendFrame->pBuffer[1]= (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
			pSendFrame->pBuffer[2] = 0;	               //信息体格式0,8/8：Class ID = 8 位整数，Instance ID = 8 位整数
             pSendFrame->len = 3;
			SendData(pSendFrame);             //发送报文
			return ;
		}
		if(pReciveFrame->pBuffer[4] & VISIBLE_MSG)
		{	
			InitVisibleConnectionObj();//分配显式信息连接  
			VisibleConnectionObj.produced_connection_id = 0x7800 | (DeviceNetObj.MACID << 5);
			VisibleConnectionObj.consumed_connection_id = 0x8080 | (DeviceNetObj.MACID << 8);
			//成功执行响应
			pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);   
			pSendFrame->pBuffer[0] = pReciveFrame->pBuffer[0] & 0x7F;
			pSendFrame->pBuffer[1]= (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
			pSendFrame->pBuffer[2] = 0;	//信息体格式0,8/8：类Class ID = 8 位整数，实例Instance ID = 8 位整数
             pSendFrame->len = 3;
			//发送
			SendData(pSendFrame);			
		}
		if(pReciveFrame->pBuffer[4] & 0x04) //分配位选通连接
		{	
		
			pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);   
			pSendFrame->pBuffer[0] = pReciveFrame->pBuffer[0] & 0x7F;
			pSendFrame->pBuffer[1]= (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
			pSendFrame->pBuffer[2] = 0;	//信息体格式0,8/8
            pSendFrame->len = 3;
			SendData(pSendFrame);
			return ;
		}
		IdentifierObj.device_state |= 0x01;	//设备已和主站连接
		return ;
	}
	else if(pReciveFrame->pBuffer[1] == SVC_RELEASE_GROUP2_IDENTIFIER_SET)   //释放连接服务
	{
		if(!CheckReleaseCode(buf))
        {
            return;
        }
		//释放连接
		DeviceNetObj.assign_info.select = pReciveFrame->pBuffer[4];
		if(pReciveFrame->pBuffer[4] == 0)
			IdentifierObj.device_state &= ~0x01;	//指示已和主站断开所有连接
		//执行成功响应
		pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);   
		pSendFrame->pBuffer[0] = pReciveFrame->pBuffer[0] & 0x7F;
		pSendFrame->pBuffer[1]= (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
		pSendFrame->pBuffer[2] = 0;	//信息体格式0,8/8
         pSendFrame->len = 3;
		SendData(pSendFrame);
	}
	else
	{	//组2非连接显式信息服务不支持，错误响应
		pSendFrame->ID =  MAKE_GROUP2_ID(GROUP2_VISIBLE_UCN, DeviceNetObj.MACID);
		pSendFrame->pBuffer[0] = (pReciveFrame->pBuffer[0] & 0x7F);
		pSendFrame->pBuffer[1]= (0x80 | SVC_ERROR_RESPONSE);
		pSendFrame->pBuffer[2] = ERR_SERVICE_NOT_SUPPORT;
		pSendFrame->pBuffer[3] = 0x02;
         pSendFrame->len = 4;
		SendData(pSendFrame);
		return ;
	}
}
/*********************************************************************************
** 函数名:	void VisibleMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
** 形参:	DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收的报文数组 
** 返回值:      无 	
** 功能描述:	显式信息服务函数，执行主站的显示请求响应
*********************************************************************************/
void VisibleMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	BYTE class, obj;

	class = pReciveFrame->pBuffer[2]; //类ID
	obj = pReciveFrame->pBuffer[3];   //实例ID

	//信息路由
	if(!(DeviceNetObj.assign_info.select & VISIBLE_MSG))	//没有建立显式信息连接 
		return ;
    
    switch(class)
    {
        case 0x01: //标识符对象
        {   
            if(obj == 0)	    //类服务
            {
                IdentifierClassService( pReciveFrame, pSendFrame);
            }
            else if(obj == 1)	//实例1服务	
            {
                IdentifierObjService(pReciveFrame, pSendFrame);
            }              
            break;
        }
        case 0x02: //信息路由器对象
        {
            RountineClassObjService(pReciveFrame, pSendFrame);
            break;
        }
        case 0x03://DeviceNet对象
        {
            if(obj == 0)	    //类服务
            {
                DeviceNetClassService(pReciveFrame, pSendFrame);
                return ;
            }
            else if(obj == 1)	//实例1服务
            {
                DeviceNetObjService(pReciveFrame, pSendFrame);
                return ;
            }
            break;
        }
        case 0x05:	//连接对象
        {
            if(obj == 0)	    //类服务
            {
                ConnectionClassService(pReciveFrame, pSendFrame); 
                return ;
            }
            else if(obj == 1)	//显式信息连接
            {
                VisibleConnectObjService(pReciveFrame, pSendFrame); 
                return ;
            }
            else if(obj == 2)	//I/O轮询连接
            {
                CycInquireConnectObjService(pReciveFrame, pSendFrame);
                return ;
            }
            break;
        }   
    } 
}
/********************************************************************************
** 函数名:	void  CycleInquireMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
** 功能描述:	I/O轮询信息服务函数，在主站和从站之间传输数据
** 形参:	DefFrameData* pReciveFrame, DefFrameData* pSendFrame，接收的报文数组 
** 返回值:      无 	
*********************************************************************************/
void  CycleInquireMsgService(DefFrameData* pReciveFrame, DefFrameData* pSendFrame)
{
	
     out_Data[0] = pReciveFrame->pBuffer[0];
     out_Data[1] = pReciveFrame->pBuffer[1];
     out_Data[2] = pReciveFrame->pBuffer[2];
     out_Data[3] = pReciveFrame->pBuffer[3];          //保存主站设置的数据

    if(CycleInquireConnedctionObj.state != 4)	//轮询I/O连接没建立
		return ;

	//pSendFrame->pBuffer[0] = 0x78 | DeviceNetObj.MACID >> 3;
	//pSendFrame->pBuffer[1] = DeviceNetObj.MACID << 5;  //发送数组的帧ID
    pSendFrame->ID =  MAKE_GROUP1_ID(GROUP1_POLL_ACK, DeviceNetObj.MACID);   
    pSendFrame->pBuffer[0] = 0x1A;
    pSendFrame->pBuffer[1] = 0x2A;
    pSendFrame->pBuffer[2] = 0x3A;
    pSendFrame->pBuffer[3] = 0x4A;
    pSendFrame->pBuffer[4] = 0x5A;
    pSendFrame->pBuffer[5] = 0x6A;
    pSendFrame->pBuffer[6] = 0x7A;
    pSendFrame->pBuffer[7] = 0x8A;
    pSendFrame->len = 8;
  	SendData(pSendFrame);
	return ;
}
/*******************************************************************************
** 函数名:	void DeviceMonitorPluse(void)
** 功能描述:	设备监测脉冲函数
** 形参:	无
** 返回值:      无 	
********************************************************************************/
void DeviceMonitorPluse(void)
{
//	if(query_time_event(2))
//	{
//		start_time(160);
//		//发送检测脉冲
//		pSendFrame->pBuffer[0] = 0x80 | DeviceNetObj.MACID;
//		pSendFrame->pBuffer[1] = 0x60;
//		pSendFrame->pBuffer[2] = DeviceNetObj.assign_info.master_MACID;
//		pSendFrame->pBuffer[3]= 0x80 | SVC_MONITOR_PLUSE;
//		pSendFrame->pBuffer[4] = 0x01;		//标识符对象ID=1
//		pSendFrame->pBuffer[5] = 0x00;
//		*(send_buf + 6) = IdentifierObj.device_state;
//		*(send_buf + 7) = 0;
//		*(send_buf + 8) = 0;
//		*(send_buf + 9) = 0;
//		SendData(10, send_buf);
//	}
}


/*******************************************************************************
* 函数名:	void DeviceNetReciveCenter(uint16* id, uint8 * pdata)
* 功能描述: 从缓冲中获取数据并解析
* 形  参: uint16* pID  11bitID标识, uint8 * pbuff 缓冲数据, uint8 len 数据长度
* 返回值:      BYTE  0-信息未进行处理  1-信息进行处理	
********************************************************************************/
BOOL DeviceNetReciveCenter(uint16* pID, uint8 * pbuff, uint8 len)
{   
    //判断是否为仅限组2---可以在滤波器设置屏蔽
    if( GROUP2_MSG != GET_GROUP2_MAC(*pID))  //不是仅限组2报文处理
	{       
        return FALSE;    
    }        
    
    if( ReciveFrame.complteFlag) //
    {
        return FALSE;
    }
   
    if (len <= 8) //最大长度限制
    {
         ReciveFrame.ID = *pID;   
         ReciveFrame.len = len;
        for(BYTE i= 0; i< len; i< len) //复制数据
        {
            ReciveFrame.pBuffer[i] = pbuff[i];
        }
         ReciveFrame.complteFlag = 0xff;
         return TRUE;
    }
}
/*******************************************************************************
* 函数名:	void  SendData(uint16* id, uint8 * pdata)
* 功能描述: 从缓冲中获取数据并解析
* 形  参: DefFrameData* pFrame
* 返回值:      null
********************************************************************************/
void SendData(DefFrameData* pFrame)
{
    
}
BYTE ReadData()
{
   // ReadData();
    return 0;
}
/*******************************************************************************
* 函数名:	void  StartOverTimer()
* 功能描述: 启动超时定时器
* 形  参:   null
* 返回值:   null
********************************************************************************/
void StartOverTimer()
{
    
}
/*******************************************************************************
* 函数名:	BOOL IsOverTime()
* 功能描述: 启动超时定时器
* 形  参:   null
* 返回值:   TRUE-没有超时 FALSE-超时
********************************************************************************/
BOOL IsOverTime()
{
    return true;
}
