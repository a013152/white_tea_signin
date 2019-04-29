#pragma once
#include <time.h> 

#define GET_LOG CLog::getInstance()

enum logType{
	infoLog,	//��ͨ
	debugLog,  //����
	errorLog   //������Ϣ
};

class CLog
{
	CLog();
	void detectPath(const char* path_);
	SYSTEMTIME stCurrentTime  ;
	//SYSTEMTIME stPreTime ;
	FILE *fp = NULL;  //�ļ����
public:
	static CLog* getInstance();

	void logInfo(logType type, const char* format, ...);

	~CLog();
};

