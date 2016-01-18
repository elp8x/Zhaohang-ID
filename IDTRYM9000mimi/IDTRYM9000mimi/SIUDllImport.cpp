#include "StdAfx.h"
#include "SIUDllImport.h"
#include "Utils.h"

HINSTANCE CSIUDllImport::m_hinDll=NULL;
OpenFunc CSIUDllImport::m_lpOpenFunc=NULL;
CloseFunc CSIUDllImport::m_lpCloseFunc=NULL;
ResetFunc CSIUDllImport::m_lpResetFunc=NULL;
CancelFunc CSIUDllImport::m_lpCancelFunc=NULL;
EnableEventFunc CSIUDllImport::m_lpEnableEventFunc=NULL;
GetStautsFunc CSIUDllImport::m_lpGetStautsFunc=NULL;
GetCapabilitiesFunc CSIUDllImport::m_lpGetCapabilitiesFunc=NULL;
SetPortFunc CSIUDllImport::m_lpSetPortFunc=NULL;

CSIUDllImport::CSIUDllImport(void)
{
	m_bOpen=FALSE;
}


CSIUDllImport::~CSIUDllImport(void)
{
	if (m_hinDll)
	{
		FreeLibrary(m_hinDll);
		m_hinDll=NULL;
	}
}

int CSIUDllImport::LED_Open(const char* lpName,EventHandle eventHandle,void* lpData){
	if (m_hinDll==NULL)
	{
		string sDllPath=CUtils::GetCurrDir()+"SIUService.Hjjs.2.1.dll";
		m_hinDll=LoadLibrary(sDllPath.c_str());
		if (!m_hinDll)
		{
			LOG_ERROR("load library fail with error code: "<<GetLastError());
			return -1;
		}
		m_lpOpenFunc=(OpenFunc)::GetProcAddress(m_hinDll,"Open");
		m_lpCloseFunc=(CloseFunc)::GetProcAddress(m_hinDll,"Close");
		m_lpResetFunc=(ResetFunc)::GetProcAddress(m_hinDll,"Reset");
		m_lpCancelFunc=(CancelFunc)::GetProcAddress(m_hinDll,"Cancel");
		m_lpEnableEventFunc=(EnableEventFunc)::GetProcAddress(m_hinDll,"EnableEvent");
		m_lpGetStautsFunc=(GetStautsFunc)::GetProcAddress(m_hinDll,"GetStatus");
		m_lpGetCapabilitiesFunc=(GetCapabilitiesFunc)::GetProcAddress(m_hinDll,"GetCapabilities");
		m_lpSetPortFunc=(SetPortFunc)::GetProcAddress(m_hinDll,"SetPort");
	}
	if (m_bOpen)
	{
		return 0;
	}
	int nReturn=m_lpOpenFunc(lpName,eventHandle,lpData);
	if (nReturn==0)
	{
		m_bOpen=TRUE;
		return 0;
	}
	else
	{
		return -1;
	}
}
int CSIUDllImport::LED_Close(void){
	if (!m_lpCloseFunc)
	{
		return -1;
	}
	int nReturn=m_lpCloseFunc();
	if (nReturn==0)
	{
		m_bOpen=FALSE;
		return 0;
	}
	else
	{
		return -1;
	}
	
}
int CSIUDllImport::LED_Reset(const char* lpActive){
	if (!m_lpResetFunc)
	{
		return -1;
	}
	return m_lpResetFunc(lpActive);
}
int CSIUDllImport::LED_Cancle(void){
	if (!m_lpCancelFunc)
	{
		return -1;
	}
	return m_lpCancelFunc();
}
int CSIUDllImport::LED_EnableEvent(bool bEnable){
	if (!m_lpEnableEventFunc)
	{
		return -1;
	}
	return m_lpEnableEventFunc(bEnable);
}
int CSIUDllImport::LED_GetStatus(char* lpStatus,int* nSize){
	if (!m_lpGetStautsFunc)
	{
		return -1;
	}
	return m_lpGetStautsFunc(lpStatus,nSize);
}
int CSIUDllImport::LED_GetCapabilities(char* lpCaps,int* nSize){
	if (!m_lpGetCapabilitiesFunc)
	{
		return -1;
	}
	return m_lpGetCapabilitiesFunc(lpCaps,nSize);
}

int CSIUDllImport::LED_SetPort(const char* lpPort,const char* lpStatus){
	if (!m_lpSetPortFunc)
	{
		return -1;
	}
	return m_lpSetPortFunc(lpPort,lpStatus);
}