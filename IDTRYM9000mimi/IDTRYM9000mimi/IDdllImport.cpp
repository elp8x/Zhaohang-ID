#include "StdAfx.h"
#include "IDdllImport.h"
#include "Utils.h"

HINSTANCE CIDdllImport::m_hinDll=NULL;
OpenFunc CIDdllImport::m_lpOpen=NULL;
CloseFunc CIDdllImport::m_lpClose=NULL;
AntennaControlFunc CIDdllImport::m_lpAntennaControl=NULL;
MFCPUSAMPowerOnFunc CIDdllImport::m_lpMFCPUSAMPowerOn=NULL;
GetIdCardMessageR1Func CIDdllImport::m_lpGetIdCardMessageR1=NULL;

CIDdllImport::CIDdllImport(void)
{
	
}


CIDdllImport::~CIDdllImport(void)
{
	if (m_hinDll)
	{
		FreeLibrary(m_hinDll);
	}
}


int CIDdllImport::IDReader_Open(PBYTE pszPort, PBYTE pszBuildCommDCB){
	if (!m_hinDll)
	{
		string sCurrentDir=CUtils::GetCurrDir();
		string sDllPath=CUtils::GetCurrDir()+"TYRM_9000L_268DLL.dll";
		LOG_INFO("TYRM_9000L_268DLL.dll address: "<<sDllPath);
		SetCurrentDirectory(sCurrentDir.c_str());
		m_hinDll=LoadLibrary(sDllPath.c_str());
		if (!m_hinDll)
		{
			LOG_ERROR("TYRM_9000L_268DLL.dll laoding fail with error code:"<<GetLastError());
			return -10;
		}
		m_lpOpen=(OpenFunc)GetProcAddress(m_hinDll,"PortOpen");
		m_lpClose=(CloseFunc)GetProcAddress(m_hinDll,"PortClose");
		m_lpAntennaControl=(AntennaControlFunc)GetProcAddress(m_hinDll,"TY_MFAntennaCtrl");
		m_lpMFCPUSAMPowerOn=(MFCPUSAMPowerOnFunc)GetProcAddress(m_hinDll,"TY_MFCPUSAMPowerOn");
		m_lpGetIdCardMessageR1=(GetIdCardMessageR1Func)GetProcAddress(m_hinDll,"GetIdCardMessageR1");
		if (m_lpOpen==NULL || m_lpClose==NULL || m_lpAntennaControl==NULL || m_lpMFCPUSAMPowerOn==NULL || m_lpGetIdCardMessageR1==NULL)
		{
			LOG_INFO("GetProcAddress fail");
			return -10;
		}
	}


	return m_lpOpen(pszPort,pszBuildCommDCB);

}
int CIDdllImport::IDReader_Close(){
	return m_lpClose();
}
int CIDdllImport::IDReader_AntennaControl(char addr_mac,char CtrlCode,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite){
	return m_lpAntennaControl(addr_mac,CtrlCode,pszReadData,nReadLen,dwRead,nTimeWaite);
}
int CIDdllImport::IDReader_MFCPUSAMPowerOn(char addr_mac, char cType ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite){
	return m_lpMFCPUSAMPowerOn(addr_mac,cType,pszReadData,nReadLen,dwRead,nTimeWaite);
}
int CIDdllImport::IDReader_GetIdCardMessageR1(char* cName, char* cSex, char* cNation, char* cNationID, char* cBirth, char* cAddress, char* cIdNumber, char* cDepartment, char* cStartDate, char* cEndDate, char* cDLL_FILE_Path){
	return m_lpGetIdCardMessageR1(cName,cSex,cNation,cNationID,cBirth,cAddress,cIdNumber,cDepartment,cStartDate,cEndDate,cDLL_FILE_Path);
}
