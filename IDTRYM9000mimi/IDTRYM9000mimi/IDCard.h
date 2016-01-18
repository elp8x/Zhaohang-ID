#pragma once
#include "IDCerClass.h"
#include "CardMix.h"
#include "ErrorCode.h"
#include "Utils.h"
#include "TYRM_9000L_268DLL.H"
#pragma comment(lib,"TYRM_9000L_268DLL.lib")


#include "SIUDllImport.h"
enum DoJobType{
	noType,
	doJob,
	SaveInfo
};
enum TaskType{
	noAction,
	id_Open,
	id_Authentication,
	id_GetData,
	id_Close,
	id_Open_thread,
	findCardType,
	Card_Open,
	Card_Close,
	Card_GetDevStatus,
	Card_QueryCardStatus,
	Card_ActiveContactlessICCard,
	Card_RFTypeABCommand
};
typedef struct tagShareMemory{
	TaskType enumTask;
	IDCerInfo idInfo;
	BOOL bResult;
	BOOL bOpen;
	BOOL bThreadExist;
	int nCardType;
	CmdInfo CommandSend;
	CmdInfo CommandReceived;
	//BOOL bClose;

	tagShareMemory(){
		enumTask=noAction;
		bResult=FALSE;
		bOpen=FALSE;
		bThreadExist=FALSE;
		nCardType=0;
		ZeroMemory(CommandSend.data,MAX_IC_BUFFER_SIZE);
		CommandSend.dwSize=0;
		ZeroMemory(CommandReceived.data,MAX_IC_BUFFER_SIZE);
		CommandReceived.dwSize=0;
		//bClose=FALSE;
		ZeroMemory(idInfo.name.data,MAX_IDCER_INFO_SIZE);
		idInfo.name.dwSize=0;
		ZeroMemory(idInfo.sex.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.sex.dwSize=0;
		ZeroMemory(idInfo.nation.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.nation.dwSize=0;
		ZeroMemory(idInfo.birthday.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.birthday.dwSize=0;
		ZeroMemory(idInfo.address.data,MAX_IDCER_INFO_SIZE);
		idInfo.address.dwSize=0;
		ZeroMemory(idInfo.idno.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.idno.dwSize=0;
		ZeroMemory(idInfo.department.data,MAX_IDCER_INFO_SIZE);
		idInfo.department.dwSize=0;
		ZeroMemory(idInfo.startDate.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.startDate.dwSize=0;
		ZeroMemory(idInfo.endDate.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.endDate.dwSize=0;
		ZeroMemory(idInfo.newAddress.data,MAX_IDCER_INFO_SIZE);
		idInfo.newAddress.dwSize=0;
		ZeroMemory(idInfo.photoPath.data,MAX_IDCER_PHOTO_PATH);
		idInfo.photoPath.dwSize=0;
	}
	~tagShareMemory(){}
	tagShareMemory(const tagShareMemory& ta){
		enumTask=ta.enumTask;
		bResult=ta.bResult;
		bOpen=ta.bOpen;
		nCardType=ta.nCardType;
		//strcpy((char*)CommandSend.data,(ta.CommandSend.data));
		memcpy(CommandSend.data,ta.CommandSend.data,ta.CommandSend.dwSize);
		CommandSend.dwSize=ta.CommandSend.dwSize;
		//strcpy((char*)CommandReceived.data,(ta.CommandReceived.data));
		memcpy(CommandReceived.data,ta.CommandReceived.data,ta.CommandReceived.dwSize);
		CommandReceived.dwSize=ta.CommandReceived.dwSize;

		strcpy(idInfo.name.data,ta.idInfo.name.data);
		idInfo.name.dwSize=ta.idInfo.name.dwSize;
		strcpy(idInfo.sex.data,ta.idInfo.sex.data);
		idInfo.sex.dwSize=ta.idInfo.sex.dwSize;
		strcpy(idInfo.nation.data,ta.idInfo.nation.data);
		idInfo.nation.dwSize=ta.idInfo.nation.dwSize;
		strcpy(idInfo.birthday.data,ta.idInfo.birthday.data);
		idInfo.birthday.dwSize=ta.idInfo.birthday.dwSize;
		strcpy(idInfo.address.data,ta.idInfo.address.data);
		idInfo.address.dwSize=ta.idInfo.address.dwSize;
		strcpy(idInfo.idno.data,ta.idInfo.idno.data);
		idInfo.idno.dwSize=ta.idInfo.idno.dwSize;
		strcpy(idInfo.department.data,ta.idInfo.department.data);
		idInfo.department.dwSize=ta.idInfo.department.dwSize;
		strcpy(idInfo.startDate.data,ta.idInfo.startDate.data);
		idInfo.startDate.dwSize=ta.idInfo.startDate.dwSize;
		strcpy(idInfo.endDate.data,ta.idInfo.endDate.data);
		idInfo.endDate.dwSize=ta.idInfo.endDate.dwSize;
		strcpy(idInfo.newAddress.data,ta.idInfo.newAddress.data);
		idInfo.newAddress.dwSize=ta.idInfo.newAddress.dwSize;
		strcpy(idInfo.photoPath.data,ta.idInfo.photoPath.data);
		idInfo.photoPath.dwSize=ta.idInfo.photoPath.dwSize;
	}

	void Initialisation(){
		enumTask=noAction;
		bResult=FALSE;
		//nCardType=0;

		ZeroMemory(CommandSend.data,MAX_IC_BUFFER_SIZE);
		CommandSend.dwSize=0;
		ZeroMemory(CommandReceived.data,MAX_IC_BUFFER_SIZE);
		CommandReceived.dwSize=0;

		ZeroMemory(idInfo.name.data,MAX_IDCER_INFO_SIZE);
		idInfo.name.dwSize=0;
		ZeroMemory(idInfo.sex.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.sex.dwSize=0;
		ZeroMemory(idInfo.nation.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.nation.dwSize=0;
		ZeroMemory(idInfo.birthday.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.birthday.dwSize=0;
		ZeroMemory(idInfo.address.data,MAX_IDCER_INFO_SIZE);
		idInfo.address.dwSize=0;
		ZeroMemory(idInfo.idno.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.idno.dwSize=0;
		ZeroMemory(idInfo.department.data,MAX_IDCER_INFO_SIZE);
		idInfo.department.dwSize=0;
		ZeroMemory(idInfo.startDate.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.startDate.dwSize=0;
		ZeroMemory(idInfo.endDate.data,MAX_IDCER_SIMPLE_INFO_SIZE);
		idInfo.endDate.dwSize=0;
		ZeroMemory(idInfo.newAddress.data,MAX_IDCER_INFO_SIZE);
		idInfo.newAddress.dwSize=0;
		ZeroMemory(idInfo.photoPath.data,MAX_IDCER_PHOTO_PATH);
		idInfo.photoPath.dwSize=0;
	}

}ShareMemory, *lpShareMemory;

class CIDCard:public IDCerClass
{
public:
	CIDCard(void);
	~CIDCard(void);
private:
	CIDCard(const CIDCard& instance){}
	CIDCard& operator=(const CIDCard& instance){}

public:
	virtual ErrorCodeEnum GetDevCategory(DevCategoryInfo &devCategory);
	virtual ErrorCodeEnum Reset();
	virtual ErrorCodeEnum DevClose();
	virtual ErrorCodeEnum GetLastErr(DevErrorInfo &devErrInfo);


	virtual ErrorCodeEnum DevOpen(DWORD dwPort);//
	virtual ErrorCodeEnum IDCerRFControl(bool bControl);//
	virtual ErrorCodeEnum IDCerAuthenticate();//
	virtual ErrorCodeEnum IDCerGetData(IDCerInfo &idCerInfo);//

private:
	int ID_Open();
	int ID_Close();
	int ID_Authenticate();
	int ID_GetData(IDCerInfo &idCerInfo);
	int ID_find();
	int ID_ActiveContactlessCard();
	int ID_RFTypeABCommand(CmdInfo sendBuf,CmdInfo &recvBuf);
	string Asc2Hex(const char* szData,int nDataLen);
	int LED_Flash();
	int LED_Close();

	static unsigned int WINAPI FileMappingThread(LPVOID lpVoid);
	int AddTask(ShareMemory& infos,DoJobType enumJobType);
	int DoJobs(const ShareMemory& infos);
	int GetResultFromFileMappingObject(ShareMemory& infos);

private:
	DevStateEnum  m_enumDevState;
	string        m_sErrorMsg;
	DWORD         m_dwPort;
	
	CSIUDllImport m_LEDInstance;
	HANDLE        m_hMutex;
	HANDLE        m_hFileMapping;
	lpShareMemory m_lpShareMemory;
	volatile BOOL m_bClose;
	volatile BOOL m_bOpen;

	volatile BOOL m_bResultOpen;
	volatile BOOL m_bResultClose;
	volatile BOOL m_bResultAuthentication;
	volatile BOOL m_bResultGetData;

	HANDLE        m_hEventOpen;
	HANDLE        m_hEventClose;
	HANDLE        m_hEventAuthentication;
	HANDLE        m_hEventGetData;
	HANDLE        m_hEventFindCardType;
	HANDLE        m_hEventActiveContactlessCard;
	HANDLE        m_hEventRFTypeABCommand;

	ShareMemory   m_ShareMemoryContext;

	volatile int  m_nCardType;
	
};

