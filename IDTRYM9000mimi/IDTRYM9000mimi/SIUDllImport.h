#pragma once
typedef void(CALLBACK* EventHandle)(const void*,const char*,const char*);

typedef int(__stdcall *OpenFunc)(const char* lpName,EventHandle eventHandle,void* lpData);
typedef int(__stdcall *CloseFunc)(void);
typedef int(__stdcall *ResetFunc)(const char* lpActive);
typedef int(__stdcall *CancelFunc)(void);
typedef int(__stdcall *EnableEventFunc)(bool bEnable);
typedef int(__stdcall *GetStautsFunc)(char* lpStatus,int* nSize);
typedef int(__stdcall *GetCapabilitiesFunc)(char* lpCaps,int* nSize);
typedef int(__stdcall *SetPortFunc)(const char* lpPort,const char* lpStatus);


class CSIUDllImport
{
public:
	CSIUDllImport(void);
	~CSIUDllImport(void);
public:
	int LED_Open(const char* lpName,EventHandle eventHandle,void* lpData);
	int LED_Close(void);
	int LED_Reset(const char* lpActive);
	int LED_Cancle(void);
	int LED_EnableEvent(bool bEnable);
	int LED_GetStatus(char* lpStatus,int* nSize);
	int LED_GetCapabilities(char* lpCaps,int* nSize);
	int LED_SetPort(const char* lpPort,const char* lpStatus);

private:
	static HINSTANCE            m_hinDll;
	static OpenFunc             m_lpOpenFunc;
	static CloseFunc            m_lpCloseFunc;
	static ResetFunc            m_lpResetFunc;
	static CancelFunc           m_lpCancelFunc;
	static EnableEventFunc      m_lpEnableEventFunc;
	static GetStautsFunc        m_lpGetStautsFunc;
	static GetCapabilitiesFunc  m_lpGetCapabilitiesFunc;
	static SetPortFunc          m_lpSetPortFunc;

	BOOL                        m_bOpen;
};

