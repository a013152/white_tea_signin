// white_tea_signin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include "NFCReader.h" 
#include "Log.h"
#include "Curl.h"
#include "PlaySound.h"
#include <iostream>
#include <io.h>
std::string g_strAppDir;
std::string g_strAppSetIniPath;

void testFun();
int _tmain(int argc, _TCHAR* argv[])
{	
	//初始化
	initSys();
	
	//test
	GET_CURL->init();
	testFun();
	getchar();
	return 0;

	//初始化NFC读卡器,设置定时读取
	GET_NFC->init();
	//设置NFC响应回调=》签到函数
	GET_NFC->setCallbackFun(CCurl::signinFunction);

	//开启NFC读卡器
	if (GET_NFC->openDev(true))
	{		 
		GET_CURL->init();
		GET_PLAYS->init();	  
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
/*宽字节转utf-8 */
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
//utf-8 转 ascii  
std::string UTF_82ASCII(std::string& strUtf8Code)
{
	//先把 utf8 转为 unicode  
	std::wstring wstr = Utf82Unicode(strUtf8Code);
	//最后把 unicode 转为 ascii  
	std::string strRet = WStringToMBytes(wstr.c_str());
	return strRet;
}

// ascii 转 utf-8
std::string ASCII2UTF_8(std::string& strASCIICode)
{
	//先把 utf8 转为 unicode  
	std::wstring wstr = MBytesToWString(strASCIICode.c_str());
	//最后把 unicode 转为 ascii  
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
	char* str = new char[len + 1];//需要在外面析构，可以改成传指针进来的方式
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

void testFun(){
	stResultJson stJson; stJson.clearMem();	
	char  szPostField[1024] = { 0 };

	//先构建一个填写参数，然后把Json对象序列化成字符串，代码如下：
	Json::FastWriter jsonWriter;
	Json::Value wValue;
	Json::Value path_;   // 构建容器数组
	Json::Value data_1;  
	Json::Value data_2;  
	//
	//wValue["shop_plants_id"] = szSigninId;
	wValue["append_shops_id"] = "9";
	wValue["camera_devices_id"] = "0";
	wValue["brands_id"] = "1";
	wValue["url"] = "https://shop.tdneed.com/#/source/detail?code=0aee8080c78bbed2f2884b1853c93cbe";
	path_.append("phone/P5hbSTuqocLN3eRyu1AifWVf8P6ROg2Mvk4PfRKX?watermark/2/text/MjAxOS0wMy0wNiAxNjozNzozMCDnuqzluqY6MjMuMDMyOTc0ODcxNDAwNjkg57uP5bqmOjExMy4xMzUwNDEwMDAzMjQ0NSDpq5jluqY6LTE5LjcyMjQ2OTMyOTgzMzk4NCDljLrln5_vvJogIOacquefpQ==/font/5b6u6L2v6ZuF6buR/fontsize/600/dissolve/75/gravity/NorthWest/dx/60/dy/125/");
	wValue["path"] = path_;
	
	data_2["id"]="2";	
	data_2["value"] = "4";
	data_1.append(data_2);

	wValue["datas"] = data_1;

	std::string strWrite = jsonWriter.write(wValue);
	sprintf_s(szPostField, 1024, "%s", strWrite.c_str());

	int re = GET_CURL->curlAPI(&stJson, "http://team.tongdao.com/api/source/anti-fake/relate", szPostField, GET_CURL->getToken());
	if (re == 0 ){
		printf("茶纹录入 state:%d \t info:%s", stJson.iStatus, stJson.strInfo.c_str());
	}

	return;
	//stResultJson stJson; stJson.clearMem();
	//char szTemp[256] = { 0 }; sprintf_s(szTemp, "%s001.jpg",g_strAppDir.c_str());
	//if (_access(szTemp, 0) == -1){
	//	printf("%s is not exist!\n", szTemp);
	//	return;
	//}
	//int re = GET_CURL->curlAPI(stJson, "https://admin.tdneed.com/api/common/source/capture/upload", szTemp, "pc upload");
	//if (re == 0 && stJson.jsonVauleData.isNull() == false && stJson.jsonVauleData["url"].isNull() == false && stJson.jsonVauleData["url"].isString()){
	//	printf("url:%s", stJson.jsonVauleData["url"].asCString());

	//}
	//return;

	//char *szPostField = "{\"watermark\":\"test watermark\"}";
	//char *watermark = "pc";
	//char *url = "https://admin.tdneed.com/api/common/source/capture/upload";
	//CURL *pCurl = NULL;
	//CURLcode res;
	//struct curl_slist *headerlist = NULL;
	//struct curl_httppost *post = NULL;
	//struct curl_httppost *last = NULL;
	//curl_formadd(&post, &last,
	//	CURLFORM_COPYNAME, "file",                     //此处表示要传的参数名	
	//	CURLFORM_FILE, "C:\\Users\\lenovo\\Desktop\\20171220152812132.png",                //此处表示图片文件的路径
	//	CURLFORM_CONTENTTYPE, "image/png",
	//	CURLFORM_END);
	////curl_formadd(&post, &last,
	////	CURLFORM_COPYNAME, "backImage",              //此处表示要传的参数名	
	////	CURLFORM_FILE, "tmp/ab/background.jpg",     //此处表示图片文件的路径
	////	//CURLFORM_CONTENTTYPE, "image/jpeg",					
	////	CURLFORM_END);
	//curl_formadd(&post, &last,
	//	//CURLFORM_COPYNAME, "watermark",                           //此处为别的参数
	//	//CURLFORM_COPYCONTENTS, watermark,             //要上传的json字符串
	//	//CURLFORM_CONTENTLEN, strlen(watermark),
	//	CURLFORM_PTRNAME, "watermark",
	//	CURLFORM_PTRCONTENTS, watermark,
	//	CURLFORM_END);
	//pCurl = curl_easy_init();
	//if (NULL != pCurl)
	//{
	//	//curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 5);		
	//	curl_easy_setopt(pCurl, CURLOPT_URL, url);
	//	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, true);  //检测域名
	//	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
	//	curl_easy_setopt(pCurl, CURLOPT_HTTPPOST, post);
	//	//curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, szPostField);  //-data 数据
	//	//curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, strlen(szPostField));  //-data 数据
	//	res = curl_easy_perform(pCurl);
	//	if (res != CURLE_OK)
	//	{
	//		printf("curl_easy_perform() failed，error code is:%s\n",
	//			curl_easy_strerror(res));
	//	}
	//	curl_easy_cleanup(pCurl);
	//}
}