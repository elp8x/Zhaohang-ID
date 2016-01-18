// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "Utils.h"
#include <io.h>
#include <ShlObj.h>

Logger pTestLogger;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			CUtils::Init(hModule);
			char szLogPath[MAX_PATH]="C:\\log";
			CreateDirectory(szLogPath,NULL);
			log4cplus::initialize();
			string sConfig = CUtils::GetCurrDir()+"IDTRYM9000mimi.Hjjs.2.1.properties";
			PropertyConfigurator::doConfigure(sConfig.c_str());


			//if (0 != _access("c:\\log",0))
			//{
			//	SHCreateDirectoryEx(NULL,(CUtils::GetCurrDir()+"log").c_str(),NULL);
			//}
			//SharedAppenderPtr pFileAppender(new FileAppender("c:\\log\\IDTRYM9000mimi.log",8));
			//auto_ptr<Layout> pPatternLayout(new PatternLayout("%D{%m/%d/%y %H:%M:%S} [%-4t] %-5p    - %m [%l]%n"));//06/26/15 11:55:48 [33148] INFO     - 打印机打开成功 [PrinterStamper.cpp:193]
			//pFileAppender->setLayout(pPatternLayout);
			//pTestLogger=Logger::getInstance("LoggerName");
			//pTestLogger.addAppender(pFileAppender);
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

