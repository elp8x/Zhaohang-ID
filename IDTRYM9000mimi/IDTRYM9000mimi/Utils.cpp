#include "StdAfx.h"
#include "Utils.h"
//#include <XFSADMIN.H>
#include <WinSpool.h>
//#include <CommDlg.h>
HINSTANCE CUtils::m_shInstance=NULL;
map<string,int> CUtils::m_mapPages;
CUtils::CUtils(void){
}
CUtils::~CUtils(void){
}
void CUtils::Init(HINSTANCE hInstance){
	m_shInstance = hInstance;
}
string CUtils::GetDllName(){
	char szFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(m_shInstance,szFullPath,MAX_PATH); 
	char szDrive[_MAX_DRIVE]={0},szDir[_MAX_DIR]={0},szFname[_MAX_FNAME]={0},szExt[_MAX_EXT]={0}; 
	_splitpath_s(szFullPath,szDrive,sizeof(szDrive),szDir,sizeof(szDir),szFname,sizeof(szFname),szExt,sizeof(szExt));
	return string(szFname);
}
string CUtils::GetCurrDir(){
	char szFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(m_shInstance,szFullPath,MAX_PATH); 
	char szDrive[_MAX_DRIVE]={0},szDir[_MAX_DIR]={0},szFname[_MAX_FNAME]={0},szExt[_MAX_EXT]={0}; 
	_splitpath_s(szFullPath,szDrive,sizeof(szDrive),szDir,sizeof(szDir),szFname,sizeof(szFname),szExt,sizeof(szExt));
	char szCurrDir[MAX_PATH]={0};
	sprintf_s(szCurrDir,sizeof(szCurrDir),"%s%s",szDrive,szDir);
	return string(szCurrDir);
}
string CUtils::GetHostDir(){
	char szFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL,szFullPath,MAX_PATH); 
	char szDrive[_MAX_DRIVE]={0},szDir[_MAX_DIR]={0},szFname[_MAX_FNAME]={0},szExt[_MAX_EXT]={0}; 
	_splitpath_s(szFullPath,szDrive,sizeof(szDrive),szDir,sizeof(szDir),szFname,sizeof(szFname),szExt,sizeof(szExt));
	char szHostDir[MAX_PATH]={0};
	sprintf_s(szHostDir,sizeof(szHostDir),"%s%s",szDrive,szDir);
	return string(szHostDir);
}
HINSTANCE CUtils::GetModule(){
	return m_shInstance;
}

string&	CUtils::StringTrim(string& str,const string& trim){
	StringTrimLeft(str,trim);
	StringTrimRight(str,trim);
	return str;
}
string& CUtils::StringTrimLeft(string& str,const string& trim){  
	return str.erase(0,str.find_first_not_of(trim.c_str()));  
} 
string& CUtils::StringTrimRight(string& str,const string& trim){  
	return str.erase(str.find_last_not_of(trim.c_str())+1);  
}  

