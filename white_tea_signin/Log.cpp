#include "stdafx.h"
#include "Log.h"
#include <io.h>
#include <stdexcept>


CLog *pthis = nullptr;
CLog* CLog::getInstance()
{
	if (pthis == nullptr){
		try {
			pthis = new CLog();
			// 其它代码
		}
		catch (const std::bad_alloc& e) {
			printf("创建log实例失败：%s\n", e.what());
		}
	}
	return pthis;
}

void CLog::freeInstance()
{
	if (pthis){
		delete pthis;
	}
}

CLog::CLog()
{
	strLog[infoLog] = "info";
	strLog[debugLog] = "debug";
	strLog[errorLog] = "error";
}



void CLog::logInfo(logType type_, const char* format, ...)
{
	char szLogSumb[1024] = { 0 };
	char szDstLan[1024] = { 0 };
	char szTemp[256] = { 0 };
	va_list arglist;
	va_start(arglist, format);
	vsprintf_s(szDstLan, format, arglist);
	va_end(arglist);

	GetLocalTime(&stCurrentTime);
	sprintf_s(szLogSumb, "%02d:%02d:%02d[%s]:%s\r\n", stCurrentTime.wHour, stCurrentTime.wMinute, stCurrentTime.wSecond, strLog[type_].c_str(), szDstLan); 
	
	
	sprintf_s(szTemp, "%s\\log\\%04d%02d%02d.txt", g_strAppDir.c_str(), stCurrentTime.wYear, stCurrentTime.wMonth, stCurrentTime.wDay);
	
	fopen_s(&fp, szTemp, "ab+");/*以读/写方式打开一个二进制文件，允许读或在文件末追加数据。如果不存在,会自动创建*/
	if (fp == NULL){
		detectPath(szTemp);
		fopen_s(&fp, szTemp, "ab+");
		if (fp == NULL)
			return;
	}
	fwrite(szLogSumb, strlen(szLogSumb), 1, fp);
	fclose(fp);
	fp = NULL;
	
}

CLog::~CLog()
{
}


//探索文件路径，如果不存在路径，则创建
void CLog::detectPath(const char* path_)
{
	std::string strPath = path_;
	int pos = strPath.find_last_of('\\', strPath.length());
	if (pos == -1)
		return;
	strPath = strPath.substr(0, pos);
	detectPath(strPath.c_str());
	if (_access(strPath.c_str(), 0) == -1){ //判断文件是否存在
		CreateDirectory(strPath.c_str(), NULL);
	}
}
