#include "StdAfx.h"
#include "IDCard.h"
#include <process.h>

CIDCard::CIDCard(void)
{
	m_enumDevState=DEVICE_STATUS_NOT_READY;
	m_sErrorMsg="no errors";
	m_hMutex=NULL;
	m_hFileMapping=NULL;
	m_lpShareMemory=NULL;
	m_bClose=FALSE;
	m_bOpen=FALSE;
	m_bResultOpen=FALSE;
	m_bResultClose=FALSE;
	m_bResultAuthentication=FALSE;
	m_bResultGetData=FALSE;

	m_hEventOpen=NULL;
	m_hEventGetData=NULL;
	m_hEventClose=NULL;
	m_hEventAuthentication=NULL;

	//string sDllPath=CUtils::GetCurrDir()+"TYRM_9000L_268DLL.dll";
	string sDllPath="C:\\TianYi\\TYRM_9000L_268DLL.dll";
	LOG_INFO("TYRM_9000L_268DLL.dll address: "<<sDllPath);
	if (LoadLibrary(sDllPath.c_str()))
	{
		LOG_INFO("TYRM_9000L_268DLL.dll laoding successfuly");
	}
	else
	{
		LOG_INFO("loding fail error code: "<<GetLastError());
	}
}
CIDCard::~CIDCard(void)
{
}

ErrorCodeEnum CIDCard::GetDevCategory(DevCategoryInfo &devCategory){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 GetDevCategory！ ");
	devCategory.eState=m_enumDevState;
	devCategory.version.wMajor=1;
	devCategory.version.wMinor=0;
	devCategory.version.wRevision=1.0;
	devCategory.version.wBuild=1.1;
	memset(devCategory.szVendor,0,MAX_DEV_VENDOR_LEN);
	memset(devCategory.szModel,0,MAX_DEV_MODEL_LEN);
	memset(devCategory.szType,0,MAX_DEV_TYPE_LEN);
	string sVendor="HJ";
	string sType="";
	string sModel="";
	memcpy(devCategory.szVendor,sVendor.c_str(),sVendor.length()+1);
	memcpy(devCategory.szModel,sModel.c_str(),sModel.length()+1);
	memcpy(devCategory.szType,sType.c_str(),sType.length()+1);

	LOG_INFO("devCategory.szVendor:  "<<devCategory.szVendor);
	LOG_INFO("devCategory.szModel:  "<<devCategory.szModel);
	LOG_INFO("devCategory.szType:  "<<devCategory.szType);
	LOG_INFO("devCategory.eState(0-not ready,1-normal):  "<<devCategory.eState);



	LOG_INFO("结束调用函数 GetDevCategory！返回值 Error_Succeed");
	return Error_Succeed;
}
ErrorCodeEnum CIDCard::Reset(){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 Reset！ ");
	ErrorCodeEnum ErrorCode=Error_Succeed;
	



	LOG_INFO("结束调用函数 Reset！返回值 Error_Succeed");
	return ErrorCode;
}
ErrorCodeEnum CIDCard::DevClose(){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 DevClose！  ");
	ErrorCodeEnum ErrorCode=Error_Succeed;
	
	BOOL bToClose=FALSE;
	ShareMemory sm;
	int nR=GetResultFromFileMappingObject(sm);
	if (nR==0)
	{
		if (sm.bOpen==TRUE)//目前端口是开着的
		{
			LOG_INFO("目前端口是开着的");
			bToClose=TRUE;
		}
		else if (sm.bOpen==FALSE)
		{
			LOG_INFO("端口已经被关闭");
			bToClose=FALSE;
		}
	}
	//********************************************************************************************//
	if (TRUE==bToClose)//目前端口是开着的,需要关闭端口
	{
		LOG_INFO("由于端口是打开状态，需要关闭。");
		ShareMemory structMemoryInfos;
		structMemoryInfos.enumTask=id_Close;
		DoJobType enumJobType=doJob;
		if (0!=AddTask(structMemoryInfos,enumJobType))
		{
			LOG_ERROR("DevOpen-AddTask fail");
			ErrorCode=Error_DevConnFailed;
		}
		//********************************************************************************************//
		WaitForSingleObject(m_hEventClose,INFINITE);
		ShareMemory structMemoryResults;
		int nReturn=GetResultFromFileMappingObject(structMemoryResults);
		if (0==nReturn)
		{
			if (id_Close==structMemoryResults.enumTask && TRUE==structMemoryResults.bResult)
			{
				m_bOpen=FALSE;
				m_bClose=TRUE;
				LOG_INFO("results from file mapping object: close opens successfully!!!!!");
			}
			else if (id_Close==structMemoryResults.enumTask && FALSE==structMemoryResults.bResult)
			{
				LOG_INFO("results from file mapping object: close fail!!!!!");
				ErrorCode=Error_DevConnFailed;
			}
			else
			{
				LOG_INFO("others");
				ErrorCode=Error_DevConnFailed;
			}
		}
		else
		{
			LOG_ERROR("cannot get result from share memory.");
			ErrorCode=Error_DevConnFailed;
		}
		ResetEvent(m_hEventClose);
	}
	else
	{
		LOG_INFO("由于端口已经处于关闭状态，不需要再关闭");
	}
	
	//********************************************************************************************//
	if (Error_DevConnFailed==ErrorCode)
	{
		m_sErrorMsg="cannot close port";
	}
	Sleep(500);
	LOG_INFO("结束调用函数 DevClose！返回值 ："<<ErrorCode);
	return ErrorCode;
}
ErrorCodeEnum CIDCard::GetLastErr(DevErrorInfo &devErrInfo){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 GetLastErr！ ");
	devErrInfo.dwErrMsgLen=m_sErrorMsg.length();
	strcpy(devErrInfo.szErrMsg,m_sErrorMsg.c_str());
	LOG_INFO("devErrInfo.dwErrMsgLen:  "<<devErrInfo.dwErrMsgLen);
	LOG_INFO("devErrInfo.szErrMsg:  "<<devErrInfo.szErrMsg);
	LOG_INFO("结束调用函数 GetLastErr！返回值 Error_Succeed");
	return Error_Succeed;
}