vector<string> CUtils::Split(string str,string pattern){
	string::size_type pos;
	vector<std::string> result;
	str+=pattern;//扩展字符串以方便操作
	int size=str.size();
	for(int i=0; i<size; i++){
		pos=str.find(pattern,i);
		if(pos<size){
			string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}


HDC	CUtils::GetPrintDC(const char* szPrinter,const char* szPage,int nOrientation){
	HDC hDC = NULL;
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	ZeroMemory(&pd, sizeof(pd)); 
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	BOOL bResult = OpenPrinter((char*)szPrinter,&hPrinter,&pd);
	if(bResult && hPrinter){
		DWORD dwSizeNeeded=0;
		GetPrinter(hPrinter,2,0,0,&dwSizeNeeded);
		if(dwSizeNeeded){
			PRINTER_INFO_2* pi2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,dwSizeNeeded);
			bResult = GetPrinter(hPrinter,2,(LPBYTE)pi2,dwSizeNeeded,&dwSizeNeeded);
			if(bResult==FALSE){
				//LOG_ERROR("获取打印机信息2失败");
				return NULL;
			}
			if (pi2->pDevMode == NULL){
				//LOG_INFO("打印机 DevMode为空,尝试获取");
				dwSizeNeeded = DocumentProperties(NULL, hPrinter,pi2->pPrinterName,NULL, NULL, 0);
				if (dwSizeNeeded <= 0){
					//LOG_ERROR("获取DevMode失败");
					GlobalFree(pi2);
					ClosePrinter(hPrinter);
					return FALSE;
				}

				DEVMODE* pDevMode = (DEVMODE *)GlobalAlloc(GPTR,dwSizeNeeded);
				if (pDevMode == NULL){
					//LOG_ERROR("获取DevMode失败");
					GlobalFree(pi2);
					ClosePrinter(hPrinter);
					return NULL;
				}
				DocumentProperties(NULL,hPrinter,pi2->pPrinterName,pDevMode, NULL,DM_OUT_BUFFER);
				pi2->pDevMode = pDevMode;
			}
			int nPageSize = GetPageIndex(szPage);
			if(nPageSize<=0){
				//LOG_ERROR("获取纸张失败");
				//GlobalFree(pi2);
				//ClosePrinter(hPrinter);
				//return NULL;
			}
			pi2->pDevMode->dmPaperSize = nPageSize;
			pi2->pDevMode->dmOrientation = nOrientation;
			pi2->pDevMode->dmFields = DM_PAPERSIZE|DM_ORIENTATION;
			pi2->pSecurityDescriptor = NULL;

			hDC = ::CreateDC((LPCTSTR)pi2->pDriverName,szPrinter,(LPCTSTR)pi2->pPortName,pi2->pDevMode);


			GlobalFree(pi2);
			ClosePrinter(hPrinter);
		}


	}else{
		hDC = NULL;
		//LOG_ERROR("打开打印机失败");
	}
	return hDC;
}

int CUtils::GetPageIndex(const char* szPage){
	if (!m_mapPages.size()){
		if (!GetPageIndex(m_mapPages)){
			return 0;
		}
	}
	map<string,int>::iterator itor = m_mapPages.find(szPage);
	if(itor == m_mapPages.end())
		return 0;
	return itor->second;
}

bool CUtils::SetPrint(int nPageSize,int nOrientation,int nRotain){
	//LOG_INFO("开始设置打印机参数");
	string sPrinter;
	DWORD dwSizeNeeded=0;
	::GetDefaultPrinter(NULL,&dwSizeNeeded);
	LPTSTR lpPrinterName=(LPTSTR)::GlobalAlloc(GPTR,dwSizeNeeded);
	if(GetDefaultPrinter(lpPrinterName,&dwSizeNeeded)){
		sPrinter=lpPrinterName;
	}
	::GlobalFree((HGLOBAL)lpPrinterName);
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;

	ZeroMemory(&pd, sizeof(pd)); 
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	BOOL bResult = OpenPrinter((char*)sPrinter.c_str(),&hPrinter,&pd);
	if(bResult && hPrinter){
		GetPrinter(hPrinter,2,0,0,&dwSizeNeeded);
		if(dwSizeNeeded){
			PRINTER_INFO_2* pi2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,dwSizeNeeded);
			bResult = GetPrinter(hPrinter,2,(LPBYTE)pi2,dwSizeNeeded,&dwSizeNeeded);
			if(bResult==FALSE){
				return false;
			}
			if (pi2->pDevMode == NULL){
				//LOG_INFO("打印机 DevMode为空,尝试获取");
				dwSizeNeeded = DocumentProperties(NULL, hPrinter,pi2->pPrinterName,NULL, NULL, 0);
				if (dwSizeNeeded <= 0){
					//LOG_ERROR("获取DevMode失败");
					GlobalFree(pi2);
					ClosePrinter(hPrinter);
					return FALSE;
				}

				DEVMODE* pDevMode = (DEVMODE *)GlobalAlloc(GPTR,dwSizeNeeded);
				if (pDevMode == NULL){
					//LOG_ERROR("获取DevMode失败");
					GlobalFree(pi2);
					ClosePrinter(hPrinter);
					return FALSE;
				}
				DocumentProperties(NULL,hPrinter,pi2->pPrinterName,pDevMode, NULL,DM_OUT_BUFFER);
				pi2->pDevMode = pDevMode;
			}
			pi2->pDevMode->dmPaperSize = nPageSize;
			pi2->pDevMode->dmOrientation = nOrientation;
			pi2->pDevMode->dmFields = DM_PAPERSIZE|DM_ORIENTATION;
			pi2->pSecurityDescriptor = NULL;
			DWORD fMode = DM_IN_BUFFER | DM_OUT_BUFFER;
			//LOG_INFO("DocumentProperties 开始");
			DocumentProperties(NULL, hPrinter,pi2->pPrinterName,pi2->pDevMode,pi2->pDevMode,fMode);
			//LOG_INFO("DocumentProperties 结束");
			bResult = SetPrinter(hPrinter,2,(LPBYTE)pi2,0);
			//LOG_INFO("SetPrinter 结束");
			if(!bResult){
				//LOG_ERROR("打印机设置失败");
			}
			GlobalFree(pi2);
			//GlobalFree(pi2->pDevMode);
			ClosePrinter(hPrinter);
			//SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE,0L,(LPARAM)(LPCTSTR)"windows", SMTO_NORMAL, 100, NULL);
		}

		//LOG_INFO("打印机参数设置完成");
	}
	return true;
}
typedef TCHAR PAPERNAME[64]; //打印机纸张名称类型
bool CUtils::GetPageIndex(map<string,int>& mapPages){
	string sPrinter;
	DWORD dwSizeNeeded=0;
	::GetDefaultPrinter(NULL,&dwSizeNeeded);
	LPTSTR lpPrinterName=(LPTSTR)::GlobalAlloc(GPTR,dwSizeNeeded);
	if(GetDefaultPrinter(lpPrinterName,&dwSizeNeeded)){
		sPrinter=lpPrinterName;
	}
	::GlobalFree((HGLOBAL)lpPrinterName);
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;

	ZeroMemory(&pd, sizeof(pd)); 
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	BOOL bResult = OpenPrinter((char*)sPrinter.c_str(),&hPrinter,&pd);
	if(bResult && hPrinter){
		GetPrinter(hPrinter,2,0,0,&dwSizeNeeded);
		if(dwSizeNeeded){
			PRINTER_INFO_2* pi2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,dwSizeNeeded);
			bResult = GetPrinter(hPrinter,2,(LPBYTE)pi2,dwSizeNeeded,&dwSizeNeeded);
			if(bResult==FALSE){
				return false;
			}
			int nNeeded = DeviceCapabilities(sPrinter.c_str(),pi2->pPortName,DC_PAPERNAMES, NULL, NULL);
			if(nNeeded){
				PAPERNAME *pszPaperNames = new PAPERNAME[nNeeded]; //分配纸张名称数组
				if(DeviceCapabilities(sPrinter.c_str(),pi2->pPortName, DC_PAPERNAMES, (LPTSTR)pszPaperNames, NULL) != -1){
					nNeeded = DeviceCapabilities(sPrinter.c_str(),pi2->pPortName, DC_PAPERS, NULL, NULL);
					LPWORD pPapers = new WORD[nNeeded]; //分配纸张尺寸号数组
					if (DeviceCapabilities(sPrinter.c_str(),pi2->pPortName, DC_PAPERS, (LPSTR)pPapers, NULL) != -1){
						for (int i = 0; i < nNeeded;i++){
							mapPages[pszPaperNames[i]]=pPapers[i];
						}
					}
					//查找纸张类型szPaperName在数组中的索引
				}
				delete []pszPaperNames;
			}

		}
		ClosePrinter(hPrinter);
	}
	return true;
}

