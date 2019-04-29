// white_tea_signin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include "NFCReader.h" 
#include "Log.h"
std::string g_strAppDir;
std::string g_strAppSetIniPath;


int _tmain(int argc, _TCHAR* argv[])
{
	char szLogSumb[256] = { 0 }; WORD wHour = 10, wMinute = 16, wSecond = 18;
	sprintf_s(szLogSumb, "[debug]:%s\n", "App start");
	sprintf_s(szLogSumb, "%02d[debug]:%s\n", wHour, "App start");
	try
	{
		sprintf_s(szLogSumb, "%02d%:02d[debug]:%s\n", wHour, wMinute, "App start");
	}
	catch (exception e)
	{
		printf("%s\n", e.what());
	}
	
	sprintf_s(szLogSumb, "%02d:0%2d:%02d[debug]:%s\n", wHour, wMinute, wSecond, "App start");
	
	//initSys();
	
	////开启NFC读卡器
	if (GET_NFC->openDev()){		 
	//	
	}
	// 
	//printf("path:%s\n", getAppDir().c_str());

	getchar();
	GET_NFC->freeInstance();
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
	printf("%02d:%02d:%02d.%03d 星期%1d\n", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);

	g_strAppDir = getAppDir();
	g_strAppSetIniPath = g_strAppDir + "set.ini";
	GET_LOG->logInfo(debugLog,"App start! ");
}