ErrorCodeEnum CIDCard::DevOpen(DWORD dwPort){
	LOG_INFO("############################20150929####################################################");
	LOG_INFO("开始调用函数 DevOpen！ 参数dwPort: "<<dwPort);
	ErrorCodeEnum ErrorCode=Error_Succeed;

	m_bClose=FALSE;
	////////////////////////////////////////////////////////////////////////////////
	m_dwPort=dwPort;

	m_hEventOpen=CreateEvent(NULL,TRUE,FALSE,"OpenEvent");
	if (NULL==m_hEventOpen)
	{
		LOG_ERROR("Event \"OpenEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"OpenEvent\" already exists");
	}
	m_hEventClose=CreateEvent(NULL,TRUE,FALSE,"CloseEvent");
	if (NULL==m_hEventClose)
	{
		LOG_ERROR("Event \"CloseEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"CloseEvent\" already exists");
	}
	m_hEventAuthentication=CreateEvent(NULL,TRUE,FALSE,"AuthenticationEvent");
	if (NULL==m_hEventAuthentication)
	{
		LOG_ERROR("Event \"AuthenticationEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"AuthenticationEvent\" already exists");
	}
	m_hEventGetData=CreateEvent(NULL,TRUE,FALSE,"GetDataEvent");
	if (NULL==m_hEventGetData)
	{
		LOG_ERROR("Event \"GetDataEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"GetDataEvent\" already exists");
	}
	m_hEventFindCardType=CreateEvent(NULL,TRUE,FALSE,"FindCardTypeEvent");
	if (NULL==m_hEventFindCardType)
	{
		LOG_ERROR("Event \"FindCardTypeEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"FindCardTypeEvent\" already exists");
	}
	m_hEventActiveContactlessCard=CreateEvent(NULL,TRUE,FALSE,"ActiveContactlessCardEvent");
	if (NULL==m_hEventActiveContactlessCard)
	{
		LOG_ERROR("Event \"ActiveContactlessCardEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"ActiveContactlessCardEvent\" already exists");
	}
	m_hEventRFTypeABCommand=CreateEvent(NULL,TRUE,FALSE,"RFTypeABCommandEvent");
	if (NULL==m_hEventRFTypeABCommand)
	{
		LOG_ERROR("Event \"RFTypeABCommandEvent\" creates fail");
	}
	if (ERROR_ALREADY_EXISTS==GetLastError())
	{
		LOG_INFO("Event \"RFTypeABCommandEvent\" already exists");
	}

	//********************************************************************************************//
	do 
	{
		m_hMutex=CreateMutex(NULL,FALSE,"IDMutex");//create or open
		if (NULL==m_hMutex)
		{
			LOG_ERROR("CreateMutex return NULL with error code: "<<GetLastError());
			ErrorCode=Error_DevConnFailed;
			break;
		}
		m_hFileMapping=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(ShareMemory),"IDContactlessFileMappingObject");
		if (NULL==m_hFileMapping)
		{
			LOG_ERROR("CreateFileMapping return NULL with error code: "<<GetLastError());
			ErrorCode=Error_DevConnFailed;
			break;
		}
		if (ERROR_ALREADY_EXISTS==GetLastError())
		{
			LOG_INFO("File mapping object alreay exist.");
			break;
		}
		m_lpShareMemory=(lpShareMemory)MapViewOfFile(m_hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0);
		memset(m_lpShareMemory,0,sizeof(ShareMemory));

		/*HANDLE hThread=(HANDLE)_beginthreadex(NULL,0,FileMappingThread,this,0,NULL);
		if (NULL==hThread)
		{
			LOG_ERROR("_beginthreadex return NULL with error code: "<<GetLastError());
			CloseHandle(m_hFileMapping);
			CloseHandle(m_hMutex);
			m_hFileMapping=NULL;
			m_hMutex=NULL;
			hThread=NULL;
			ErrorCode=Error_DevConnFailed;
			break;
		}
		CloseHandle(hThread);
		hThread=NULL;*/
	} while (false);
	

	//********************************************************************************************/
	BOOL bNeedToOpen=FALSE;
	BOOL bNeedToCreateThread=FALSE;
	ShareMemory sm;
	int nR=GetResultFromFileMappingObject(sm);
	if (nR==0)
	{
		if (TRUE==sm.bOpen)
		{
			LOG_INFO("端口处于打开状态，不需要再次打开");
		}
		else if (false==sm.bOpen)
		{
			LOG_INFO("端口处于关闭状态，需要打开");
			bNeedToOpen=TRUE;
		}
		else
		{

		}
		if (TRUE==sm.bThreadExist)
		{
			LOG_INFO("多线程已经存在，不需要再次创建");
		}
		else if (FALSE==sm.bThreadExist)
		{
			LOG_INFO("多线程还未创建，需要创建");
			bNeedToCreateThread=TRUE;
		}
		else
		{

		}
	}

	if (bNeedToCreateThread==TRUE)
	{
		HANDLE hThread=(HANDLE)_beginthreadex(NULL,0,FileMappingThread,this,0,NULL);
		if (NULL==hThread)
		{
			LOG_ERROR("_beginthreadex return NULL with error code: "<<GetLastError());
			CloseHandle(m_hFileMapping);
			CloseHandle(m_hMutex);
			m_hFileMapping=NULL;
			m_hMutex=NULL;
			hThread=NULL;
			ErrorCode=Error_DevConnFailed;
		}
		CloseHandle(hThread);
		hThread=NULL;
		ShareMemory smTemp;
		smTemp.enumTask=id_Open_thread;
		smTemp.bThreadExist=TRUE;
		DoJobType djtTemp=SaveInfo;
		AddTask(smTemp,djtTemp);
	}



	if (TRUE==bNeedToOpen)
	{
		ShareMemory structMemoryInfos;
		structMemoryInfos.enumTask=id_Open;
		DoJobType enumJobType=doJob;
		if (0!=AddTask(structMemoryInfos,enumJobType))
		{
			LOG_ERROR("DevOpen-AddTask fail");
			ErrorCode=Error_DevConnFailed;
		}
		
		WaitForSingleObject(m_hEventOpen,INFINITE);
		ShareMemory structMemoryResults;
		int nReturn=GetResultFromFileMappingObject(structMemoryResults);
		if (0==nReturn)
		{
			if (id_Open==structMemoryResults.enumTask && TRUE==structMemoryResults.bResult)
			{
				m_bOpen=TRUE;
				m_bClose=FALSE;
				LOG_INFO("results from file mapping object: port opens successfully!!!!!");
			}
			else if (id_Open==structMemoryResults.enumTask && FALSE==structMemoryResults.bResult)
			{
				m_bOpen=FALSE;
				LOG_INFO("results from file mapping object: port opens fail!!!!!");
				ErrorCode=Error_DevConnFailed;
			}
			else
			{
				LOG_INFO("otheres");
				ErrorCode=Error_DevConnFailed;
			}
		}
		else
		{
			LOG_ERROR("cannot get result from share memory.");
			ErrorCode=Error_DevConnFailed;
		}
		ResetEvent(m_hEventOpen);
	}
	//********************************************************************************************//
	if (Error_DevConnFailed==ErrorCode)
	{
		m_sErrorMsg="cannot open port";
	}
	
	LOG_INFO("结束调用函数 DevOpen！返回值: "<< ErrorCode);
	return ErrorCode;
}
ErrorCodeEnum CIDCard::IDCerRFControl(bool bControl){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 IDCerRFControl！ 参数bControl: "<<bControl);
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	//bool bOnOrOff=bControl;
	//char szAddr_mac=0x68;
	//char szControl;
	//if (bOnOrOff==true)
	//{
	//	szControl=0x30;//open RF
	//}
	//else
	//{
	//	szControl=0x31;//close RF
	//}
	//LOG_INFO("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%szControl: "<<szControl);
	//PBYTE pByteReadData=new BYTE[2048];
	//memset(pByteReadData,0,2048*sizeof(BYTE));
	//int nReadLen=2048;
	//DWORD dwRead;
	//int nTimeWaits=3;
	//int nReturn=0;
	//nReturn=TY_MFAntennaCtrl(szAddr_mac,szControl,pByteReadData,nReadLen,dwRead,nTimeWaits);
	//if (nReturn!=0)
	//{
	//	LOG_INFO("RF control fail...");
	//	m_sErrorMsg="RF control fail";
	//	enumErrorCode=Error_DevCommFailed;
	//	return enumErrorCode;
	//	LOG_INFO("结束调用函数 IDCerRFControl！返回值：Error_DevCommFailed ");
	//}
	LOG_INFO("结束调用函数 IDCerRFControl！返回值：Error_Succeed ");
	return enumErrorCode;
}
ErrorCodeEnum CIDCard::IDCerAuthenticate(){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 IDCerAuthenticate！ 无参数");
	ErrorCodeEnum ErrorCode=Error_Succeed;
	LED_Flash();
	//********************************************************************************************//
	ShareMemory structMemoryInfos;
	structMemoryInfos.enumTask=id_Authentication;
	DoJobType enumJobType=doJob;
	if (0!=AddTask(structMemoryInfos,enumJobType))
	{
		LOG_ERROR("IDCerAuthenticate-AddTask fail");
		ErrorCode=Error_DevConnFailed;
	}
	//********************************************************************************************//
	LOG_INFO("before  WaitForSingleObject");
	WaitForSingleObject(m_hEventAuthentication,INFINITE);
	LOG_INFO("after  WaitForSingleObject");
	ShareMemory structMemoryResults;
	int nReturn=GetResultFromFileMappingObject(structMemoryResults);
	if (0==nReturn)
	{
		if (id_Authentication==structMemoryResults.enumTask && TRUE==structMemoryResults.bResult)
		{
			LOG_INFO("results from file mapping object: authentication successfully!!!!!");
		}
		else if (id_Authentication==structMemoryResults.enumTask && FALSE==structMemoryResults.bResult)
		{
			LOG_INFO("results from file mapping object: authentication fail!!!!!");
			ErrorCode=Error_DevConnFailed;
		}
		else
		{
			LOG_INFO("others");
			ErrorCode=Error_DevConnFailed;
		}
	}
	else
	{
		LOG_ERROR("cannot get result from share memory.");
		ErrorCode=Error_DevConnFailed;
	}
	//********************************************************************************************//
	if (Error_DevConnFailed==ErrorCode)
	{
		m_sErrorMsg="cannot find ID card";
	}

	BOOL bEventReturn=ResetEvent(m_hEventAuthentication);
	if (TRUE==bEventReturn)
	{
		LOG_INFO("ResetEvent(m_hEventAuthentication) successfully---身份证---无信号");
	}
	else
	{
		LOG_INFO("ResetEvent(m_hEventAuthentication) fail---身份证---无信号");
	}

	LED_Close();
	LOG_INFO("结束调用函数 IDCerAuthenticate！返回值: "<<ErrorCode);
	return ErrorCode;
}
ErrorCodeEnum CIDCard::IDCerGetData(IDCerInfo &idCerInfo){
	LOG_INFO("################################################################################");
	LOG_INFO("开始调用函数 IDCerGetData！");
	ErrorCodeEnum ErrorCode=Error_Succeed;
	LED_Flash();
	//********************************************************************************************//
	ShareMemory structMemoryInfos;
	structMemoryInfos.enumTask=id_GetData;
	DoJobType enumJobType=doJob;
	if (0!=AddTask(structMemoryInfos,enumJobType))
	{
		LOG_ERROR("IDCerGetData-AddTask fail");
		ErrorCode=Error_DevConnFailed;
	}
	//********************************************************************************************//
	//********************************************************************************************//
	WaitForSingleObject(m_hEventGetData,INFINITE);
	ShareMemory structMemoryResults;
	int nReturn=GetResultFromFileMappingObject(structMemoryResults);
	if (0==nReturn)
	{
		LOG_INFO("内存共享中获取信息成功");
		if (id_GetData==structMemoryResults.enumTask && TRUE==structMemoryResults.bResult)
		{
			LOG_INFO("results from file mapping object: get data successfully!!!!!");
			idCerInfo=structMemoryResults.idInfo;
			LOG_INFO("idCerInfo.name.data: "<<idCerInfo.name.data);
			LOG_INFO("idCerInfo.sex.data: "<<idCerInfo.sex.data);
			LOG_INFO("idCerInfo.nation.data: "<<idCerInfo.nation.data);
			LOG_INFO("idCerInfo.birthday.data: "<<idCerInfo.birthday.data);
			LOG_INFO("idCerInfo.address.data: "<<idCerInfo.address.data);
			LOG_INFO("idCerInfo.idno.data: "<<idCerInfo.idno.data);
			LOG_INFO("idCerInfo.department.data: "<<idCerInfo.department.data);
			LOG_INFO("idCerInfo.startDate.data: "<<idCerInfo.startDate.data);
			LOG_INFO("idCerInfo.endDate.data: "<<idCerInfo.endDate.data);
			LOG_INFO("idCerInfo.photoPath.data: "<<idCerInfo.photoPath.data);
		}
		else if (id_GetData==structMemoryResults.enumTask && FALSE==structMemoryResults.bResult)
		{
			LOG_INFO("results from file mapping object: get data fail!!!!!");
			ErrorCode=Error_DevConnFailed;
		}
		else
		{
			LOG_INFO("others");
			ErrorCode=Error_DevConnFailed;

		}
	}
	else
	{
		LOG_ERROR("cannot get result from share memory.");
		ErrorCode=Error_DevConnFailed;
	}
	//********************************************************************************************//
	if (Error_DevConnFailed==ErrorCode)
	{
		m_sErrorMsg="cannot read ID data";
	}

	ResetEvent(m_hEventGetData);
	LED_Close();
	LOG_INFO("结束调用函数 IDCerGetData！返回值:"<<ErrorCode);
	return ErrorCode;
	
}