//下边的字符串转换函数使用了win的api
wstring CUtils::Utf82Unicode(const string& utf8string){  
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(),utf8string.size(), NULL, 0);  
	/*if(widesize == ERROR_NO_UNICODE_TRANSLATION)  {  
		return wstring(_T(" "));  
	}  
	if(widesize == 0){  
		return wstring(_T(" "));  
	} */ 
	std::vector<wchar_t> resultstring(widesize+1);  
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(),utf8string.size(), &resultstring[0], widesize);  
	/*if(convresult != widesize){  
		return wstring(_T(" "));
	}  */
	return std::wstring(&resultstring[0]);  
}  
string CUtils::Unicode2Acsii(const wstring& wstrcode){  
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), wstrcode.size(), NULL, 0, NULL, NULL);  
	if(asciisize == ERROR_NO_UNICODE_TRANSLATION){  
		return string("");
	}  
	if(asciisize == 0){  
		return string("");
	}  
	std::vector<char> resultstring(asciisize+1);  
	int convresult =::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(),wstrcode.size(), &resultstring[0], asciisize, NULL, NULL);  
	if(convresult != asciisize){  
		return string("");
	}  
	return std::string(&resultstring[0]);  
}  

string CUtils::Utf82Ascii(const string& strUtf8Code)  {    
	//先把 utf8 转为 unicode  
	wstring wstr = Utf82Unicode(strUtf8Code);  
	//最后把 unicode 转为 ascii  
	return Unicode2Acsii(wstr);   
} 