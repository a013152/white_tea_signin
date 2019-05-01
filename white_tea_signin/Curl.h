#pragma once
#include "cMyTimer.h"
#include "./include/curl/curl.h"
#pragma comment(lib,"./lib/libcurl.lib")    

#define GET_CURL CCurl::getInstance();
typedef struct stResultJson
{
	int iStatus;  //״̬
	string strInfo; //
	string strData;
	string strRequestUrl;
	void clearMem(){
		iStatus = 0; strInfo = ""; strData = ""; strRequestUrl = "";
	}
}ST_RESULT_JSON;



//��������ӿ�http�������������


class CCurl :public cMyTimer
{
	CCurl();

	stResultJson stCurrentResult;  //��ǰ���ص�json
	char recBuffer[MAX8192] ;
	size_t recIndex ;
	char szToken[MAX1024];
	int shopID ;
	int processID ;

private:
	void autoLogin();  //�Զ���½����ȡtoken��
	void login01(string strAccount, string strPassword);
	void getProcessList();
	void parseJsonLogin(const std::string strData);
	void parseJsonResult(const std::string strData);

public:
	static CCurl* getInstance();
	static void freeInstance();

	~CCurl();
	static size_t callBack_(void *ptr, size_t size, size_t nmemb, void *stream);
	size_t callBack(void *ptr, size_t size, size_t nmemb, void *stream);

	
	static void logninFunction(const char* szData);  //��½�ص�����

	virtual int OnTimer(int id, int iParam = 0, string str = "");

	
};

