#pragma once
#include "cMyTimer.h"
#include "./include/curl/curl.h"
#pragma comment(lib,"./lib/libcurl.lib")    
#include "cppJson/json/json.h"

#define GET_CURL CCurl::getInstance()
typedef struct stResultJson
{
	int iStatus;  //状态
	string strInfo; //
	Json::Value jsonVauleData;
	string strData;
	string strRequestUrl;
	void clearMem(){
		iStatus = 0; strInfo = ""; jsonVauleData = NULL; strData = ""; strRequestUrl = "";
	}
}ST_RESULT_JSON;



//处理网络接口http请求与解析返回


class CCurl :public cMyTimer
{
	CCurl();

	stResultJson stCurrentResult;  //当前返回的json
	char recBuffer[MAX8192] ;//curl接收响应时内存容器。
	size_t recIndex ;		//curl接收响应时内存游标。
	int aotuLoginInterval;  //定时器：自动登陆
	string strAccount;		//调用php登陆接口需要使用//读取set.ini文件获取
	string strPassword;		//调用php登陆接口需要使用//读取set.ini文件获取
	char szToken[MAX1024];   //调用php接口需要使用token  //定时登陆获取
	int processType;		//调用php获取流程接口需要使用type，  //读取set.ini文件获取
	int shopID ;			//调用php签到接口需要使用append_shops_id，  //定时登陆获取
	
	int processID ;			//调用php签到接口需要使用shop_processes_id，  //读取set.ini文件获取
	int deviceID;			//调用php签到接口需要使用camera_devices_id，  //读取set.ini文件获取
	int signinId;			//调用php签到接口需要使用shop_plants_id	//读取set.ini文件获取
	string strCapturePath;  //调用php签到接口需要使用path[]    //调用php抓拍接口获取。 
	string strCameraSerial;  //调用php抓拍接口需要使用serial    //读取set.ini文件获取
	string strWatermark;	 //调用php抓拍接口需要使用watermark    //读取set.ini文件获取
	string strWatermark2;	 //调用php抓拍接口需要使用watermark    //读取set.ini文件获取
	
private:
	CURL *curl = NULL;  //curl 对象指针

private:
	//定时器函数
	virtual int OnTimer(int id, int iParam = 0, string str = "");
	//curl 处理函数 API, 返回0执行成功
	int curlAPI(stResultJson* result_, const char* szUrl, const char* szPostField, const char* szToken_ = NULL);  


	

	//解析结果
	void parseJsonResult(const char* strData, stResultJson* result_);

	void autoLogin();  //自动登陆，获取token， 用于定时器响应

	

public:
	
	//pthis调用接口

	size_t callBack(void *ptr, size_t size, size_t nmemb, void *stream);  //curl 响应回调函数 内存赋值

	int captureRequest(); //抓拍请求

	//签到请求,
	//参数1 容器id（采茶证id)
	//参数2 是否发送服务器图片path(false=》不发送，接口将返回采茶证是否能签到结果)
	int signinRequest(const char* szContainerID, bool bUpload); 
public:
	//对外接口
	static CCurl* getInstance(); //获取实例

	static void freeInstance(); //释放实例

	void init();  //初始化

	~CCurl();

	static size_t callBack_(void *ptr, size_t size, size_t nmemb, void *stream);//curl 响应回调函数
	
	static void signinFunction(const char* szData);  //签到回调函数，用于nfc读卡器响应

	//curl 处理函数 API, 返回0执行成功，上传文件
	int curlAPI( stResultJson& result_, const char* szUrl, const char* szFilePath, const char* szWatermark, const char* szToken_ = NULL);

	
	
};

