

/********************************************接口函数说明**************************************************
版本：TYRM-9000L-268动态库 
日期:20141125
***********************************************************************************************************/



//===========================================================================
#ifndef _TYRM_9000L_268_H
#define _TYRM_9000L_268_H
//===========================================================================
#ifdef BUILD_DLL
#define DLL_EXP __declspec(dllexport)
#else
#define DLL_EXP __declspec(dllimport)
#endif

//===========================================================================
#ifdef __cplusplus
extern "C" {
#endif


  //*********************************************串口操作***********************************
  //打开端口
  //pszPort:如"COM1"
  //pszBuildCommDCB: 如"COM1:9600,N,8,1"
  DLL_EXP int PASCAL PortOpen (PBYTE pszPort, PBYTE pszBuildCommDCB);
  DLL_EXP int PASCAL PCSCOpen();


  //关闭串口
  DLL_EXP int PASCAL PortClose(void);

  // 计算16位CRC16值 逆序
  //===========================================================================
  DLL_EXP unsigned int PASCAL Cal_CRC16_Byte(unsigned char * ptr, unsigned int len);
 
  //**************************************发送指令通用函数(磁卡都用此进行操作,除了命令参数，没有其它能数)
//  无数据包指令接口，主要包括磁卡指令
//  addr_mac: 读卡器的地址
//  command: 命令字
//  para: 命令参数
//          (命令字和命令参数具体见<<TYRM-3000通讯协议>>)
//  pszReadData: 从串口收到的数据存放的地址指针(接收数据缓冲区)
//  nReadLen: 数组pszReadData的长度(接收数据缓冲区大小)
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长，一般设置为0
  DLL_EXP int PASCAL SendData (char addr_mac,char command,char para, PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  
//设置设备地址
//  addr_mac: 读卡器的地址
//  command:命令字
//  para：命令参数
//  addr: 待待设置的设备地址
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL SetCardReaderAddr (char addr_mac,char command,char para, char addr,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  允许进卡
//  addr_mac: 读卡器的地址
//  iTime：允许进卡时间，以S为单位
//  nTimeWaite: 串口接收数据等待接收的预计超时时长
  DLL_EXP int PASCAL InsertCardEnable(char addr_mac,int iTime,int nTimeWaite);

  //****************************************CPU卡SAM卡******************************************************
//IC卡上下电
//  addr_mac: 读卡器的地址
//  command:命令字
//  para：命令参数
//  mid：IC卡座编号
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_CPUSAMPowerOnOrDown (char addr_mac,char command,char para, char mid ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  

//IC卡发送APDU指令
//  addr_mac: 读卡器的地址
//  command:命令字
//  para：命令参数
//  mid：IC卡座编号
//  pszSendData:发送apdu指令数据区；
//  nSendLen:发送apdu指令数据字节数；
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_CPUSAMSendAPDU (char addr_mac,char command,char para, char mid, PBYTE pszSendData ,int nSendLen ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  非接触式IC卡上电复位
//  addr_mac: 读卡器的地址
//  cType:卡类型选择， ='A'为：TYPE-A Mifare pro，='B'为：TYPE-B
//  mid：IC卡座编号
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFCPUSAMPowerOn (char addr_mac, char cType ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  

//  非接触式IC卡发送APDU指令
//  addr_mac: 读卡器的地址
//  pszSendData:发送apdu指令数据区；
//  nSendLen:发送apdu指令数据字节数；
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFCPUSAMSendAPDU (char addr_mac, PBYTE pszSendData ,int nSendLen ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//读SIM卡卡号
//  addr_mac: 读卡器的地址
//  command:命令字
//  para：命令参数
//  mid：IC卡座编号
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_CPUSAMReadICCID (char addr_mac,char command,char para, char mid ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  


  //*****************************************MF1卡**********************************************************
//  MF1卡读卡模式
//nMode=0,内置读卡器读卡(默认); /nMode=1,外置读卡器读卡
DLL_EXP int PASCAL TY_MFReadMode (int nMode);


//  读卡类型设置
//  addr_mac: 读卡器的地址
//  cType:读卡类型；cType='A'： 读ISO14443-A类卡(具体参照：TYRM-3000通讯协议_V22.doc)
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFSelectCardType (char addr_mac,char cType,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  选卡并读卡
//  addr_mac: 读卡器的地址
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFSelectAndRead (char addr_mac,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  验证密码
//  addr_mac: 读卡器的地址
//  blockID：块号
//  keyType:密码类型
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFAuthentication (char addr_mac,char blockID,char keyType,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  

//  读卡的指定块
//  addr_mac: 读卡器的地址
//  blockID：块号
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFReadBlockData (char addr_mac,char blockID,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  写数据到指定块
//  addr_mac: 读卡器的地址
//  blockID：块号
//  pszSendData:待写数据，一般为16字节
//  nSendLen：待写数据长度，一般为16
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFWriteBlockData (char addr_mac,char blockID,PBYTE pszSendData ,int nSendLen,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  复位读卡器
//  addr_mac: 读卡器的地址
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFReset (char addr_mac,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  对卡的指定的值块扣值
//  addr_mac: 读卡器的地址
//  blockID：块号
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFDeduValue (char addr_mac,char blockID,PBYTE pszValue ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  对卡的指定的值块充值
//  addr_mac: 读卡器的地址
//  blockID：块号
//  pszValue: 待充值数据，4字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFAddValue (char addr_mac,char blockID,PBYTE pszValue ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  初始化指定值块的值
//  addr_mac: 读卡器的地址
//  blockID：块号
//  pszValue: 待充值数据，4字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFInitValue (char addr_mac,char blockID,PBYTE pszValue ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  将值块备份到指定的块
//  addr_mac: 读卡器的地址
//  blockID0：待备份块号
//  blockID1: blockID0数据备份到blockID1块号
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFBackUpValue (char addr_mac,char blockID0,char blockID1,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  下载密码到EEPROM
//  addr_mac: 读卡器的地址
//  FanID: 扇区号
//  keyType: 密码类型
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFDLKeyToEEPROM (char addr_mac,char FanID,char keyType,PBYTE pszKeyData,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  下载密码到FIFO并验证
//  addr_mac: 读卡器的地址
//  FanID: 扇区号
//  keyType: 密码类型
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFDLKeyToFIFO (char addr_mac,char FanID,char keyType,PBYTE pszKeyData,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  初始化卡的指定扇区和密码
//  addr_mac: 读卡器的地址
//  FanID: 扇区号
//  pszOldKeyA：原密码A
//  pszOldKeyB：原密码B
//  pszNewKeyA：新密码A
//  blockCTL0：块0
//  blockCTL1：块1
//  blockCTL2：块2
//  pszNewKeyB：新密码B
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFInitFanAndKey (char addr_mac,char FanID,PBYTE pszOldKeyA,PBYTE pszOldKeyB,PBYTE pszNewKeyA,char blockCTL0,char blockCTL1,char blockCTL2,PBYTE pszNewKeyB,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  读取卡号、验证密码、读卡的指定块的数据(批处理读块)
//  addr_mac: 读卡器的地址
//  blockID: 块号
//  keyType: 密码类型
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFReadBockBat (char addr_mac,char blockID,char keyType ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  读取卡号、验证密码、写数据到卡的指定块(批处理写块)
//  addr_mac: 读卡器的地址
//  addr: 待读数据在卡中的首地址
//  len: 待读数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFWriteBlockBat (char addr_mac,char blockID,char keyType ,PBYTE pszSendData,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  读取卡号、验证密码、扣值(批处理扣值)
//  addr_mac: 读卡器的地址
//  blockID: 块号
//  keyType: 密码类型
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFDeduValueBat (char addr_mac,char blockID,char keyType,PBYTE pszValue,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  读取卡号、验证密码、充值(批处理充值)
//  addr_mac: 读卡器的地址
//  blockID: 块号，扇区号+数据块号
//  pszValue: 充值，4字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFAddValueBat (char addr_mac,char blockID,PBYTE pszValue,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

  
//  读取卡号、验证密码、预置指定值块的值(批处理初始化)
//  addr_mac: 读卡器的地址
//  blockID: 块号，扇区号+数据块号
//  pszValue: 充值，4字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFInitBat (char addr_mac,char blockID,PBYTE pszValue ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

  
//  命令卡片进入休眠状态
//  addr_mac: 读卡器的地址
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFCardInSleep (char addr_mac,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//打开/关闭天线的射频磁场//  打开/关闭天线的射频磁场
//  addr_mac: 读卡器的地址
//  CtrlCode：控制字，0x30表示打开；0x31表示关闭
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFAntennaCtrl (char addr_mac,char CtrlCode,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

//蜂鸣器鸣叫和LED闪烁
  //  addr_mac: 读卡器的地址
  //  module: 模块
  //  pszReadData: 从串口收到的数据存放的地址指针
  //  nReadLen: 数组pszReadData的长度
  //  dwRead: 从串口收到的数据的长度
  //  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_MFLEDAndBuzzer (char addr_mac,char module,PBYTE pszValue ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

  //***************************************************4442和4428**************************************************
//4442和4428下发不带参数指令，如上下电等
//  addr_mac: 读卡器的地址
//  command: 命令字
//  para: 命令参数 
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_44XXSendData ( char addr_mac,char command,char para,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

  
//  4442卡读主存储器
//  addr_mac: 读卡器的地址
//  addr: 要读的数据在卡中的首地址
//  len: 要读取的数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4442ReadMainMemory(char addr_mac,char addr,char len ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  

//  4442卡校验密钥
//  addr_mac: 读卡器的地址
//  pszKeyData: 卡密钥，3字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4442AuthenticationKey(char addr_mac,PBYTE pszKeyData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4442卡写主存储器
//  addr_mac: 读卡器的地址
//  addr: 待写数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4442WriteMainMemory(char addr_mac,char addr,char Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


 //  4442卡写保护存储器
//  addr_mac: 读卡器的地址
//  addr: 待写数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4442WriteProtectMemory(char addr_mac,char addr,char Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
  

//  4442卡修改密钥
//  addr_mac: 读卡器的地址
//  pszOldKeyData: 旧密码，3字节
//  pszNewKeyData: 新密码，3字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4442ModifyKey(char addr_mac,PBYTE pszOldKeyData ,PBYTE pszNewKeyData,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4428卡读8位数据
//  addr_mac: 读卡器的地址
//  addr: 待读数据在卡中的首地址
//  len: 待读数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428Read8bit(char addr_mac,PBYTE addr,PBYTE len ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);



//  4428卡只读保护位
//  addr_mac: 读卡器的地址
//  addr: 待读数据在卡中的首地址
//  len: 待读数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428ReadPWS(char addr_mac,PBYTE addr,PBYTE len ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);



//  4428卡读9位数据
//  addr_mac: 读卡器的地址
//  addr: 待读数据在卡中的首地址
//  len: 待读数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428Read9bit(char addr_mac,PBYTE addr,PBYTE len ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4428卡校验密钥
//  addr_mac: 读卡器的地址
//  pszKeyData：密码，2字节
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428AuthenticationKey(char addr_mac,PBYTE pszKeyData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4428卡修改密钥
//  addr_mac: 读卡器的地址
//  pszOldKeyData: 旧密码，2字节 
//  pszNewKeyData: 新密码，2字节 
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428ModifyKey(char addr_mac,PBYTE pszOldKeyData ,PBYTE pszNewKeyData,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4428卡只写数据
//  addr_mac: 读卡器的地址
//  addr: 待写数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428WriteData(char addr_mac,PBYTE addr,PBYTE Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4428卡只写数据和保护位
//  addr_mac: 读卡器的地址
//  addr: 待写数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428WriteDataAndProtect(char addr_mac,PBYTE addr,PBYTE Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  4428卡只写保护位
//  addr_mac: 读卡器的地址
//  addr: 待写数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_4428WriteProtect(char addr_mac,PBYTE addr,PBYTE Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);



  //***********************************************************24C16卡**********************************************************
//  24C16上下电
//  addr_mac: 读卡器的地址
//  command: 命令字
//  para: 命令参数
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_24C16SendData (char addr_mac,char command,char para, PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  24C16卡读数据
//  addr_mac: 读卡器的地址
//  addr: 待读数据在卡中的首地址
//  len: 待读数据长度
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_24C16ReadData(char addr_mac,char cType,PBYTE addr,PBYTE len ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);


//  24CXX卡指定型号写数据
//  addr_mac: 读卡器的地址
//  cType:容量代号
//  addr: 要写的数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszWriteData：待写数据缓冲区
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_24C16WriteDataForType(char addr_mac,char cType,PBYTE addr,PBYTE Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

//  24CXX卡指定容量(具体型号)写数据，并回读写入的内容
//  addr_mac: 读卡器的地址
//  cType:容量代号
//  addr: 要写的数据在卡中的首地址
//  Writelen: 待写数据长度
//  pszWriteData：待写数据缓冲区
//  pszReadData: 从串口收到的数据存放的地址指针
//  nReadLen: 数组pszReadData的长度
//  dwRead: 从串口收到的数据的长度
//  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_24C16WriteDataForCapacity(char addr_mac,char cType,PBYTE addr,PBYTE Writelen,PBYTE pszWriteData ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);

  //***************************************************102**************************************************
#pragma region 102卡操作 
  //  102卡读主存储器
  //  addr_mac: 读卡器的地址
  //  addr: 要读的数据在卡中的首地址
  //  len: 要读取的数据长度
  //  pszReadData: 从串口收到的数据存放的地址指针
  //  nReadLen: 数组pszReadData的长度
  //  dwRead: 从串口收到的数据的长度
  //  nTimeWaite: 等待接收的预计超时时长
  DLL_EXP int PASCAL TY_102ReadMainMemory(char addr_mac, char addr, char len, PBYTE pszReadData, int nReadLen, DWORD &dwRead, int nTimeWaite);


	  //  102卡校验密钥
	  //  addr_mac: 读卡器的地址
	  //  pszKeyData：密码，2字节
	  //  pszReadData: 从串口收到的数据存放的地址指针
	  //  nReadLen: 数组pszReadData的长度
	  //  dwRead: 从串口收到的数据的长度
	  //  nTimeWaite: 等待接收的预计超时时长
	  DLL_EXP int PASCAL TY_102AuthenticationKey(char addr_mac, PBYTE pszKeyData, PBYTE pszReadData, int nReadLen, DWORD &dwRead, int nTimeWaite);

	  //  102卡写主存储器
	  //  addr_mac: 读卡器的地址
	  //  addr: 待写数据在卡中的首地址
	  //  Writelen: 待写数据长度
	  //  pszReadData: 从串口收到的数据存放的地址指针
	  //  nReadLen: 数组pszReadData的长度
	  //  dwRead: 从串口收到的数据的长度
	  //  nTimeWaite: 等待接收的预计超时时长
	  DLL_EXP int PASCAL TY_102WriteMainMemory(char addr_mac, char addr, unsigned char Writelen, PBYTE pszWriteData, PBYTE pszReadData, int nReadLen, DWORD &dwRead, int nTimeWaite);
	 
	  //  102卡读主存储器
	  //  addr_mac: 读卡器的地址
	  //  addr: 要读的数据在卡中的首地址
	  //  len: 要读取的数据长度
	  //  pszReadData: 从串口收到的数据存放的地址指针
	  //  nReadLen: 数组pszReadData的长度
	  //  dwRead: 从串口收到的数据的长度
	  //  nTimeWaite: 等待接收的预计超时时长
	  DLL_EXP int PASCAL TY_102ErasureMainMemory(char addr_mac, char addr, char len, PBYTE pszReadData, int nReadLen, DWORD &dwRead, int nTimeWaite);
	  

	  //  102卡修改密钥
	  //  addr_mac: 读卡器的地址
	  //  pszOldKeyData: 旧密码，2字节
	  //  pszNewKeyData: 新密码，2字节
	  //  pszReadData: 从串口收到的数据存放的地址指针
	  //  nReadLen: 数组pszReadData的长度
	  //  dwRead: 从串口收到的数据的长度
	  //  nTimeWaite: 等待接收的预计超时时长
	  DLL_EXP int PASCAL TY_102ModifyKey(char addr_mac, PBYTE pszOldKeyData, PBYTE pszNewKeyData, PBYTE pszReadData, int nReadLen, DWORD &dwRead, int nTimeWaite);
#pragma endregion 

#pragma region 读身份证信息 

  //  读身份证
  //  cName: 姓名
  //  cSex: 性别
  //  cNationID: 民族编号
  //  cBirth: 出生日期
  //  cAddress: 住址
  //  cIdNumber: 身份证号
  //  cDepartment: 签发机关
  //  cStartDate: 有效期起始日期
  //  cEndDate: 有效期截止日期	  
  DLL_EXP int PASCAL GetSFZMessage(char* cName, char* cSex, char* cNation, char* cBirth, char* cAddress, char* cIdNumber, char* cDepartment, char* cStartDate, char* cEndDate);
  
  //  读身份证
  //  cName: 姓名
  //  cSex: 性别
  //  cNation: 民族
  //  cNationID: 民族编号
  //  cBirth: 出生日期
  //  cAddress: 住址
  //  cIdNumber: 身份证号
  //  cDepartment: 签发机关
  //  cStartDate: 有效期起始日期
  //  cEndDate: 有效期截止日期
  //  cPhotoPath: 相片
  DLL_EXP int PASCAL GetIdCardMessage(char* cName, char* cSex, char* cNation, char* cNationID, char* cBirth, char* cAddress, char* cIdNumber, char* cDepartment, char* cStartDate, char* cEndDate, char* cPhotoPath);


  //  读身份证
  //  cName: 姓名
  //  cSex: 性别
  //  cNationID: 民族编号
  //  cBirth: 出生日期
  //  cAddress: 住址
  //  cIdNumber: 身份证号
  //  cDepartment: 签发机关
  //  cStartDate: 有效期起始日期
  //  cEndDate: 有效期截止日期	  
  //  cDLL_FILE_Path: DLL.FILE.dll路径
  DLL_EXP int PASCAL  GetSFZMessageR1(char* cName, char* cSex, char* cNation, char* cBirth, char* cAddress, char* cID, char* Department, char* cStartDate, char* cEndDate, char* cDLL_FILE_Path);

  //  读身份证
  //  cName: 姓名
  //  cSex: 性别
  //  cNation: 民族
  //  cNationID: 民族编号
  //  cBirth: 出生日期
  //  cAddress: 住址
  //  cIdNumber: 身份证号
  //  cDepartment: 签发机关
  //  cStartDate: 有效期起始日期
  //  cEndDate: 有效期截止日期
  //  cDLL_FILE_Path: DLL.FILE.dll路径
  DLL_EXP int PASCAL GetIdCardMessageR1(char* cName, char* cSex, char* cNation, char* cNationID, char* cBirth, char* cAddress, char* cIdNumber, char* cDepartment, char* cStartDate, char* cEndDate, char* cDLL_FILE_Path);
#pragma endregion

  DLL_EXP int PASCAL TY_SendPort(char addr_mac, char command, char para, PBYTE paraArr, int nParaLen, PBYTE pszReadData, int nReadLen, DWORD &dwRead, int nTimeWaite);
  //pcsc模式
  DLL_EXP int PASCAL SendModePcsc(bool isPscs);
  //加密模式
  DLL_EXP int PASCAL TY_ModeSlect(int nMode);

#ifdef __cplusplus
}
#endif
//===========================================================================
#endif
//===========================================================================