int CIDCard::ID_Open(){
	LOG_INFO("ID_Open in...");
	char szPort[6]={0};
	itoa(m_dwPort,szPort,10);
	LOG_INFO("szPort: "<<szPort);
	unsigned char uszPort[16]={0};
	unsigned char uszBuildComm[30]={0};
	strcpy((char*)uszPort,"COM");
	strcat((char*)uszPort,szPort);
	LOG_INFO("uszPort: "<<uszPort);
	strcpy((char*)uszBuildComm,(const char*)uszPort);
	strcat((char*)uszBuildComm,":9600,N,8,1");
	LOG_INFO("uszBuildComm: "<<uszBuildComm);

	int nReturn=0;
	nReturn=PortOpen(uszPort,uszBuildComm);
	if (nReturn!=0)
	{
		LOG_ERROR("身份证读卡器打开失败");
		Sleep(1000);
		nReturn=PortOpen(uszPort,uszBuildComm);
		if (nReturn==0)
		{
			LOG_INFO("身份证读卡器打开成功");
			m_enumDevState=DEVICE_STATUS_NORMAL;
		}
		else
		{
			LOG_ERROR("身份证读卡器打开失败");
			nReturn=-1;
		}

	}
	else
	{
		LOG_INFO("身份证读卡器打开成功");
		m_enumDevState=DEVICE_STATUS_NORMAL;
	}
	LOG_INFO("ID_Open out...");
	return nReturn;

}
int CIDCard::ID_Close(){
	LOG_INFO("ID_Close in...");
	int nReturn=0;
	LOG_INFO("starting to call PortClose");
	nReturn=PortClose();
	if (nReturn!=0)
	{
		LOG_INFO("身份证读卡器关闭失败");
		nReturn=-1;
	}
	else
	{
		LOG_INFO("身份证读卡器关闭成功");
	}
	LOG_INFO("ID_Close out...");
	return nReturn;
}
int CIDCard::ID_Authenticate(){
	LOG_INFO("ID_Authenticate in...");
	int nReturn=0;
	do 
	{
		char szAddr=0x68;
		char szCommand=0x67;
		char szPara=0x57;
		char szMid=0x52;
		BYTE byteRead[2048]={0};
		int nLen=2048;
		DWORD dwLen;
		nReturn=TY_CPUSAMPowerOnOrDown(szAddr,szCommand,szPara,szMid,byteRead,nLen,dwLen,10);
		if (nReturn==0)
		{
			LOG_INFO("TY_CPUSAMPowerOnOrDown successfully");
			if (dwLen>7)
			{
				PBYTE info=new BYTE[dwLen];
				memset(info,0,dwLen*sizeof(BYTE));
				for (int i=0;i<dwLen;i++)
				{
					info[i]=byteRead[i];
					LOG_INFO("byteInfo[i]: "<<info[i]);

				}
				if (info[7]==0x30)
				{
					LOG_INFO("无卡");
					nReturn=-1;
					break;
				}
				else if (info[7]==0x31)
				{
					LOG_INFO("有卡-激活");
				}
				else if (info[7]==0x32)
				{
					LOG_INFO("有卡-未激活");
				}
				else {

				}

				delete[] info;
			}
			else
			{
				LOG_INFO("返回字节《7");
			}
		}
		else
		{
			LOG_INFO("TY_CPUSAMPowerOnOrDown fail");
		}

		///////////
		char szAddr_mac=0x68;
		char szType=0x42;
		BYTE byteReadData[2048]={0};
		int nReadLen=2048;
		DWORD dwRead;
		int nTimeWait=3;
		nReturn=TY_MFCPUSAMPowerOn(szAddr_mac,szType,byteReadData,nReadLen,dwRead,nTimeWait);  //=0 返回成功
		if (0==nReturn)
		{
			PBYTE byteInfo=new BYTE[dwRead];
			memset(byteInfo,0,dwRead*sizeof(BYTE));
			for (int i=0;i<dwRead;i++)
			{
				byteInfo[i]=byteReadData[i];
				LOG_INFO("byteInfo[i]: "<<byteInfo[i]);

			}
			if (byteInfo[7]=='S')
			{
				LOG_INFO("找到的卡为身份证(TYPE-B CPU卡)!");

			}
			else
			{
				LOG_INFO("没有找到身份证");
				nReturn=-1;
				break;

			}
		}
		else
		{
			nReturn=-1;
			break;
		}

	} while (false);
	LOG_INFO("ID_Authenticate out...return value: "<<nReturn);
	return nReturn;
}
int CIDCard::ID_GetData(IDCerInfo &idCerInfo){
	LOG_INFO("ID_GetData in...");
	int nResult=0;
	do 
	{
		char szCurrentDirToGet[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH,szCurrentDirToGet);
		LOG_INFO("GetCurrentDirectory: "<<szCurrentDirToGet);
		string sCurrentDirToSet="C:\\TianYi";
		if (SetCurrentDirectory(sCurrentDirToSet.c_str()))
		{
			LOG_INFO("SetCurrentDirectory successfully");
		}

		string sIDPhotoAddress=CUtils::GetCurrDir()+"zp.bmp";
		char szFill_Dll[260]={0};
		strcpy(szFill_Dll,"C:\\TianYi\\");
		LOG_INFO("szFill_Dll: "<<szFill_Dll);
		char szName[128]={0};
		char szSex[32]={0};
		char szNationInfo[32]={0};
		char szNationID[50]={0};
		char szBirth[32]={0};
		char szAddress[128]={0};
		char szIdNumber[32]={0};
		char szDepartment[128]={0};
		char szStartDate[32]={0};
		char szEndDate[32]={0};
		char szPhotoPath[260]={0};
		int nReturn=0;
		LOG_INFO("read");
		nReturn=GetIdCardMessageR1(szName,szSex,szNationInfo,szNationID,szBirth,szAddress,szIdNumber,szDepartment,szStartDate,szEndDate,szFill_Dll);
		//LOG_INFO("szName: "<<szName);
		//LOG_INFO("szSex: "<<szSex);
		//LOG_INFO("szNationInfo: "<<szNationInfo);
		//LOG_INFO("szNationID: "<<szNationID);
		//LOG_INFO("szBirth: "<<szBirth);
		//LOG_INFO("szAddress: "<<szAddress);
		//LOG_INFO("szIdNumber: "<<szIdNumber);
		//LOG_INFO("szDepartment: "<<szDepartment);
		//LOG_INFO("szStartDate: "<<szStartDate);
		//LOG_INFO("szEndDate: "<<szEndDate);
		strcpy(szPhotoPath,sIDPhotoAddress.c_str());
		//LOG_INFO("szPhotoPath: "<<szPhotoPath);
		//LOG_ERROR("#################################################nReturn: "<<nReturn);
		if (nReturn!=0 /*&& nReturn!=-22*/)
		{
			LOG_ERROR("读卡信息失败");
			nResult=-1;
			break;
		}
		else
		{
			LOG_INFO("读磁卡信息成功！信息为：");
		}

		int nNatinID=atoi(szNationID);
		switch(nNatinID){
		case 1:strcpy(szNationInfo, "汉族"); break;
		case 2:strcpy(szNationInfo, "蒙古族"); break;
		case 3:strcpy(szNationInfo, "回族"); break;
		case 4:strcpy(szNationInfo, "藏族"); break;
		case 5:strcpy(szNationInfo, "维吾尔族"); break;
		case 6:strcpy(szNationInfo, "苗族"); break;
		case 7:strcpy(szNationInfo, "彝族"); break;
		case 8:strcpy(szNationInfo, "壮族"); break;
		case 9:strcpy(szNationInfo, "布依族"); break;
		case 10:strcpy(szNationInfo, "朝鲜族"); break;
		case 11:strcpy(szNationInfo, "满族"); break;
		case 12:strcpy(szNationInfo, "侗族"); break;
		case 13:strcpy(szNationInfo, "瑶族"); break;
		case 14:strcpy(szNationInfo, "白族"); break;
		case 15:strcpy(szNationInfo, "土家族"); break;
		case 16:strcpy(szNationInfo, "哈尼族"); break;
		case 17:strcpy(szNationInfo, "哈萨克族"); break;
		case 18:strcpy(szNationInfo, "傣族"); break;
		case 19:strcpy(szNationInfo, "黎族"); break;
		case 20:strcpy(szNationInfo, "傈僳族"); break;
		case 21:strcpy(szNationInfo, "佤族"); break;
		case 22:strcpy(szNationInfo, "畲族"); break;
		case 23:strcpy(szNationInfo, "高山族"); break;
		case 24:strcpy(szNationInfo, "拉祜族"); break;
		case 25:strcpy(szNationInfo, "水族"); break;
		case 26:strcpy(szNationInfo, "东乡族"); break;
		case 27:strcpy(szNationInfo, "纳西族"); break;
		case 28:strcpy(szNationInfo, "景颇族"); break;
		case 29:strcpy(szNationInfo, "柯尔克孜族"); break;
		case 30:strcpy(szNationInfo, "土族"); break;
		case 31:strcpy(szNationInfo, "达斡尔族"); break;
		case 32:strcpy(szNationInfo, "仫佬族"); break;
		case 33:strcpy(szNationInfo, "羌族"); break;
		case 34:strcpy(szNationInfo, "布朗族"); break;
		case 35:strcpy(szNationInfo, "撒拉族"); break;
		case 36:strcpy(szNationInfo, "毛南族"); break;
		case 37:strcpy(szNationInfo, "仡佬族"); break;
		case 38:strcpy(szNationInfo, "锡伯族"); break;
		case 39:strcpy(szNationInfo, "阿昌族"); break;
		case 40:strcpy(szNationInfo, "普米族"); break;
		case 41:strcpy(szNationInfo, "塔吉克族"); break;
		case 42:strcpy(szNationInfo, "怒族"); break;
		case 43:strcpy(szNationInfo, "乌孜别克族"); break;
		case 44:strcpy(szNationInfo, "俄罗斯族"); break;
		case 45:strcpy(szNationInfo, "鄂温克族"); break;
		case 46:strcpy(szNationInfo, "德昂族"); break;
		case 47:strcpy(szNationInfo, "保安族"); break;
		case 48:strcpy(szNationInfo, "裕固族"); break;
		case 49:strcpy(szNationInfo, "京族"); break;
		case 50:strcpy(szNationInfo, "塔塔尔族"); break;
		case 51:strcpy(szNationInfo, "独龙族"); break;
		case 52:strcpy(szNationInfo, "鄂伦春族"); break;
		case 53:strcpy(szNationInfo, "赫哲族"); break;
		case 54:strcpy(szNationInfo, "门巴族"); break;
		case 55:strcpy(szNationInfo, "珞巴族"); break;
		case 56:strcpy(szNationInfo, "基诺族"); break;
		case 57:strcpy(szNationInfo, "其他"); break;
		case 58:strcpy(szNationInfo, "外国血统"); break;
		default:strcpy(szNationInfo, "00"); break;

		}

		strcpy(idCerInfo.name.data,szName);
		idCerInfo.name.dwSize=strlen(idCerInfo.name.data);
		if (strcmp(szSex,"1")==0)
		{
			strcpy(idCerInfo.sex.data,"男");
			idCerInfo.sex.dwSize=strlen(idCerInfo.sex.data);
		}
		else if (strcmp(szSex,"2")==0)
			//else if (strcmp(szSex,"0")==0)
		{
			strcpy(idCerInfo.sex.data,"女");
			idCerInfo.sex.dwSize=strlen(idCerInfo.sex.data);
		}
		else
		{

		}

		strcpy(idCerInfo.nation.data,szNationInfo);
		idCerInfo.nation.dwSize=strlen(idCerInfo.nation.data);
		//szBirth: 19861213
		string sBirth(szBirth);
		string s1=sBirth.substr(0,4);
		string s2=sBirth.substr(4,2);
		string s3=sBirth.substr(6);
		string s=s1+".";
		s=s+s2;
		s=s+".";
		s=s+s3;
		LOG_INFO("s: "<<s);

		strcpy(idCerInfo.birthday.data,s.c_str());
		idCerInfo.birthday.dwSize=strlen(idCerInfo.birthday.data);
		strcpy(idCerInfo.address.data,szAddress);
		idCerInfo.address.dwSize=strlen(idCerInfo.address.data);
		strcpy(idCerInfo.idno.data,szIdNumber);
		idCerInfo.idno.dwSize=strlen(idCerInfo.idno.data);
		strcpy(idCerInfo.department.data,szDepartment);
		idCerInfo.department.dwSize=strlen(idCerInfo.department.data);
		strcpy(idCerInfo.startDate.data,szStartDate);
		idCerInfo.startDate.dwSize=strlen(idCerInfo.startDate.data);
		strcpy(idCerInfo.endDate.data,szEndDate);
		idCerInfo.endDate.dwSize=strlen(idCerInfo.endDate.data);
		strcpy(idCerInfo.photoPath.data,szPhotoPath);
		idCerInfo.photoPath.dwSize=strlen(idCerInfo.photoPath.data);

		LOG_INFO("idCerInfo.name.data: "<<idCerInfo.name.data);
		LOG_INFO("idCerInfo.sex.data: "<<idCerInfo.sex.data);
		LOG_INFO("idCerInfo.nation.data: "<<idCerInfo.nation.data);
		LOG_INFO("idCerInfo.birthday.data: "<<idCerInfo.birthday.data);
		LOG_INFO("idCerInfo.address.data: "<<idCerInfo.address.data);
		LOG_INFO("idCerInfo.idno.data: "<<idCerInfo.idno.data);
		LOG_INFO("idCerInfo.department.data: "<<idCerInfo.department.data);
		LOG_INFO("idCerInfo.startDate.data: "<<idCerInfo.startDate.data);
		LOG_INFO("idCerInfo.endDate.data: "<<idCerInfo.endDate.data);
		LOG_INFO("idCerInfo.photoPath.data: "<<idCerInfo.photoPath.data);



		if (SetCurrentDirectory(szCurrentDirToGet))
		{
			LOG_INFO("SetCurrentDirectory successfully");
		}


		///////////////////////将照片move to 工作目录中/////////////////////////////////////
		LOG_INFO("移动照片");
		string sPhotoSrc="C:\\TianYi\\zp.bmp";
		string sPhotoDestination=CUtils::GetCurrDir()+"zp.bmp";
		BOOL bReturn=CopyFile(sPhotoSrc.c_str(),sPhotoDestination.c_str(),FALSE);
		if (bReturn)
		{
			LOG_INFO("move phote successfully");
		}
		else
		{
			LOG_ERROR("move photo fail");
		}
	} while (false);
	LOG_INFO("ID_GetData out... Return Value: "<<nResult);
	return nResult;
}
int CIDCard::ID_find(){
	LOG_INFO("ID_find in...");
	int nResult=0;
	PBYTE byteInfo=NULL;
	do 
	{
		char szAddr_mac=0x68;
		char szType=0x42;
		BYTE byteReadData[2048]={0};
		int nReadLen=2048;
		DWORD dwRead;
		int nTimeWait=3;
		int nReturn=0;
		nReturn=TY_MFCPUSAMPowerOn(szAddr_mac,szType,byteReadData,nReadLen,dwRead,nTimeWait);  //=0 返回成功
		if (nReturn<0)
		{
			LOG_ERROR("IDReader_MFCPUSAMPowerOn返回失败,返回值： "<<nReturn);
			nResult=-1;
			break;

		}
		LOG_INFO("IDReader_MFCPUSAMPowerOn successfully");
		byteInfo=new BYTE[dwRead];
		memset(byteInfo,0,dwRead*sizeof(BYTE));
		for (int i=0;i<dwRead;i++)
		{
			byteInfo[i]=byteReadData[i];

		}
		if (byteInfo[7]=='S')
		{
			m_nCardType=1;
			LOG_INFO("找到的卡为身份证(TYPE-B CPU卡)!");
		}
		else if (byteInfo[7]=='B'||byteInfo[7]=='2'||byteInfo[7]=='3')
		{
			m_nCardType=2;
			LOG_INFO("找到的卡为非接触卡(TYPE-B CPU卡)!");
		}
		else if (byteInfo[7]=='A')
		{
			m_nCardType=3;
			LOG_INFO("找到的卡为非接触卡(TYPE-A CPU卡)!");
		}
		else if (byteInfo[7]=='5'||byteInfo[7]=='7')
		{
			m_nCardType=10;
			LOG_INFO("找到的卡为s50 或者s70 为MF1卡");
		}
		else
		{
			LOG_INFO("没有找到身份证和非接卡");
			nResult=-1;
			break;
		}

	} while (false);

	delete[] byteInfo;
	LOG_INFO("ID_find out...return value： "<<nResult);
	return nResult;
}
int CIDCard::ID_ActiveContactlessCard(){
	LOG_INFO("ID_ActiveContactlessCard in...");
	int nResult=0;
	PBYTE byteInfo=NULL;
	do 
	{
		char szAddr_mac=0x68;
		char szType=0x40;//@
		BYTE byteReadData[1024]={0};
		int nReadLen=1024;
		DWORD dwRead;
		int nTimeWait=10;
		int nReturn=0;
		//nReturn=TY_MFCPUSAMPowerOn(szAddr_mac,szType,byteReadData,nReadLen,dwRead,nTimeWait);  //=0 返回成功
		nReturn=TY_MFCPUSAMPowerOn(szAddr_mac,szType,byteReadData,nReadLen,dwRead,nTimeWait); 
		if (nReturn==0)
		{
			LOG_INFO("发送上电命令成功");
			byteInfo=new BYTE[dwRead];
			memset(byteInfo,0,dwRead*sizeof(BYTE));
			for (int i=0;i<dwRead;i++)
			{
				byteInfo[i]=byteReadData[i];
				//LOG_INFO("byteInfo[i]: "<<byteInfo[i]);

			}
			if (byteInfo[6]=='0')
			{
				LOG_INFO("上电成功！");
			}
			else
			{
				LOG_INFO("上电失败！");
				nReturn=-1;
				break;
			}
		}
	} while (false);

	delete[] byteInfo;
	LOG_INFO("ID_ActiveContactlessCard out.....return value: "<<nResult);
	return nResult;
}
int CIDCard::ID_RFTypeABCommand(CmdInfo sendBuf,CmdInfo &recvBuf){
	LOG_INFO("ID_RFTypeABCommand in...");
	int nResult=0;
	do 
	{


		char szAddr_mac=0x68;
		BYTE byteReadData[2048]={0};
		int nReadLen=2048;
		DWORD dwRead=0;
		int nTimeWait=10;
		int nReturn=0;
		nReturn=TY_MFCPUSAMSendAPDU(szAddr_mac,sendBuf.data,sendBuf.dwSize,byteReadData,nReadLen,dwRead,nTimeWait);
		if (nReturn==0)
		{
			LOG_INFO("发送APDU成功");
			PBYTE byteInfo=new BYTE[dwRead];
			memset(byteInfo,0,dwRead*sizeof(BYTE));
			LOG_INFO("发送APDU后返回的数据长度： "<<dwRead);
			for (int i=0;i<dwRead;i++)
			{
				byteInfo[i]=byteReadData[i];
			}
			string sRecv = Asc2Hex((char*)byteInfo,dwRead);
			LOG_DEBUG("TY_MFCPUSAMSendAPDU接口 返回数据:"<<sRecv.c_str());
			if (byteInfo[6]!=0x30)//操作失败
			{
				LOG_INFO("操作失败");
				delete[] byteInfo;
				nResult=-1;
				break;
			}
			LOG_INFO("接下来对返回的命令进行解析：");
			int nAPDULen=dwRead-9;
			PBYTE byteAPDU=new BYTE[nAPDULen];
			memset(byteAPDU,0,nAPDULen*sizeof(BYTE));
			memcpy(byteAPDU,byteInfo+7,nAPDULen);
			sRecv = Asc2Hex((char*)byteAPDU,nAPDULen);
			LOG_DEBUG("截取的APDU:"<<sRecv.c_str());
			unsigned char szCharacterTemp=byteAPDU[0];
			if (szCharacterTemp==0x61 ||szCharacterTemp==0x6C)/////////////////////////////////////////////////////////////数据没有读完
			{
				LOG_INFO("数据没有读完，需要再发一次");
				BYTE byteResend[5]={0};
				byteResend[0]=0x00;
				byteResend[1]=0xC0;
				byteResend[2]=0x00;
				byteResend[3]=0x00;
				byteResend[4]=byteAPDU[1];
				BYTE byteResponse[2048]={0};
				int nLen=2048;
				DWORD dwResponse=0;
				int nTimeWait=10;
				nReturn=TY_MFCPUSAMSendAPDU(szAddr_mac,byteResend,5,byteResponse,nLen,dwResponse,10);
				if (nReturn==0)
				{
					LOG_INFO("重发成功");
					recvBuf.dwSize=dwResponse-9;
					LOG_INFO("****输出参数长度： "<<recvBuf.dwSize);
					memcpy(recvBuf.data,byteResponse+7,recvBuf.dwSize*sizeof(BYTE));
					sRecv = Asc2Hex((char*)byteAPDU,nAPDULen);
					LOG_DEBUG("****输出参数数据:"<<sRecv.c_str());
				}
				else
				{
					LOG_INFO("重发失败");
					nResult=-1;
					delete[] byteInfo;
					delete[] byteAPDU;
					break;
				}
			}
			else
			{
				memcpy(recvBuf.data,byteAPDU,nAPDULen*sizeof(BYTE));
				recvBuf.dwSize=nAPDULen;
				LOG_INFO("****输出参数长度： "<<recvBuf.dwSize);
				sRecv = Asc2Hex((char*)recvBuf.data,recvBuf.dwSize);
				LOG_DEBUG("****输出参数数据:"<<sRecv.c_str());
			}

			delete[] byteInfo;
			delete[] byteAPDU;
		}
		else
		{
			LOG_INFO("发送APDU失败");
			nResult=-1;
			break;
		}
	} while (false);


	LOG_INFO("ID_RFTypeABCommand out...return value: "<<nResult);
	return nResult;
}
string CIDCard::Asc2Hex(const char* szData,int nDataLen){
	char* szResult = new char[nDataLen*2+1];
	memset(szResult,0,nDataLen*2+1);

	unsigned char ch;
	for (int i=0;i<nDataLen;i++){
		ch=(char)((szData[i]&0xf0)>>4);
		szResult[i*2]=(char)((ch>9)?ch+0x41-10:ch+0x30);
		ch=(char)(szData[i]&0xf);
		szResult[i*2+1]=(char)((ch>9)?ch+0x41-10:ch+0x30);
	}
	string sResult(szResult,nDataLen*2);
	delete []szResult;
	return sResult;
}
int CIDCard::LED_Flash(){
	int nReturn=0;
	do 
	{
		nReturn=m_LEDInstance.LED_Open("5|9600",NULL,NULL);
		if (nReturn!=0)
		{
			LOG_INFO("LED 端口打开失败");
			nReturn=-1;
			break;
		}
		m_LEDInstance.LED_SetPort("i","f");
		nReturn=m_LEDInstance.LED_Close();
		if (nReturn!=0)
		{
			LOG_INFO("LED 端口关闭失败");
			nReturn=-1;
			break;
		}
	}while(false);
	return nReturn;
}
int CIDCard::LED_Close(){
	int nReturn=0;
	do 
	{
		nReturn=m_LEDInstance.LED_Open("5|9600",NULL,NULL);
		if (nReturn!=0)
		{
			LOG_INFO("LED 端口打开失败");
			nReturn=-1;
			break;
		}
		m_LEDInstance.LED_SetPort("i","c");
		nReturn=m_LEDInstance.LED_Close();
		if (nReturn!=0)
		{
			LOG_INFO("LED 端口关闭失败");
			nReturn=-1;
			break;
		}
	}while(false);
	return nReturn;

}

