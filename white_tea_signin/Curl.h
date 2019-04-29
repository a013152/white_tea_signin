#pragma once
#include "./include/curl/curl.h"
#pragma comment(lib,"./lib/libcurl.lib")    


typedef struct stResultJson
{
	int iStatus;  //状态
	string strInfo; //
	string strData;
	string strRequestUrl;
	void clearMem(){
		iStatus = 0; strInfo = ""; strData = ""; strRequestUrl = "";
	}
}ST_RESULT_JSON;



//处理网络接口http请求与解析返回


class CCurl
{
	CCurl();

	stResultJson stCurrentResult;  //当前返回的json
	char recBuffer[MAX8192] ;
	size_t recIndex ;
	char szToken[MAX1024];
	int shopID ;
	int processID ;
public:
	static CCurl* getInstance();
	
	~CCurl();
	static size_t s_CallBack(void *ptr, size_t size, size_t nmemb, void *stream);
	size_t callBack(void *ptr, size_t size, size_t nmemb, void *stream);

	void login01(string strAccount, string strPassword);
	void getProcessList();
	void parseJsonLogin(const std::string strData);
	void parseJsonResult(const std::string strData);
};

