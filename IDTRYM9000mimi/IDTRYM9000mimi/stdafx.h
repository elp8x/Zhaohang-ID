// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <iostream>
#include <string>
#include <map>
#include <list>
using namespace std;







#include  <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
using namespace log4cplus;
using namespace log4cplus::helpers; 

#ifndef _DEBUG 
#pragma comment(lib,"log4cplusS.lib") 
#else 
#pragma comment(lib,"log4cplusSD.lib") 
#endif

//extern Logger pTestLogger;
//#define LOG_FATAL(x)	LOG4CPLUS_FATAL(pTestLogger,x)
//#define LOG_ERROR(x)	LOG4CPLUS_ERROR(pTestLogger,x)
//#define LOG_WARN(x)	    LOG4CPLUS_WARN(pTestLogger,x)
//#define LOG_INFO(x)	    LOG4CPLUS_INFO(pTestLogger,x)
//#define LOG_DEBUG(x)	LOG4CPLUS_DEBUG(pTestLogger,x)
//#define LOG_TRACE(x)	LOG4CPLUS_TRACE(pTestLogger,x)

#define LOG_FATAL(x)	LOG4CPLUS_FATAL(Logger::getRoot(),x)
#define LOG_ERROR(x)	LOG4CPLUS_ERROR(Logger::getRoot(),x)
#define LOG_WARN(x)	    LOG4CPLUS_WARN(Logger::getRoot(),x)
#define LOG_INFO(x)	    LOG4CPLUS_INFO(Logger::getRoot(),x)
#define LOG_DEBUG(x)	LOG4CPLUS_DEBUG(Logger::getRoot(),x)
#define LOG_TRACE(x)	LOG4CPLUS_TRACE(Logger::getRoot(),x)