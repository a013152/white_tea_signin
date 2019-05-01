#pragma once
#include <time.h> 

#define GET_LOG CLog::getInstance()

enum logType{
	infoLog,	//普通
	debugLog,  //调试
	errorLog   //错误信息
};

class CLog
{
	CLog();
	void detectPath(const char* path_);
	SYSTEMTIME stCurrentTime  ;
	//SYSTEMTIME stPreTime ;
	FILE *fp = NULL;  //文件句柄
	string strLog[10];
public:
	static CLog* getInstance();
	static void  freeInstance();
	void logInfo(logType type, const char* format, ...);

	~CLog();
};

