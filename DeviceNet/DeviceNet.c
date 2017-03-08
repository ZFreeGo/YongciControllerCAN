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

void SendData(BYTE count, BYTE* buf)
{
}
BYTE ReadData()
{
   // ReadData();
    return 0;
}


//////////////////////函数申明/////////////////////////////////
void ResponseMACID(void);
void VisibleMsgService(BYTE * buf);
void CycleInquireMsgService(BYTE * buf);
void UnconVisibleMsgService(BYTE * buf);
////////////////////连接对象变量////////////////////////////////
struct DefConnectionObj  CycleInquireConnedctionObj;//循环响应
struct DefConnectionObj  VisibleConnectionObj;   //显示连接
//////////////////DeviceNet对象变量////////////////////////////
struct DefDeviceNetClass  DeviceNet_class = {2}; 
struct DefDeviceNetObj  DeviceNetObj;
struct DefIdentifierObject  IdentifierObj; 
/////////////////////////////////////////////////////////////////
BYTE  send_buf[10];//can总线发送数组 数组第一字节为ID的高8bit，第二字节的高3bit，ID的低3bit

BYTE  out_Data[8];//从站输出数组
/*******************************************************************************
* 函数名:	void DeviceNetClassService(unsigned char * buf)
* 形参  :	unsigned char * buf，接收报文数组 
* 返回值:    	无
* 功能描述:	DeviceNet分类服务函数
            DeviceNet分类只有1个属性，可选执行Get_Attribute_Single服务，响应其版本信息
*******************************************************************************/
void DeviceNetClassService(unsigned char * buf)
{  
    
	if(*(buf + 3) != SVC_GET_ATTRIBUTE_SINGLE)        //不支持的服务
	{	
		*send_buf = 0x80 | DeviceNetObj.MACID;       //组2信息+源MAC ID(从站ID) 
		*(send_buf + 1) = 0x60;                       //信息id=3，表示从站响应主站的显示请求
		*(send_buf + 2) = (*(buf + 2) & 0x7F);        //目的MAC ID(主站ID) 
		*(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;  //R/R=1表示响应，SVC_ERROR_RESPONSE，错误响应服务代码
		*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;    //ERR_SERVICE_NOT_SUPPORT ,不支持的服务
		*(send_buf + 5) = ERR_RES_INAVAIL;            //ERR_RES_INAVAIL ,对象执行服务的资源不可用(附加错误代码)
		//发送
		SendData(6, send_buf);                    //发送错误响应报文
		return ;
	}
	if(*(buf + 5) != 1)                               //不存在的属性
	{	
		*send_buf = 0x80 | DeviceNetObj.MACID;
		*(send_buf + 1) = 0x60;
		*(send_buf + 2) = (*(buf + 2) & 0x7F);
		*(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
		*(send_buf + 4) = ERR_ID_INAVAIL;             //ERR_ID_INAVAIL ,不存在指定的类/实例/属性ID(附加错误代码)
		*(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;     //ERR_NO_ADDITIONAL_DESC ,无附加描述代码
		//发送
		SendData(6, send_buf);                    //发送错误响应报文
		return ;
	}
	//执行显示请求
	*send_buf = 0x80 | DeviceNetObj.MACID;           //组2信息+源MAC ID(从站ID) 
	*(send_buf + 1) = 0x60;                           //信息id=3，表示从站响应主站的显示请求
	*(send_buf + 2) = (*(buf + 2) & 0x7F);            //目的MAC ID(主站ID) 
	*(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;//R/R=1表示响应，VC_Get_Attribute_Single服务代码
	*(send_buf + 4) = DeviceNet_class.version;
	*(send_buf + 5) = DeviceNet_class.version >> 8;   //类的版本信息
	//发送
	SendData(6, send_buf);                        //发送显示信息的响应报文
}
/******************************************************************************** 
*函数名 : void DeviceNetObjService(unsigned char * buf)
*形  参 : unsigned char * buf，接收报文数组 
*返回值 :  	无
*功能描述: DeviceNet对象服务函数
********************************************************************************/
void DeviceNetObjService(unsigned char * buf)
{
   
    switch( *(buf + 3))
    {
    case (SVC_GET_ATTRIBUTE_SINGLE):         //获取单个属性服务
	{        
        switch(*(buf + 5)) //实例ID
        {
            case DEVICENET_OBJ_MACID:
            {
                *send_buf = 0x80 | DeviceNetObj.MACID;   //组2信息+源MAC ID(从站ID) 
                *(send_buf + 1) = 0x60;                   //信息id=3，表示从站响应主站的显示请求
                *(send_buf + 2) = (*(buf + 2) & 0x7F);    //目的MAC ID(主站ID) 
                *(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;//R/R=1表示响应，SVC_GET_ATTRIBUTE_SINGLE服务代码
                *(send_buf + 4) = DeviceNetObj.MACID;    //源MAC ID(从站ID) 
                //发送
                SendData(5, send_buf);                //发送显示信息的响应报文
                return ;             
            }
            case  DEVICENET_OBJ_BAUD:
            {
                *send_buf = 0x80 | DeviceNetObj.MACID;
                *(send_buf + 1) = 0x60;
                *(send_buf + 2) = (*(buf + 2) & 0x7F); 
                *(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
                *(send_buf + 4) = DeviceNetObj.baudrate;//从站波特率
                //发送
                SendData(5, send_buf);
                return;
            }
            case DEVICENET_OBJ_MASTERID:
            {
                *send_buf = 0x80 | DeviceNetObj.MACID;
                *(send_buf + 1) = 0x60;
                *(send_buf + 2) = (*(buf + 2) & 0x7F); 
                *(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
                *(send_buf + 4) = DeviceNetObj.assign_info.select;       //分配选择
                *(send_buf + 5) = DeviceNetObj.assign_info.master_MACID; //主站MAC ID 
                //发送
                SendData(6, send_buf);
                return ;
            }
            default:
            {
                *send_buf = 0x80 | DeviceNetObj.MACID;      //组2信息+源MAC ID(从站ID) 
                *(send_buf + 1) = 0x60;                      //信息id=3，表示从站响应主站的显示请求
                *(send_buf + 2) = (*(buf + 2) & 0x7F);       //目的MAC ID(主站ID) 
                *(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE; //R/R=1表示响应，SVC_ERROR_RESPONSE，错误响应服务代码
                *(send_buf + 4) = ERR_ID_INAVAIL;            //ERR_ID_INAVAIL ,不存在指定的类/实例/属性ID(附加错误代码)
                *(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;    //ERR_NO_ADDITIONAL_DESC ,无附加描述代码
                //发送
                SendData(6, send_buf);                   //发送错误响应报文
                return ;
            }        
        }  
        break;
	}
	case ( SVC_SET_ATTRIBUTE_SINGLE):      //设置单个属性服务
	{
        switch (*(buf  + 5))
        {
            case DEVICENET_OBJ_MACID:
            case DEVICENET_OBJ_BAUD:
            case DEVICENET_OBJ_MASTERID://不支持设置节点地址、波特率、分配选择和主站MAC ID 
            {
                *send_buf = 0x80 | DeviceNetObj.MACID;
                *(send_buf + 1) = 0x60;
                *(send_buf + 2) = (*(buf + 2) & 0x7F);
                *(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
                *(send_buf + 4) = ERR_PROPERTY_NOT_SET;
                *(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;
                //发送
                SendData(6, send_buf);
                return ;              
            }
            default:
            {
               *send_buf = 0x80 | DeviceNetObj.MACID;
                *(send_buf + 1) = 0x60;
                *(send_buf + 2) = (*(buf + 2) & 0x7F);
                *(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
                *(send_buf + 4) = ERR_ID_INAVAIL;
                *(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;
                //发送
                SendData(6, send_buf);
                return ;			           
            }
        }		
	}
	case (SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET)://建立连接服务代码
    case (SVC_RELEASE_GROUP2_IDENTIFIER_SET):  //释放组2连接服务
	{	
		UnconVisibleMsgService(buf);                            //非连接显式信息服务，
        break;
	}	
	default:
	{
		*send_buf = 0x80 | DeviceNetObj.MACID;
		*(send_buf + 1) = 0x60;
		*(send_buf + 2) = (*(buf + 2) & 0x7F);
		*(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
		*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;
		*(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;
		//发送
		SendData(6, send_buf);
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
void ConnectionClassService(BYTE  *buf)
{
	*send_buf = 0x80 | DeviceNetObj.MACID;              //组2信息+源MAC ID(从站ID) 
	*(send_buf + 1) = 0x60;                              //信息id=3，表示从站响应主站的显示请求
	*(send_buf + 2) = (*(buf + 2) & 0x7F);               //目的MAC ID(主站ID) 
	*(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;         //R/R=1表示响应，SVC_ERROR_RESPONSE，错误响应服务代码
	*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;           //ERR_SERVICE_NOT_SUPPORT ,不支持的服务
	*(send_buf + 5) = 0x01;                              //附加错误代码
	SendData(6, send_buf);//发送
	return ;
}
/*******************************************************************************
* 函数名:	void VisibleConnectObjService(BYTE  *buf)
* 形参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	显式信息连接服务函数
*******************************************************************************/
void VisibleConnectObjService(BYTE  *buf)
{
	*send_buf = 0x80 | DeviceNetObj.MACID;    // 报文组2(0x80)+源MAC ID(从站ID) 
	*(send_buf + 1) = 0x60;                    // 报文组2响应报文(0x60)，信息id=3，表示从站响应主站的显示请求
	*(send_buf + 2) = (*(buf + 2) & 0x7F);     // 目的MAC ID(主站ID) 
	*(send_buf + 3) = 0x80 | *(buf + 3);       // R/R=1表示响应，*(buf + 3)是接收报文中的服务代码
    
    switch(*(send_buf + 3))
    {
        case SVC_SET_ATTRIBUTE_SINGLE://设置服务，#define SVC_SET_ATTRIBUTE_SINGLE	0x10
        {
            *(send_buf + 3) = 0x80 | SVC_SET_ATTRIBUTE_SINGLE;
            *(send_buf + 4) = *(buf + 7);
            *(send_buf + 5) = *(buf + 8);
            break;       
        }
        case SVC_GET_ATTRIBUTE_SINGLE://获取服务 
        {
            *(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
            if(*(buf + 6) == 7)                   //属性ID为7，表示通过本连接发送的最大字节数为8个
            {	 			
                *(send_buf + 4) = 0x08;
                *(send_buf + 5) = 0;
            }
            else if(*(buf + 6) == 8)              //属性ID为8，表示通过本连接接收的最大字节数为8个
            { 
                *(send_buf + 4) = 0x08;
                *(send_buf + 5) = 0;
            }
            break;
        }
        default:
        {
            *(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
            *(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;
            *(send_buf + 5) = 0x01;
            break;
        }            
    }  	
	//发送
	SendData(6, send_buf);
}
/********************************************************************************
* 函数名:	void CycInquireConnectObjService(BYTE  *buf)
* 形  参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	轮询信息连接实例服务函数
********************************************************************************/
void CycInquireConnectObjService(BYTE  *buf)
{
	*send_buf = 0x80 | DeviceNetObj.MACID;
	*(send_buf + 1) = 0x60;
	*(send_buf + 2) = (*(buf + 2) & 0x7F);
	*(send_buf + 3) = 0x80 | *(buf + 3);
	if(*(buf + 3) == SVC_SET_ATTRIBUTE_SINGLE)
	{
		*(send_buf + 3) = 0x80 | SVC_SET_ATTRIBUTE_SINGLE;

			*(send_buf + 4) = *(buf + 7);
			*(send_buf + 5) = *(buf + 8);

		CycleInquireConnedctionObj.state = 4;
	}
	else if(*(buf + 3) == SVC_GET_ATTRIBUTE_SINGLE)
	{
		*(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
		if(*(buf + 6) == 7)//属性ID为7，表示通过本连接发送的最大字节数
		{		
			*(send_buf + 4) = 0x08;
			*(send_buf + 5) = 0;
		}
		else if(*(buf + 6) == 8)//属性ID为8，表示通过本连接接收的最大字节数
		{
			*(send_buf + 4) = 0x08;
			*(send_buf + 5) = 0;
		}
	}
	else
	{
		*(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
		*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;
		*(send_buf + 5) = 0x01;
	}
	SendData(6, send_buf);
}

/*******************************************************************************
* 函数名:	void IdentifierClassService(BYTE * buf)
* 形参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	标识符类服务, 不支持任何服务，错误响应
********************************************************************************/
void IdentifierClassService(BYTE * buf)
{
	*send_buf = 0x80 | DeviceNetObj.MACID;
	*(send_buf + 1) = 0x60;
	*(send_buf + 2) = (*(buf + 2) & 0x7F);
	*(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
	*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;
	*(send_buf + 5) = 0x01;
	SendData(6, send_buf);
	return ;
}
/*******************************************************************************
* 函数名:	void IdentifierObjService(BYTE * buf)
* 形  参:	BYTE  *buf，接收报文数组 
* 返回值:    	无
* 功能描述:	标识符对象服务函数，响应主站有关标示符的请求
********************************************************************************/   
void IdentifierObjService(BYTE * buf)
{
	if(*(buf + 3) == SVC_GET_ATTRIBUTE_SINGLE)
	{
		*send_buf = 0x80 | DeviceNetObj.MACID;
		*(send_buf + 1) = 0x60;
		*(send_buf + 2) = *(buf + 2) & 0x7F;  
		*(send_buf + 3) = 0x80 | SVC_GET_ATTRIBUTE_SINGLE;
        
        switch(*(buf + 6))
        {
            case IDENTIFIER_OBJ_SUPPLIERID:  //属性ID1，表示用数字标识各供应商
            {
                *(send_buf + 4) = IdentifierObj.providerID;
			    *(send_buf + 5) = IdentifierObj.providerID >> 8;
			    SendData(6, send_buf);
                break;
            }
            case IDENTIFIER_OBJ_TYPE://属性ID2，表示产品通用类型说明
            {
                *(send_buf + 4) = IdentifierObj.device_type;
			    *(send_buf + 5) = IdentifierObj.device_type >> 8;
			    SendData(6, send_buf);
                break;
            }
            case IDENTIFIER_OBJ_CODE://属性ID3，表示产品代码
            {
                *(send_buf + 4) = IdentifierObj.product_code;
			    *(send_buf + 5) = IdentifierObj.product_code >> 8;
			    SendData(6, send_buf);
                break;
            }
            case IDENTIFIER_OBJ_VERSION://属性ID4，表示产品版本
            {
                *(send_buf + 4) = IdentifierObj.version.major_ver;
			    *(send_buf + 5) = IdentifierObj.version.minor_ver;
			    SendData(6, send_buf);
                break;
            }
             case IDENTIFIER_OBJ_STATUES://属性ID5，表示设备状态概括
             {
                 *(send_buf + 4) = IdentifierObj.device_state;
			     *(send_buf + 5) = IdentifierObj.device_state >> 8;
			     SendData(6, send_buf);
                 break;
             }
            case IDENTIFIER_OBJ_SERIALNUM://属性ID6，表示设备序列号
            {
             	*(send_buf + 4) = IdentifierObj.serialID;
			    *(send_buf + 5) = IdentifierObj.serialID >> 8;
                *(send_buf + 6) = IdentifierObj.serialID >> 16;
                *(send_buf + 7) = IdentifierObj.serialID >> 24;
                SendData(8, send_buf);
                break;
            }
            case IDENTIFIER_OBJ_NAME://属性ID7，表示产品名称
            {
                *(send_buf + 4) = IdentifierObj.product_name.length;
                *(send_buf + 5) = IdentifierObj.product_name.ucdata[0];
                *(send_buf + 6) = IdentifierObj.product_name.ucdata[1];
                *(send_buf + 7) = IdentifierObj.product_name.ucdata[2];
                *(send_buf + 8) = IdentifierObj.product_name.ucdata[3];
                *(send_buf + 9) = IdentifierObj.product_name.ucdata[4];
                SendData(10, send_buf);
                break;
            }
            default://不支持的服务
            {
                *send_buf = 0x80 | DeviceNetObj.MACID;
                *(send_buf + 1) = 0x60;
                *(send_buf + 2) = *(buf + 2) & 0x7F;//DeviceNetObj.MACID;
                *(send_buf + 3) = 0x80 | SVC_ERROR_RESPONSE;
                *(send_buf + 4) = ERR_PROPERTY_NOT_SET;
                *(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;
                SendData(6, send_buf);
                break;
            }
        }
    }

}
/********************************************************************************
* 函数名:	void RountineClassObjService(BYTE * buf)
* 形参:	无
* 返回值:    	无
* 功能描述:	信息路由器服务，不支持任何服务，错误响应
********************************************************************************/  
void RountineClassObjService(BYTE * buf)
{
	*send_buf = 0x80 | DeviceNetObj.MACID;
	*(send_buf + 1) = 0x60;
	*(send_buf + 2) = (*(buf + 2) & 0x7F);
	*(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
	*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;
	*(send_buf + 5) = 0x01;
	SendData(6, send_buf);
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
** 函数名:	void CANFrameFilter(BYTE * buf)
** 形参:	BYTE * buf，接收的报文数组 
** 返回值:    	无
** 功能描述:	CAN信息过滤器函数，提取帧ID1和帧ID2中的信息，
                仅限组2设备，并对信息进行分类处理
*******************************************************************************/
void CANFrameFilter(BYTE * buf)
{
	unsigned char t0, t1;
	t0 = *buf;
	t1 = *(buf + 1);
	
	if(((t0 & 0xC0) == 0x80) && ((t0 & 0x3F) == DeviceNetObj.MACID))  //仅限组2设备
	{	        
        switch(t1 &= 0xE0)
        {
            case 0xE0:  //重复MAC ID检查信息
            {            
				ResponseMACID();                   //重复MACID检查响应函数
                return; //MACID不匹配,丢弃
            }
            case 0xC0: //1100 0000：非连接显示请求信息，预定义主从连接
            {                
				UnconVisibleMsgService(buf);    //非连接显式信息服务 
                return;
            }
            case 0xA0: //主站I/O轮询命令/状态变化/循环信息
            {     
                CycleInquireMsgService(buf);     // I/O轮询信息服务
                return ;    
            }
            case 0x80:  //主站显式请求信息
            {               
				VisibleMsgService(buf);        //显式信息服务
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
void ResponseMACID(void)
{
	*send_buf = 0x80 | DeviceNetObj.MACID;         //组2信息+源MAC ID(从站ID) 
	*(send_buf + 1) = 0xE0;                         //重复MACID检查
	*(send_buf + 2) = 0x80;	                        //请求/响应标志=1，表示响应，端口号0
	*(send_buf + 3) = IdentifierObj.providerID;	//制造商ID低字节
	*(send_buf + 4) = IdentifierObj.providerID >> 8;	//制造商ID高字节
	*(send_buf + 5) = IdentifierObj.serialID;	                    //序列号低字节
	*(send_buf + 6) = IdentifierObj.serialID >> 8;                //序列号中间字节1
	*(send_buf + 7) = IdentifierObj.serialID >>16;          //序列号中间字节2
	*(send_buf + 8) = IdentifierObj.serialID >>24;	//序列号高字节
	SendData(9, send_buf);                      //发送报文
}
/*******************************************************************************
* 函数名:	unsigned char CheckMACID(void)
* 功能描述:	主动检查重复MACID函数
* 形参:	无
* 返回值:      1    网络上有和自己重复的地址
                0    网络上没有和自己重复的地址   	
*******************************************************************************/
unsigned char CheckMACID(void)
{
	unsigned char num_retries = 2;        //检查的次数为2次
	unsigned long t;

	
	//InterruptEnReg = 0;      //关SJA1000接收中断

	
	if(ReadData())       //SJA1000缓冲区中有信息，检查是否为重复MACID请求/响应
	  {	
//		if(((*(receive_buf + 1) & 0xC0) == 0x80) && ((*(receive_buf + 2) & 0xE0) == 0xE0)
//                && ((*(receive_buf + 1) & 0x3F) == DeviceNetObj.MACID))
//		{	             //收到重复MACID请求/响应，转到通讯故障状态。
//			return 1;	//检查到重复MACID，返回1
//		}
	  }
	while(num_retries)
	{
		//发送重复MACID信息,send_buf[11];
		*send_buf = 0x80 | DeviceNetObj.MACID;             //=0xBF:1011  1111，报文组2，目标地址63
		*(send_buf + 1) = 0xE0;                             //重复 MAC ID 检查信息
		*(send_buf + 2) = 0x00;	                            //端口号0
		*(send_buf + 3) = IdentifierObj.providerID;	    //制造商ID低字节
		*(send_buf + 4) = IdentifierObj.providerID / 256;	//制造商ID高字节
		t = IdentifierObj.serialID;
		*(send_buf + 5) = t;	  //序列号低字节
		t >>= 8;
		*(send_buf + 6) = t;
		t >>= 8;
		*(send_buf + 7) = t;
		t >>= 8;
		*(send_buf + 8) = t;	  //序列号高字节
		SendData(9, send_buf);//发重复 MAC ID 检查信息
		
		//start_time(200);       //启动延时定时:(200)*5MS=1000ms
        //WDR1=1;  
        //WDR2=1; //喂狗
		//检查重复MACID响应
//		while(!query_time_event(3))//定时没有到的时候，重复读取接收缓冲区，检查是否为重复MACID请求/响应
		{
			if(ReadData())     //如果SJA1000缓冲区中有信息
			{	
//				if(((*(receive_buf + 1) & 0xC0) == 0x80) && ((*(receive_buf + 2) & 0xE0) == 0xE0) && ((*(receive_buf + 1) & 0x3F) == DeviceNetObj.MACID))
//				{	//收到重复MACID请求/响应，转到通讯故障状态。
//					return 1;	//检查到重复MACID
//				}
			}
//			if(StatusReg & BS_Bit)
//			{	//SJA1000离线
//				return 1;	//故障
//			}
		}
		num_retries--;
//        WDR1=1;  
//        WDR2=1; //喂狗
	}
	//InterruptEnReg = RIE_Bit;		//SJA1000中断使能
	//SJA1000_CS = 1;

	return 0;	//没有重复地址
}
/********************************************************************************
** 函数名:	void CheckAllocateCode(BYTE * buf)
** 形参:	BYTE * buf，接收的报文数组 
** 返回值:      unsigned char 0-溢出 非零为检测通过 	
** 功能描述:j检测非连接显式信息服务设置连接代码
********************************************************************************/
unsigned char CheckAllocateCode(BYTE * buf)
{  
    //如果已分配主站,则检查是否来自同一主站
    if((IdentifierObj.device_state & 0x01) && (*(buf + 7) != DeviceNetObj.assign_info.master_MACID))	//验证主站
    {	//不是来自当前主站，错误响应
        *send_buf = 0x80 | DeviceNetObj.MACID;
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
        *(send_buf + 4) = ERR_OBJECT_STATE_INFLICT;
        *(send_buf + 5) = 0x01;
        SendData(6, send_buf);  //发送报文
        return 0;	
    }
    //仅限组2无连接信息，这个报文指向DeviceNet对象，其类ID为3。在每个DeviceNet的物理连接中只有一个DeviceNet类的实例，因此实例ID为1
    if(*(buf + 4) != 3 || *(buf + 5) != 1)	//验证类ID和实例ID
    {   //验证类ID和实例ID错误，错误响应
        *send_buf = 0x80 | DeviceNetObj.MACID;
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);//#define SVC_ERROR_RESPONSE		0x14
        *(send_buf + 4) = ERR_PROPERTY_VALUE_INAVAIL;
        *(send_buf + 5) = ERR_NO_ADDITIONAL_DESC;
        SendData(6, send_buf);//发送报文
        return 0;	
    }
    if(*(buf + 6) == 0)	//验证分配选择字节
    {//配置字节为零，主站没有配置从站，错误响应
        *send_buf = (0x80 | DeviceNetObj.MACID);
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
        *(send_buf + 4) = (0x80 | ERR_PROPERTY_VALUE_INAVAIL);
        *(send_buf + 5) = 0x02;
        SendData(6, send_buf);//发送报文
        return 0;
    }
    if(*(buf + 6) & ~(CYC_INQUIRE | VISIBLE_MSG | 0x04))  
    {//如果不是轮询配置、显示连接、位选通，错误响应
        *send_buf = (0x80 | DeviceNetObj.MACID);
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
        *(send_buf + 4) = (0x80 | ERR_RES_INAVAIL);
        *(send_buf + 5) = 0x02;
        //发送
        SendData(6, send_buf);
        return 0;
    }
    
    return 0xff;
}
    
/********************************************************************************
** 函数名:	void CheckReleaseCode(BYTE * buf)
** 形参:	BYTE * buf，接收的报文数组 
** 返回值:      unsigned char 0-溢出 非零为检测通过 	
** 功能描述:检测非连接显式信息服务释放连接代码
********************************************************************************/
unsigned char CheckReleaseCode(BYTE * buf)
{  
    if((*(buf + 6) == 0) || ((*(buf + 6) & 0x3F) != 0))   //如果配置字节为0
    {	
        *send_buf = (0x80 | DeviceNetObj.MACID);
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_RELEASE_GROUP2_IDENTIFIER_SET);
        SendData(4, send_buf);
        return 0;
    }
    if(*(buf + 6) & ~(CYC_INQUIRE | VISIBLE_MSG | 0x04))//不支持的连接，错误响应
    {	
        *send_buf = (0x80 | DeviceNetObj.MACID);
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
        *(send_buf + 4) = ERR_RES_INAVAIL;
        *(send_buf + 5) = 0x02;
        SendData(6, send_buf);
        return 0;
    }
    if(*(buf + 6) & DeviceNetObj.assign_info.select)//连接不存在，错误响应
    {	
        *send_buf = 0x80 | DeviceNetObj.MACID;
        *(send_buf + 1) = 0x60;
        *(send_buf + 2) = (*(buf + 2) & 0x7F);
        *(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
        *(send_buf + 4) = ERR_EXISTED_MODE;
        *(send_buf + 5) = 0x02;
        SendData(6, send_buf);
        return 0;
    }
    return 0xff;
}


/********************************************************************************
** 函数名:	void UnconVisibleMsgService(BYTE * buf)
** 形参:	BYTE * buf，接收的报文数组 
** 返回值:      无 	
** 功能描述:	非连接显式信息服务函数，主站用该报文命令从站配置连接
********************************************************************************/
void UnconVisibleMsgService(BYTE * buf)
{

    if(*(buf + 3) == SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET)//*(buf + 3)是收到的服务代码
	                                                          //如果是建立连接服务代码 
                                                              
	{
        if (!CheckAllocateCode(buf))
        {          
            //检测未通过返回
            return;
        }        
        
		DeviceNetObj.assign_info.master_MACID = *(buf + 7);  //主站告诉从站：主站的地址
		DeviceNetObj.assign_info.select |= *(buf + 6);       //配置字节
        

		if(*(buf + 6) & CYC_INQUIRE)                          //分配I/O轮询连接
		{	
			InitCycleInquireConnectionObj();                       //I/O轮询连接配置函数
			CycleInquireConnedctionObj.produced_connection_id = 0x7800 | (DeviceNetObj.MACID << 5);//	produced_connection_id
			CycleInquireConnedctionObj.consumed_connection_id = 0x80A0 | (DeviceNetObj.MACID << 8);// consumed_connection_id
 	        //成功执行响应
			*send_buf = 0x80 | DeviceNetObj.MACID;//组2信息+源MAC ID(从站ID) 
			*(send_buf + 1) = 0x60;                // 报文组2响应报文(0x60)，信息id=3 
			*(send_buf + 2) = *(buf + 2) & 0x7F;   // 目的MAC ID(主站ID) 
			*(send_buf + 3) = (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
			*(send_buf + 4) = 0;	               //信息体格式0,8/8：Class ID = 8 位整数，Instance ID = 8 位整数
			SendData(5, send_buf);             //发送报文
			return ;
		}
		if(*(buf + 6) & VISIBLE_MSG)
		{	
			InitVisibleConnectionObj();//分配显式信息连接  
			VisibleConnectionObj.produced_connection_id = 0x7800 | (DeviceNetObj.MACID << 5);
			VisibleConnectionObj.consumed_connection_id = 0x8080 | (DeviceNetObj.MACID << 8);
			//成功执行响应
			*send_buf = 0x80 | DeviceNetObj.MACID;
			*(send_buf + 1) = 0x60;
			*(send_buf + 2) = *(buf + 2) & 0x7F;
			*(send_buf + 3) = (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
			*(send_buf + 4) = 0;	//信息体格式0,8/8：类Class ID = 8 位整数，实例Instance ID = 8 位整数
			//发送
			SendData(5, send_buf);			
		}
		if(*(buf + 6) & 0x04) //分配位选通连接
		{	
		
			*send_buf = 0x80 | DeviceNetObj.MACID;
			*(send_buf + 1) = 0x60;
			*(send_buf + 2) = *(buf + 2) & 0x7F;
			*(send_buf + 3) = (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
			*(send_buf + 4) = 0;	//信息体格式0,8/8
			SendData(5, send_buf);
			return ;
		}
		IdentifierObj.device_state |= 0x01;	//设备已和主站连接
		return ;
	}
	else if(*(buf + 3) == SVC_RELEASE_GROUP2_IDENTIFIER_SET)   //释放连接服务
	{
		if(!CheckReleaseCode(buf))
        {
            return;
        }
		//释放连接
		DeviceNetObj.assign_info.select = *(buf + 6);
		if(*(buf + 6) == 0)
			IdentifierObj.device_state &= ~0x01;	//指示已和主站断开所有连接
		//执行成功响应
		*send_buf = (0x80 | DeviceNetObj.MACID);
		*(send_buf + 1) = 0x60;
		*(send_buf + 2) = *(buf + 2) & 0x7F;
		*(send_buf + 3) = (0x80 | SVC_AllOCATE_MASTER_SlAVE_CONNECTION_SET);
		*(send_buf + 4) = 0;	//信息体格式0,8/8
		SendData(5, send_buf);
	}
	else
	{	//组2非连接显式信息服务不支持，错误响应
		*send_buf = 0x80 | DeviceNetObj.MACID;
		*(send_buf + 1) = 0x60;
		*(send_buf + 2) = (*(buf + 2) & 0x7F);
		*(send_buf + 3) = (0x80 | SVC_ERROR_RESPONSE);
		*(send_buf + 4) = ERR_SERVICE_NOT_SUPPORT;
		*(send_buf + 5) = 0x02;
		SendData(6, send_buf);
		return ;
	}
}
/*********************************************************************************
** 函数名:	void VisibleMsgService(BYTE * buf)
** 形参:	BYTE * buf，接收的报文数组 
** 返回值:      无 	
** 功能描述:	显式信息服务函数，执行主站的显示请求响应
*********************************************************************************/
void VisibleMsgService(BYTE * buf)
{
	BYTE class, obj;

	class = *(buf + 4); //类ID
	obj = *(buf + 5);   //实例ID

	//信息路由
	if(!(DeviceNetObj.assign_info.select & VISIBLE_MSG))	//没有建立显式信息连接 
		return ;
    
    switch(class)
    {
        case 0x01: //标识符对象
        {   
            if(obj == 0)	    //类服务
            {
                IdentifierClassService(buf);
            }
            else if(obj == 1)	//实例1服务	
            {
                IdentifierObjService(buf);
            }              
            break;
        }
        case 0x02: //信息路由器对象
        {
            RountineClassObjService(buf);
            break;
        }
        case 0x03://DeviceNet对象
        {
            if(obj == 0)	    //类服务
            {
                DeviceNetClassService(buf);
                return ;
            }
            else if(obj == 1)	//实例1服务
            {
                DeviceNetObjService(buf);
                return ;
            }
            break;
        }
        case 0x05:	//连接对象
        {
            if(obj == 0)	    //类服务
            {
                ConnectionClassService(buf); 
                return ;
            }
            else if(obj == 1)	//显式信息连接
            {
                VisibleConnectObjService(buf); 
                return ;
            }
            else if(obj == 2)	//I/O轮询连接
            {
                CycInquireConnectObjService(buf);
                return ;
            }
            break;
        }   
    } 
}
/********************************************************************************
** 函数名:	void  CycleInquireMsgService(BYTE * buf)
** 功能描述:	I/O轮询信息服务函数，在主站和从站之间传输数据
** 形参:	BYTE * buf，接收的报文数组 
** 返回值:      无 	
*********************************************************************************/
void  CycleInquireMsgService(BYTE * buf)
{
	
     out_Data[0] = *(buf + 2);
     out_Data[1] = *(buf + 3);
     out_Data[2] = *(buf + 4);
     out_Data[3] = *(buf + 5);          //保存主站设置的数据

    if(CycleInquireConnedctionObj.state != 4)	//轮询I/O连接没建立
		return ;

	*send_buf = 0x78 | DeviceNetObj.MACID >> 3;
	*(send_buf + 1) = DeviceNetObj.MACID << 5;  //发送数组的帧ID
//////将ADC采集结果拷贝到can的发送数组///////////////////////
//	*(send_buf + 2) = ADC_Data[0];
//	*(send_buf + 3) = ADC_Data[1];//ADC0通道采集结果
//	*(send_buf + 4) = ADC_Data[2];
//	*(send_buf + 5) = ADC_Data[3];//ADC1通道采集结果
//	*(send_buf + 6) = ADC_Data[4];
//	*(send_buf + 7) = ADC_Data[5];//ADC2通道采集结果
//	*(send_buf + 8) = ADC_Data[6];
//	*(send_buf + 9) = ADC_Data[7];//ADC3通道采集结果
//	SendData(10, send_buf);
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
//		*send_buf = 0x80 | DeviceNetObj.MACID;
//		*(send_buf + 1) = 0x60;
//		*(send_buf + 2) = DeviceNetObj.assign_info.master_MACID;
//		*(send_buf + 3) = 0x80 | SVC_MONITOR_PLUSE;
//		*(send_buf + 4) = 0x01;		//标识符对象ID=1
//		*(send_buf + 5) = 0x00;
//		*(send_buf + 6) = IdentifierObj.device_state;
//		*(send_buf + 7) = 0;
//		*(send_buf + 8) = 0;
//		*(send_buf + 9) = 0;
//		SendData(10, send_buf);
//	}
}


