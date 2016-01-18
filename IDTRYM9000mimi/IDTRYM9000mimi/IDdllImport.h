#pragma once

typedef int(__stdcall *OpenFunc)(PBYTE pszPort, PBYTE pszBuildCommDCB);
typedef int(__stdcall *CloseFunc)();
typedef int(__stdcall *AntennaControlFunc)(char addr_mac,char CtrlCode,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
typedef int(__stdcall *MFCPUSAMPowerOnFunc)(char addr_mac, char cType ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
typedef int(__stdcall *GetIdCardMessageR1Func)(char* cName, char* cSex, char* cNation, char* cNationID, char* cBirth, char* cAddress, char* cIdNumber, char* cDepartment, char* cStartDate, char* cEndDate, char* cDLL_FILE_Path);




class CIDdllImport
{
public:
	CIDdllImport(void);
	~CIDdllImport(void);
public:
	int IDReader_Open(PBYTE pszPort, PBYTE pszBuildCommDCB);
	int IDReader_Close();
	int IDReader_AntennaControl(char addr_mac,char CtrlCode,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
	int IDReader_MFCPUSAMPowerOn(char addr_mac, char cType ,PBYTE pszReadData, int nReadLen,DWORD &dwRead,int nTimeWaite);
	int IDReader_GetIdCardMessageR1(char* cName, char* cSex, char* cNation, char* cNationID, char* cBirth, char* cAddress, char* cIdNumber, char* cDepartment, char* cStartDate, char* cEndDate, char* cDLL_FILE_Path);


private:
	static HINSTANCE                m_hinDll;
	static OpenFunc                 m_lpOpen;
	static CloseFunc                m_lpClose;
	static AntennaControlFunc       m_lpAntennaControl;
	static MFCPUSAMPowerOnFunc      m_lpMFCPUSAMPowerOn;
	static GetIdCardMessageR1Func   m_lpGetIdCardMessageR1;
};

