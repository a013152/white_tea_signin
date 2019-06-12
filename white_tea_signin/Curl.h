#pragma once
#include "cMyTimer.h"
#include "./include/curl/curl.h"
#pragma comment(lib,"./lib/libcurl.lib")    
#include "cppJson/json/json.h"

#define GET_CURL CCurl::getInstance()
typedef struct stResultJson
{
	int iStatus;  //״̬
	string strInfo; //
	Json::Value jsonVauleData;
	string strData;
	string strRequestUrl;
	void clearMem(){
		iStatus = 0; strInfo = ""; jsonVauleData = NULL; strData = ""; strRequestUrl = "";
	}
}ST_RESULT_JSON;



//��������ӿ�http�������������


class CCurl :public cMyTimer
{
	CCurl();

	stResultJson stCurrentResult;  //��ǰ���ص�json
	char recBuffer[MAX8192] ;//curl������Ӧʱ�ڴ�������
	size_t recIndex ;		//curl������Ӧʱ�ڴ��αꡣ
	int aotuLoginInterval;  //��ʱ�����Զ���½
	string strAccount;		//����php��½�ӿ���Ҫʹ��//��ȡset.ini�ļ���ȡ
	string strPassword;		//����php��½�ӿ���Ҫʹ��//��ȡset.ini�ļ���ȡ
	char szToken[MAX1024];   //����php�ӿ���Ҫʹ��token  //��ʱ��½��ȡ
	int processType;		//����php��ȡ���̽ӿ���Ҫʹ��type��  //��ȡset.ini�ļ���ȡ
	int shopID ;			//����phpǩ���ӿ���Ҫʹ��append_shops_id��  //��ʱ��½��ȡ
	
	int processID ;			//����phpǩ���ӿ���Ҫʹ��shop_processes_id��  //��ȡset.ini�ļ���ȡ
	int deviceID;			//����phpǩ���ӿ���Ҫʹ��camera_devices_id��  //��ȡset.ini�ļ���ȡ
	int signinId;			//����phpǩ���ӿ���Ҫʹ��shop_plants_id	//��ȡset.ini�ļ���ȡ
	string strCapturePath;  //����phpǩ���ӿ���Ҫʹ��path[]    //����phpץ�Ľӿڻ�ȡ�� 
	string strCameraSerial;  //����phpץ�Ľӿ���Ҫʹ��serial    //��ȡset.ini�ļ���ȡ
	string strWatermark;	 //����phpץ�Ľӿ���Ҫʹ��watermark    //��ȡset.ini�ļ���ȡ
	string strWatermark2;	 //����phpץ�Ľӿ���Ҫʹ��watermark    //��ȡset.ini�ļ���ȡ
	
private:
	CURL *curl = NULL;  //curl ����ָ��

private:
	//��ʱ������
	virtual int OnTimer(int id, int iParam = 0, string str = "");
	//curl ������ API, ����0ִ�гɹ�
	int curlAPI(stResultJson* result_, const char* szUrl, const char* szPostField, const char* szToken_ = NULL);  


	

	//�������
	void parseJsonResult(const char* strData, stResultJson* result_);

	void autoLogin();  //�Զ���½����ȡtoken�� ���ڶ�ʱ����Ӧ

	

public:
	
	//pthis���ýӿ�

	size_t callBack(void *ptr, size_t size, size_t nmemb, void *stream);  //curl ��Ӧ�ص����� �ڴ渳ֵ

	int captureRequest(); //ץ������

	//ǩ������,
	//����1 ����id���ɲ�֤id)
	//����2 �Ƿ��ͷ�����ͼƬpath(false=�������ͣ��ӿڽ����زɲ�֤�Ƿ���ǩ�����)
	int signinRequest(const char* szContainerID, bool bUpload); 
public:
	//����ӿ�
	static CCurl* getInstance(); //��ȡʵ��

	static void freeInstance(); //�ͷ�ʵ��

	void init();  //��ʼ��

	~CCurl();

	static size_t callBack_(void *ptr, size_t size, size_t nmemb, void *stream);//curl ��Ӧ�ص�����
	
	static void signinFunction(const char* szData);  //ǩ���ص�����������nfc��������Ӧ

	//curl ������ API, ����0ִ�гɹ����ϴ��ļ�
	int curlAPI( stResultJson& result_, const char* szUrl, const char* szFilePath, const char* szWatermark, const char* szToken_ = NULL);

	
	
};

