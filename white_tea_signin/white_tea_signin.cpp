// white_tea_signin.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//��ʼ��
	initSys();
		
	//����NFC��Ӧ�ص�
	GET_NFC->setCallbackFun(CCurl::signinFunction);

	//����NFC������
	if (GET_NFC->openDev()){		 
		GET_CURL->init();
		GET_PLAYS->init();	  
	}	 

	getchar();
	GET_NFC->freeInstance();
	GET_PLAYS->freeInstance();
	GET_LOG->freeInstance();
	return 0;
}

//��ȡӦ�ó���Ŀ¼

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
	string Day[7] = { "������", "����һ", "���ڶ�", "������", "������", "������", "������" }; 
	string today_day = Day[sys.wDayOfWeek];	
	printf("%02d:%02d:%02d %s\nApp start!\n", sys.wHour, sys.wMinute, sys.wSecond, today_day.c_str());

	g_strAppDir = getAppDir();
	g_strAppSetIniPath = g_strAppDir + "set.ini";
	GET_LOG->logInfo(debugLog,"App start! ");
}



/*���ֽ�ת���ֽ�*/
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

/*���ֽ�ת���ֽ�*/
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
//utf8 תunicode
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
/*���ֽ�תutf-8 */
std::string WStringToUTF_8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, lpwcszWString, wcslen(lpwcszWString), NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, lpwcszWString, wcslen(lpwcszWString), pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
//utf-8 ת ascii  
std::string UTF_82ASCII(std::string& strUtf8Code)
{
	//�Ȱ� utf8 תΪ unicode  
	std::wstring wstr = Utf82Unicode(strUtf8Code);
	//���� unicode תΪ ascii  
	std::string strRet = WStringToMBytes(wstr.c_str());
	return strRet;
}

// ascii ת utf-8
std::string ASCII2UTF_8(std::string& strASCIICode)
{
	//�Ȱ� utf8 תΪ unicode  
	std::wstring wstr = MBytesToWString(strASCIICode.c_str());
	//���� unicode תΪ ascii  
	std::string strRet = WStringToUTF_8(wstr.c_str());
	return strRet;
}

std::string  G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];//��Ҫ���������������Ըĳɴ�ָ������ķ�ʽ
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	std::string strR = str;
	if (str) delete[] str;
	return strR;
}
//const char* UTF8_To_GBK(const char* source, char* pcDes)
//{
//	enum { GB2312 = 936 };
//	//unsigned long len;
//	int len = -1;
//	len = ::MultiByteToWideChar(CP_UTF8, NULL, source, -1, NULL, NULL);
//	if (len == 0)
//		return NULL;
//	wchar_t *wide_char_buffer = new wchar_t[len];
//	::MultiByteToWideChar(CP_UTF8, NULL, source, -1, wide_char_buffer, len);
//	len = ::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, NULL, NULL, NULL, NULL);
//	if (len == 0)
//	{
//		delete[] wide_char_buffer;
//		return NULL;
//	}
//	char *multi_byte_buffer = new char[len];
//	::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, multi_byte_buffer, len, NULL, NULL);
//	strcpy_s(pcDes, multi_byte_buffer);
//	//std::string dest(multi_byte_buffer);
//	delete[] multi_byte_buffer;
//	delete[] wide_char_buffer;
//	return pcDes;
//}