unsigned int CIDCard::FileMappingThread(LPVOID lpVoid){
	LOG_INFO("Thread in...");
	CIDCard*  lpThis=(CIDCard*)lpVoid;
	int nReturn=0;
	while(lpThis->m_bClose==FALSE){//调用DevClose退出线程
		WaitForSingleObject(lpThis->m_hMutex,INFINITE);
		HANDLE hFileObject=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"IDContactlessFileMappingObject");
		if (NULL==hFileObject)
		{
			LOG_ERROR("OpenFileMapping returns NULL with error code: "<<GetLastError());
			hFileObject=NULL;
			nReturn=-1;
			break;
		}
		lpShareMemory lpData=(lpShareMemory)MapViewOfFile(hFileObject,FILE_MAP_ALL_ACCESS,0,0,0);
		if (NULL==lpData)
		{
			LOG_ERROR("MapViewOfFile returns NULL with error code: "<<GetLastError());
			CloseHandle(hFileObject);
			hFileObject=NULL;
			nReturn=-1;
			break;
		}
		lpShareMemory lpMemoryContext=new ShareMemory;
		if (noAction!=lpData->enumTask)
		{
			LOG_INFO("@@@@@@@@@@@@@@@@@@@@@@@@@@@@Thread---lpData->enumTask: "<<lpData->enumTask);
			lpMemoryContext->enumTask=lpData->enumTask;
			lpThis->DoJobs(*lpMemoryContext);
			//lpData->enumTask=noAction;
		}
		UnmapViewOfFile(lpData);
		lpData=NULL;
		delete lpMemoryContext;
		CloseHandle(hFileObject);
		hFileObject=NULL;
		ReleaseMutex(lpThis->m_hMutex);
		Sleep(10);
	}
	LOG_INFO("Thread out...");
	return 0;
}
int CIDCard::AddTask(ShareMemory& infos,DoJobType enumJobType){
	LOG_INFO(">>>>>>>>>>>AddTask in.... Task Type: "<<infos.enumTask<<"  Type Result: "<<infos.bResult);
	int nReturn=0;

	do 
	{
		HANDLE hMutexOpen=OpenMutex(MUTEX_ALL_ACCESS,FALSE,"IDMutex");
		if (NULL==hMutexOpen)
		{
			LOG_ERROR("OpenMutex returns NULL with error code: "<<GetLastError());
			hMutexOpen=NULL;
			nReturn=-1;
			break;
		}
		WaitForSingleObject(hMutexOpen,INFINITE);
		HANDLE hFileObject=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"IDContactlessFileMappingObject");
		if (NULL==hFileObject)
		{
			LOG_ERROR("OpenFileMapping returns NULL with error code: "<<GetLastError());
			CloseHandle(hMutexOpen);
			hMutexOpen=NULL;
			hFileObject=NULL;
			nReturn=-1;
			ReleaseMutex(hMutexOpen);
			break;
		}
		lpShareMemory lpData=(lpShareMemory)MapViewOfFile(hFileObject,FILE_MAP_ALL_ACCESS,0,0,0);
		if (NULL==lpData)
		{
			LOG_ERROR("MapViewOfFile returns NULL with error code: "<<GetLastError());
			CloseHandle(hMutexOpen);
			hMutexOpen=NULL;
			CloseHandle(hFileObject);
			hFileObject=NULL;
			lpData=NULL;
			nReturn=-1;
			ReleaseMutex(hMutexOpen);
			break;
		}

		lpData->enumTask=infos.enumTask;
		lpData->bResult=infos.bResult;
		//lpData->nCardType=infos.nCardType;
		if (lpData->enumTask==id_Open)
		{
			lpData->bOpen=infos.bOpen;
		}
		if (lpData->enumTask==id_Close)
		{
			lpData->bOpen=infos.bOpen;
		}
		if (lpData->enumTask==id_Open_thread || lpData->enumTask==id_Close)
		{
			lpData->bThreadExist=infos.bThreadExist;
		}
		if (lpData->enumTask==findCardType)
		{
			lpData->nCardType=infos.nCardType;
		}
		if (lpData->enumTask==Card_RFTypeABCommand && enumJobType==doJob)
		{
			memcpy(lpData->CommandSend.data,infos.CommandSend.data,infos.CommandSend.dwSize);
			lpData->CommandSend.dwSize=infos.CommandSend.dwSize;
		}
		if (lpData->enumTask==Card_RFTypeABCommand && enumJobType==SaveInfo)
		{
			memcpy(lpData->CommandReceived.data,infos.CommandReceived.data,infos.CommandReceived.dwSize);
			lpData->CommandReceived.dwSize=infos.CommandReceived.dwSize;

		}
		if (lpData->enumTask==id_GetData && enumJobType==SaveInfo)
		{
			strcpy((lpData->idInfo).name.data,infos.idInfo.name.data);
			(lpData->idInfo).name.dwSize=infos.idInfo.name.dwSize;
			strcpy((lpData->idInfo).sex.data,infos.idInfo.sex.data);
			(lpData->idInfo).sex.dwSize=infos.idInfo.sex.dwSize;
			strcpy((lpData->idInfo).nation.data,infos.idInfo.nation.data);
			(lpData->idInfo).nation.dwSize=infos.idInfo.nation.dwSize;
			strcpy((lpData->idInfo).birthday.data,infos.idInfo.birthday.data);
			(lpData->idInfo).birthday.dwSize=infos.idInfo.birthday.dwSize;
			strcpy((lpData->idInfo).address.data,infos.idInfo.address.data);
			(lpData->idInfo).address.dwSize=infos.idInfo.address.dwSize;
			strcpy((lpData->idInfo).idno.data,infos.idInfo.idno.data);
			(lpData->idInfo).idno.dwSize=infos.idInfo.idno.dwSize;
			strcpy((lpData->idInfo).department.data,infos.idInfo.department.data);
			(lpData->idInfo).department.dwSize=infos.idInfo.department.dwSize;
			strcpy(lpData->idInfo.startDate.data,infos.idInfo.startDate.data);
			lpData->idInfo.startDate.dwSize=infos.idInfo.startDate.dwSize;
			strcpy((lpData->idInfo).endDate.data,infos.idInfo.endDate.data);
			(lpData->idInfo).endDate.dwSize=infos.idInfo.endDate.dwSize;
			strcpy((lpData->idInfo).newAddress.data,infos.idInfo.newAddress.data);
			(lpData->idInfo).newAddress.dwSize=infos.idInfo.newAddress.dwSize;
			strcpy((lpData->idInfo).photoPath.data,infos.idInfo.photoPath.data);
			(lpData->idInfo).photoPath.dwSize=infos.idInfo.photoPath.dwSize;
		}

		UnmapViewOfFile(lpData);
		lpData=NULL;
		CloseHandle(hFileObject);
		hFileObject=NULL;
		ReleaseMutex(hMutexOpen);
	} while (false);
	LOG_INFO(">>>>>>>>>>>AddTask out....");
	return nReturn;
}
int CIDCard::DoJobs(const ShareMemory& infos){
	LOG_INFO("**********DoJobs in...");
	TaskType enumJobType=infos.enumTask;
	if (noAction==enumJobType)
	{
		//do nothing
	}
	else if (id_Open==enumJobType && FALSE==m_bOpen)//处于没有打开状态则打卡
	{
		int nReturn=ID_Open();
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_Open;
			structMemoryInfos.bResult=TRUE;
			structMemoryInfos.bOpen=TRUE;
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventOpen);
		}
		else//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_Open;
			structMemoryInfos.bResult=FALSE;
			structMemoryInfos.bOpen=FALSE;
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventOpen);
		}
	}
	else if (id_Authentication==enumJobType)
	{
		int nReturn=ID_Authenticate();
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_Authentication;
			structMemoryInfos.bResult=TRUE;
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			BOOL bEventReturn=SetEvent(m_hEventAuthentication);
			if (TRUE==bEventReturn)
			{
				LOG_INFO("SetEvent(m_hEventAuthentication) successfully---身份证---有信号");
			}
			else
			{
				LOG_INFO("SetEvent(m_hEventAuthentication) fail---身份证---有信号");
			}
		}
		else//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_Authentication;
			structMemoryInfos.bResult=FALSE;
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			BOOL bEventReturn=SetEvent(m_hEventAuthentication);
			if (TRUE==bEventReturn)
			{
				LOG_INFO("SetEvent(m_hEventAuthentication) successfully---身份证---有信号");
			}
			else
			{
				LOG_INFO("SetEvent(m_hEventAuthentication) fail---身份证---有信号");
			}
		}

	}
	else if (id_GetData==enumJobType)
	{
		IDCerInfo idCerInfo;
		int nReturn=ID_GetData(idCerInfo);
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_GetData;
			structMemoryInfos.bResult=TRUE;
			strcpy(structMemoryInfos.idInfo.name.data,idCerInfo.name.data);
			structMemoryInfos.idInfo.name.dwSize=idCerInfo.name.dwSize;
			strcpy(structMemoryInfos.idInfo.sex.data,idCerInfo.sex.data);
			structMemoryInfos.idInfo.sex.dwSize=idCerInfo.sex.dwSize;
			strcpy(structMemoryInfos.idInfo.nation.data,idCerInfo.nation.data);
			structMemoryInfos.idInfo.nation.dwSize=idCerInfo.nation.dwSize;
			strcpy(structMemoryInfos.idInfo.birthday.data,idCerInfo.birthday.data);
			structMemoryInfos.idInfo.birthday.dwSize=idCerInfo.birthday.dwSize;
			strcpy(structMemoryInfos.idInfo.address.data,idCerInfo.address.data);
			structMemoryInfos.idInfo.address.dwSize=idCerInfo.address.dwSize;
			strcpy(structMemoryInfos.idInfo.idno.data,idCerInfo.idno.data);
			structMemoryInfos.idInfo.idno.dwSize=idCerInfo.idno.dwSize;
			strcpy(structMemoryInfos.idInfo.department.data,idCerInfo.department.data);
			structMemoryInfos.idInfo.department.dwSize=idCerInfo.department.dwSize;
			strcpy(structMemoryInfos.idInfo.startDate.data,idCerInfo.startDate.data);
			structMemoryInfos.idInfo.startDate.dwSize=idCerInfo.startDate.dwSize;
			strcpy(structMemoryInfos.idInfo.endDate.data,idCerInfo.endDate.data);
			structMemoryInfos.idInfo.endDate.dwSize=idCerInfo.endDate.dwSize;
			strcpy(structMemoryInfos.idInfo.newAddress.data,idCerInfo.newAddress.data);
			structMemoryInfos.idInfo.newAddress.dwSize=idCerInfo.newAddress.dwSize;
			strcpy(structMemoryInfos.idInfo.photoPath.data,idCerInfo.photoPath.data);
			structMemoryInfos.idInfo.photoPath.dwSize=idCerInfo.photoPath.dwSize;
			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventGetData);
		}
		else//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_GetData;
			structMemoryInfos.bResult=FALSE;
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventGetData);
		}
	}
	else if (id_Close==enumJobType && FALSE==m_bClose)
	{
		int nReturn=ID_Close();
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_Close;
			structMemoryInfos.bResult=TRUE;
			structMemoryInfos.bOpen=FALSE;
			structMemoryInfos.bThreadExist=FALSE;//////////
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventClose);
		}
		else//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=id_Close;
			structMemoryInfos.bResult=FALSE;
			structMemoryInfos.bOpen=TRUE;
			DoJobType emumJobType=noType;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventClose);
		}
	}
	else if (findCardType==enumJobType)
	{
		int nReturn=ID_find();
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=findCardType;
			structMemoryInfos.bResult=TRUE;
			structMemoryInfos.nCardType=m_nCardType;
			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventFindCardType);
		}
		else if (-1==nReturn)//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=findCardType;
			structMemoryInfos.bResult=FALSE;
			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventFindCardType);
		}
		else
		{

		}
	}
	else if (Card_ActiveContactlessICCard==enumJobType)
	{
		int nReturn=ID_ActiveContactlessCard();
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=Card_ActiveContactlessICCard;
			structMemoryInfos.bResult=TRUE;
			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventActiveContactlessCard);
		}
		else if (-1==nReturn)//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=Card_ActiveContactlessICCard;
			structMemoryInfos.bResult=FALSE;
			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventActiveContactlessCard);
		}
		else
		{

		}
	}
	else if (Card_RFTypeABCommand==enumJobType)
	{
		CmdInfo CommandSend;CmdInfo CommandReceive;
		memcpy(CommandSend.data,infos.CommandSend.data,infos.CommandSend.dwSize);
		CommandSend.dwSize=infos.CommandSend.dwSize;
		int nReturn=ID_RFTypeABCommand(CommandSend,CommandReceive);
		if (0==nReturn)//success
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=Card_RFTypeABCommand;
			structMemoryInfos.bResult=TRUE;
			structMemoryInfos.CommandReceived.dwSize=CommandReceive.dwSize;
			memcpy(structMemoryInfos.CommandReceived.data,CommandReceive.data,CommandReceive.dwSize);

			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventRFTypeABCommand);
		}
		else if (-1==nReturn)//fail
		{
			ShareMemory structMemoryInfos;
			structMemoryInfos.enumTask=Card_RFTypeABCommand;
			structMemoryInfos.bResult=FALSE;
			DoJobType emumJobType=SaveInfo;
			AddTask(structMemoryInfos,emumJobType);
			SetEvent(m_hEventRFTypeABCommand);
		}



	}
	else
	{

	}
	LOG_INFO("**********DoJobs out...");
	return 0;
}
int CIDCard::GetResultFromFileMappingObject(ShareMemory& infos){
	LOG_INFO("GetResultFromFileMappingObject in...");
	int nReturn=0;
	do 
	{
		WaitForSingleObject(m_hMutex,INFINITE);
		HANDLE hFileObject=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"IDContactlessFileMappingObject");
		if (NULL==hFileObject)
		{
			LOG_ERROR("OpenFileMapping returns NULL with error code: "<<GetLastError());
			hFileObject=NULL;
			nReturn=-1;
			break;
		}
		lpShareMemory lpData=(lpShareMemory)MapViewOfFile(hFileObject,FILE_MAP_ALL_ACCESS,0,0,0);
		if (NULL==lpData)
		{
			LOG_ERROR("MapViewOfFile returns NULL with error code: "<<GetLastError());
			CloseHandle(hFileObject);
			hFileObject=NULL;
			nReturn=-1;
			break;
		}
		infos.enumTask=lpData->enumTask;
		infos.bResult=lpData->bResult;
		infos.bOpen=lpData->bOpen;
		infos.bThreadExist=lpData->bThreadExist;
		infos.nCardType=lpData->nCardType;
		LOG_INFO("共享内存中得到数据为： ");
		LOG_INFO("lpData->enumTask: "<<lpData->enumTask);
		LOG_INFO("lpData->bResult: "<<lpData->bResult);
		LOG_INFO("lpData->bOpen: "<<lpData->bOpen);
		LOG_INFO("lpData->bThreadExist: "<<lpData->bThreadExist);
		LOG_INFO("lpData->nCardType: "<<lpData->nCardType);
		if (lpData->enumTask==Card_RFTypeABCommand && lpData->bResult==TRUE)
		{
			memcpy(infos.CommandReceived.data,lpData->CommandReceived.data,lpData->CommandReceived.dwSize);
			infos.CommandReceived.dwSize=lpData->CommandReceived.dwSize;
		}
		if (lpData->enumTask==id_GetData && lpData->bResult==TRUE)
		{
			strcpy(infos.idInfo.name.data,(lpData->idInfo).name.data);
			infos.idInfo.name.dwSize=(lpData->idInfo).name.dwSize;
			strcpy(infos.idInfo.sex.data,(lpData->idInfo).sex.data);
			infos.idInfo.sex.dwSize=(lpData->idInfo).sex.dwSize;
			strcpy(infos.idInfo.nation.data,(lpData->idInfo).nation.data);
			infos.idInfo.nation.dwSize=(lpData->idInfo).nation.dwSize;
			strcpy(infos.idInfo.birthday.data,(lpData->idInfo).birthday.data);
			infos.idInfo.birthday.dwSize=(lpData->idInfo).birthday.dwSize;
			strcpy(infos.idInfo.address.data,(lpData->idInfo).address.data);
			infos.idInfo.address.dwSize=(lpData->idInfo).address.dwSize;
			strcpy(infos.idInfo.idno.data,(lpData->idInfo).idno.data);
			infos.idInfo.idno.dwSize=(lpData->idInfo).idno.dwSize;
			strcpy(infos.idInfo.department.data,(lpData->idInfo).department.data);
			infos.idInfo.department.dwSize=(lpData->idInfo).department.dwSize;
			strcpy(infos.idInfo.startDate.data,(lpData->idInfo).startDate.data);
			infos.idInfo.startDate.dwSize=(lpData->idInfo).startDate.dwSize;
			strcpy(infos.idInfo.endDate.data,(lpData->idInfo).endDate.data);
			infos.idInfo.endDate.dwSize=(lpData->idInfo).endDate.dwSize;
			strcpy(infos.idInfo.newAddress.data,(lpData->idInfo).newAddress.data);
			infos.idInfo.newAddress.dwSize=(lpData->idInfo).newAddress.dwSize;
			strcpy(infos.idInfo.photoPath.data,(lpData->idInfo).photoPath.data);
			infos.idInfo.photoPath.dwSize=(lpData->idInfo).photoPath.dwSize;
		}
		else
		{

		}

		lpData->Initialisation();
		UnmapViewOfFile(lpData);
		lpData=NULL;
		CloseHandle(hFileObject);
		hFileObject=NULL;
		ReleaseMutex(m_hMutex);

	} while (false);
	LOG_INFO("GetResultFromFileMappingObject out... Return Value: "<<nReturn);
	return nReturn;
}