#pragma once
class CUtils
{
public:
	CUtils(void);
	~CUtils(void);
public:
	static	void						Init(HINSTANCE hInstance);
public:
	//获取当前dll 名字(不带后缀)
	static  string						GetDllName();
	//获取dll当前目录
	static	string						GetCurrDir();
	//获取宿主程序目录
	static  string						GetHostDir();
	//获取当前模块句柄
	static	HINSTANCE					GetModule();

	static string						GetResultName(HRESULT hResult);

	static string&						StringTrimLeft(string& str,const string& trim=" ");
	static string&						StringTrimRight(string& str,const string& trim=" ");
	static string&						StringTrim(string& str,const string& trim=" ");

	static vector<string>				Split(string str,string pattern);



	static bool							SetPrint(int nPageSize,int nOrientation,int nRotain);

	static bool							GetPageIndex(map<string,int>& mapPages);

	static int							GetPageIndex(const char* szPage);

	static HDC							GetPrintDC(const char* szPrinter,const char* szPage,int nOrientation);


	wstring Utf82Unicode(const string& utf8string);
	string Unicode2Acsii(const wstring& wstrcode);
	string Utf82Ascii(const string& strUtf8Code);
private:
	static HINSTANCE					m_shInstance;
	static map<string,int>				m_mapPages;
};

