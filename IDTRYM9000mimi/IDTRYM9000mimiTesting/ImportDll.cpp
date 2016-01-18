#include "StdAfx.h"
#include "ImportDll.h"
#include "string"
using namespace std;

CImportDll::CImportDll(void)
{
	m_lpCreate=NULL;
	m_lpRelease=NULL;
	m_hinLib=NULL;
	m_lpID=NULL;
}


CImportDll::~CImportDll(void)
{
	if (m_hinLib)
	{
		FreeLibrary(m_hinLib);
	}
}
CImportDll* CImportDll::GetInstance(){
	static CImportDll instance;
	return &instance;
}
ErrorCodeEnum CImportDll::Import_Create(){
	//string sDllPath="G:\\ZhaoShangBankPrinter20150605\\SecondPhaseCoding20150826\\IDTRYM9000mimi\\Release\\IDTRYM9000mimi.dll";
	char szFilePath[MAX_PATH]={0};
	GetModuleFileName(NULL,szFilePath,MAX_PATH);
	char szDriver[_MAX_DRIVE]={0};
	char szDirectory[_MAX_DIR]={0};
	char szFile[_MAX_FNAME]={0};
	char szExt[_MAX_EXT]={0};
	_splitpath_s(szFilePath,szDriver,szDirectory,szFile,szExt);//G:\ZhaoShangBankPrinter20150605\11111111Stamper LED\LEDControl\Release\LEDControlTesting.exe
	char szDllPath[MAX_PATH]={0};
	sprintf_s(szDllPath,"%s%s%s",szDriver,szDirectory,"IDTRYM9000mimi.dll");
	//LOG_INFO("dll path: "<<szDllPath);
	//string sDllPath="D:\\ZhaoShang sp\\IDTRYM9000mimi.dll";
	if (m_hinLib==NULL)
	{
		m_hinLib=LoadLibrary(szDllPath);
		if (!m_hinLib)
		{
			return Error_Null;
		}
		else
		{
			m_lpCreate=(CreateDevComponentFunc)GetProcAddress(m_hinLib,"CreateDevComponent");
			m_lpRelease=(ReleaseDevComponentFunc)GetProcAddress(m_hinLib,"ReleaseDevComponent");
			if (m_lpCreate==NULL || m_lpRelease==NULL)
			{
				return Error_Null;
			}
		}
	}
	

	return m_lpCreate((DeviceBaseClass *&)m_lpID);
	
}

ErrorCodeEnum CImportDll::Import_Release(){
	return m_lpRelease((DeviceBaseClass *&)m_lpID);
}