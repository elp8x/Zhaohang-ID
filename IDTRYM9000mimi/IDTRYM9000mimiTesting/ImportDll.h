#pragma once
#include "DeviceBaseClass.h"
#include "IDCerClass.h"
#include "ErrorCode.h"


typedef ErrorCodeEnum(*CreateDevComponentFunc)(DeviceBaseClass *&baseObj);
typedef ErrorCodeEnum(*ReleaseDevComponentFunc)(DeviceBaseClass *&pBaseObj);




class CImportDll
{
private:
	CImportDll(void);
	~CImportDll(void);
	CImportDll(const CImportDll& instance){}
	CImportDll& operator=(const CImportDll& instance){}
public:
	static CImportDll* GetInstance();
	ErrorCodeEnum Import_Create();
	ErrorCodeEnum Import_Release();

private:
	CreateDevComponentFunc    m_lpCreate;
	ReleaseDevComponentFunc   m_lpRelease;
	HINSTANCE                 m_hinLib;

public:
	IDCerClass*                     m_lpID;
	


};

