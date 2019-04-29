#pragma once
#include "./include/curl/curl.h"
#pragma comment(lib,"./lib/libcurl.lib")    

//处理网络接口http请求与解析返回
class CCurl
{
	CCurl();
public:
	static CCurl* getInstance();
	
	~CCurl();
};

