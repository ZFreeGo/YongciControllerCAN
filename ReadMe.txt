2017/3/12
1.修改完善连接建立
2.重新编排文件结构
8:46 修改完善释放连接
10:12 按测试步骤完成，建立连接，获取属性，IO报文，释放连接，重新建立连接

2017/3/12
  1. C1RX0CONbits.RXFUL = 1; //解决未清0溢出问题
2.  pReciveFrame->completeFlag 未清空错误
2017/3/10
1.添加CAN驱动与DeviceNet结合部分
2.重新考虑波特率设置，调节传输设置。

2017/3/9
1.设置gun99
2.设置优化等级1
3.解决所有警告


2017/3/8
1.添加超时定时器Timer3
2.完善重复MAC检测功能

2017/3/7
21:00 修改风格，进一步理解程序。
2017/3/6
7:19 ： 开始调试CAN驱动,通过回环测试。