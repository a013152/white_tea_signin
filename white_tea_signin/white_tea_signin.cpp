// white_tea_signin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include "NFCReader.h" 
#include "Log.h"
#include "Curl.h"
#include "PlaySound.h"
#include <iostream>
std::string g_strAppDir;
std::string g_strAppSetIniPath;


int _tmain(int argc, _TCHAR* argv[])
{	
	
	 

	string str111 = "zh-CNC-00001-2";
	//[A-Z]-[0-9]{5}-[0-9]  
	//[\\s\\S]*shop.tdneed.com[\\s\\S]*
	//使用正则表达式判断
	//regex pattern2(regex_str2, regex::icase);
	
	//是否匹配
	std::string strPattern("a\\w");  // [line 1]
	std::regex r(strPattern);
	std::smatch matchResult;
	string strTag;
	

	////正则匹配
	//if (std::regex_match(str111, matchResult, regex_))
	
	{
		for (size_t i = 0; i < matchResult.size(); ++i)
		{
			strTag = matchResult[i];
			//std::cout << strTag << std::endl;
		}
	}
	/*string str222 = "I'm ChenLin,1988 is my birth year";
	string regex_str2(".*(\\d{4}).*");
	regex pattern2(regex_str2, regex::icase);
	if (regex_match(str222, matchResult, pattern2)){
		if (matchResult.size()>1)
			strTag = matchResult[1];
		 
	}*/
	printf("%s\n", strTag.c_str());


	initSys();
	/*GET_PLAYS->addPlay(CPlaySound::begin);
	GET_PLAYS->addPlay(CPlaySound::end);*/
	
	//设置回调
	GET_NFC->setCallbackFun(CCurl::logninFunction);
	//开启NFC读卡器
	if (GET_NFC->openDev()){		 
	  
	}
	  

	getchar();
	GET_NFC->freeInstance();
	GET_PLAYS->freeInstance();
	GET_LOG->freeInstance();
	return 0;
}

//获取应用程序目录

std::string getAppDir()
{ 
	TCHAR szBuf[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, (LPTSTR)szBuf, MAX_PATH);
	*(strrchr(szBuf, '\\') + sizeof(TCHAR)) = 0;    
	std::string strFileName = szBuf;
	return strFileName;
}

void initSys(){
	SYSTEMTIME sys;
	GetLocalTime(&sys);	
	string Day[7] = { "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" }; 
	string today_day = Day[sys.wDayOfWeek];	
	printf("%02d:%02d:%02d %s\nApp start!\n", sys.wHour, sys.wMinute, sys.wSecond, today_day.c_str());

	g_strAppDir = getAppDir();
	g_strAppSetIniPath = g_strAppDir + "set.ini";
	GET_LOG->logInfo(debugLog,"App start! ");
}



/*多字节转宽字节*/
std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, strlen(lpcszString), NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, strlen(lpcszString), (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

/*宽字节转多字节*/
std::string WStringToMBytes(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = ::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, wcslen(lpwcszWString), NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, wcslen(lpwcszWString), pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
//utf8 转unicode
std::wstring Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}
	return std::wstring(&resultstring[0]);
}
//utf-8 转 ascii  
std::string UTF_82ASCII(std::string& strUtf8Code)
{
	//先把 utf8 转为 unicode  
	std::wstring wstr = Utf82Unicode(strUtf8Code);
	//最后把 unicode 转为 ascii  
	std::string strRet = WStringToMBytes(wstr.c_str());
	return strRet;
}