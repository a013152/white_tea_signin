#include "stdafx.h"
#include "Curl.h"


#include "Log.h"
#include "PlaySound.h"

CCurl* pthis = nullptr;
CCurl::CCurl() :recIndex(0), shopID(0), processID(0), processType(0)
{
	memset(recBuffer, 0, MAX8192);
	memset(szToken, 0, MAX1024);

	//��ȡ����=��ʱ����
	aotuLoginInterval = GetPrivateProfileIntA("set", "autoLoginInterval", 300000, g_strAppSetIniPath.c_str());
	 

	AddTimer(Timer_Auto_login, aotuLoginInterval, 0, 0);

	curl = curl_easy_init();
}


CCurl* CCurl::getInstance()
{	
	if (pthis == nullptr){
		pthis = new CCurl();
	}
	return pthis;
}

void CCurl::freeInstance()
{
	if (pthis){		
		delete pthis;
		pthis = nullptr;
	}
}

CCurl::~CCurl()
{
	if (curl){
		curl_easy_cleanup(curl);
	}
} 

size_t CCurl::callBack_(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return pthis->callBack(ptr, size, nmemb, stream);
}

size_t CCurl::callBack(void *ptr, size_t size, size_t nmemb, void *stream)
{ 
	memcpy(&recBuffer[recIndex], ptr, size*nmemb);
	recIndex += size*nmemb;
	return size*nmemb;
}
//��ȡ������
int CCurl::curlAPI(stResultJson* result_, const char* szUrl, const char* szPostField, const char* szToken_ /*= NULL*/)
{
	int returnValue = 1;
	if (curl == NULL)
		return returnValue;

	curl_easy_setopt(curl, CURLOPT_URL, szUrl); //url
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);  //�������
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	struct curl_slist *headers = NULL;
	curl_easy_setopt(curl, CURLOPT_HEADER, 0); //����Ӧͷ��Ϣ����Ӧ��һ�𴫸�write_data  
	headers = curl_slist_append(headers, "Content-Type:application/json");
	//headers = curl_slist_append(headers, "Content-type: application/x-www-form-urlencoded;charset=UTF-8");  //��json��ʽ����post ����
	headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate"); //��ȡ���ļ�ʱ����
	headers = curl_slist_append(headers, "Accept: application/json");  //����json��ʽ
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Pragma: no-cache");
	if (szToken_ != NULL)
	{
		char szTemp[1024] = { 0 };
		sprintf_s(szTemp, "Authorization: Bearer %s", szToken);   //����token
		headers = curl_slist_append(headers, szTemp);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //	
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);//���ó�ʱʱ��	
	curl_easy_setopt(curl, CURLOPT_POST, 1);//����CURLOPT_POST֮��������POST����
	//�������� 
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPostField);  //-data ����
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(szPostField));  //-data ����
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBack_);

	//struct curl_httppost *post = NULL;
	//struct curl_httppost *last = NULL;	
	//curl_formadd(&post, &last, 
	//	CURLFORM_COPYNAME, "image",                     //�˴���ʾҪ���Ĳ�����
	//	CURLFORM_FILE, "/tmp/ab/face.jpg",                //�˴���ʾͼƬ�ļ���·��	
	//	//CURLFORM_CONTENTTYPE, "image/jpeg",
	//	CURLFORM_END);
	//curl_formadd(&post, &last, 
	//	CURLFORM_COPYNAME, "body",                           //�˴�Ϊ��Ĳ���
	//	CURLFORM_COPYCONTENTS, szJsonData,             //Ҫ�ϴ���json�ַ���	
	//	CURLFORM_END				);
		


	recIndex = 0;
	memset(recBuffer, 0, MAX8192);

	CURLcode ret = curl_easy_perform(curl);
	if (ret != CURLE_OK)
	{
		GET_LOG->logInfo(errorLog,"9-100 curlAPIִ�д�����:%d ,������Ϣ %s", ret, curl_easy_strerror(ret));
		printf("curlִ�д�����:%d ,������Ϣ %s\n", ret, curl_easy_strerror(ret));
		returnValue = 2;
	}
	else{
		printf("curlִ�гɹ�\t");
		parseJsonResult(recBuffer, result_);
		returnValue = 0;
	}
	curl_slist_free_all(headers);

	return returnValue;
	
}
//�ϴ���
int CCurl::curlAPI( stResultJson& result_, const char* szUrl, const char* szFilePath, const char* szWatermark, const char* szToken_ /*= NULL*/)
{
	int returnValue = 1;
	if (curl == NULL)
		return returnValue;
	curl_easy_setopt(curl, CURLOPT_URL, szUrl); //url
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);  //�������
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	struct curl_slist *headers = NULL;
	curl_easy_setopt(curl, CURLOPT_HEADER, 0); //����Ӧͷ��Ϣ����Ӧ��һ�𴫸�write_data  
	//headers = curl_slist_append(headers, "Content-Type:application/form-data");
	////headers = curl_slist_append(headers, "Content-type: application/x-www-form-urlencoded;charset=UTF-8");  //��json��ʽ����post ����
	headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate"); //��ȡ���ļ�ʱ����
	headers = curl_slist_append(headers, "Accept: application/json");  //����json��ʽ
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Pragma: no-cache");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBack_);

	//if (szToken_ != NULL)
	//{
	//	char szTemp[1024] = { 0 };
	//	sprintf_s(szTemp, "Authorization: Bearer %s", szToken);   //����token
	//	headers = curl_slist_append(headers, szTemp);
	//}
	//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //	
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);//���ó�ʱʱ��	

	//�����ϴ��ļ� 
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "file",                     //�˴���ʾҪ���Ĳ�����	
		CURLFORM_FILE, szFilePath,                //�˴���ʾͼƬ�ļ���·��
		CURLFORM_CONTENTTYPE, "image/png",
		//CURLFORM_CONTENTTYPE, "image/jpg",
		CURLFORM_END);

	curl_formadd(&post, &last,
		//CURLFORM_COPYNAME, "watermark",                           //�˴�Ϊ��Ĳ���
		//CURLFORM_COPYCONTENTS, watermark,             //Ҫ�ϴ���json�ַ���
		//CURLFORM_CONTENTLEN, strlen(watermark),
		CURLFORM_PTRNAME, "watermark",
		CURLFORM_PTRCONTENTS, szWatermark,
		CURLFORM_END);

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);


	recIndex = 0;
	memset(recBuffer, 0, MAX8192);

	CURLcode ret = curl_easy_perform(curl);
	if (ret != CURLE_OK)
	{
		GET_LOG->logInfo(errorLog, "8-100 curlAPI�ϴ�ִ�д�����:%d ,������Ϣ %s, �ļ�·��%s", ret, curl_easy_strerror(ret), szFilePath);
		printf("curlִ�д�����:%d ,������Ϣ %s\n", ret, curl_easy_strerror(ret));
		returnValue = 2;
	}
	else{
		printf("curlִ�гɹ�\t");
		parseJsonResult(recBuffer, &result_);
		returnValue = 0;
	}
	curl_slist_free_all(headers);

	return returnValue;
}
 

void CCurl::autoLogin()
{
	//login01(strAccount,strPassword);
	stResultJson stJson; stJson.clearMem();
	char szUrl[MAX1024] = { 0 };
	GetPrivateProfileString("url", "login", "https://team.tdneed.com/api/login", szUrl, MAX1024, g_strAppSetIniPath.c_str());
	char szTemp[256] = { 0 };
	GetPrivateProfileString("set", "account", "wmcy01", szTemp, 256, g_strAppSetIniPath.c_str());
	strAccount = szTemp;
	GetPrivateProfileString("set", "password", "123456", szTemp, 256, g_strAppSetIniPath.c_str());
	strPassword = szTemp;
	char szPostField[MAX1024] = { 0 };
	sprintf_s(szPostField, "{\"account\":\"%s\",\"password\":\"%s\"}", strAccount.c_str(), strPassword.c_str());
	//sprintf_s(szPostField, "&account=%s&password=%s", strAccount.c_str(), strPassword.c_str());
	int returnValue = curlAPI(&stJson, szUrl, szPostField);
	if (returnValue == 0){ 
		if ( false == stJson.jsonVauleData.isNull())  // reader��Json�ַ���������data_Json�� 
		{
			if (stJson.iStatus == 10000){
				if (stJson.jsonVauleData["token"].isNull() == false && stJson.jsonVauleData["token"].isString()){
					strcpy_s(szToken, MAX1024, stJson.jsonVauleData["token"].asCString());  //��ȡtoken
				}
				if (stJson.jsonVauleData["shops"].isNull() == false && stJson.jsonVauleData["shops"].isArray()){
					for (size_t i = 0; i < stJson.jsonVauleData["shops"].size(); i++){
						if (stJson.jsonVauleData["shops"][i]["shops_id"].isNull() == false){
							shopID = stJson.jsonVauleData["shops"][i]["shops_id"].asInt();
							break;
						}
					}
					//��ȡ���� �б�
					stJson.clearMem();
					GetPrivateProfileString("url", "process", "https://team.tdneed.com/api/source/query/process/list", szUrl, MAX1024, g_strAppSetIniPath.c_str());
					processType = GetPrivateProfileInt("set", "process_type", 3, g_strAppSetIniPath.c_str());
					sprintf_s(szPostField, "{\"type\":\"%d\",\"append_shops_id\":\"%d\"}", processType, shopID);
					returnValue = curlAPI(&stJson, szUrl, szPostField, szToken);
					if (returnValue == 0){
						bool flag = false;
						if (false == stJson.jsonVauleData.isNull())  // reader��Json�ַ���������data_Json�� 
						{
							if (stJson.iStatus == 10000){
								if (stJson.jsonVauleData.isNull() == false && stJson.jsonVauleData.isArray()){
									if (stJson.jsonVauleData.size() >= 1){
										size_t i = 0;
										if (stJson.jsonVauleData[i].isObject()&& stJson.jsonVauleData[i]["id"].isInt()){
											processID = stJson.jsonVauleData[i]["id"].asInt();
											flag = true;
										}
									}									
								}
							}
						}
						if (flag == false){
							printf("��ȡ����δ�ܽ���,��Ϣ\n");
						}

					}
				}
				printf("�Զ���½�ɹ���������shopid��%d, processid:%d��token��%s\n", shopID, processID, szToken);
				GET_LOG->logInfo(debugLog, "�Զ���½�ɹ���������shopid��%d processid:%d", shopID, processID);
			}
			else{
				printf("�Զ���½ʧ�ܣ�%d, info��%s\n", stJson.iStatus, stJson.strInfo.c_str());
				GET_PLAYS->addPlay(CPlaySound::fail7_autoLogin); //�Զ���½ʧ�ܣ������쳣
			} 
		} 
	}
	else{
		printf("11-100ִ��curlAPI����%d\n", returnValue);
		GET_PLAYS->addPlay(CPlaySound::fail6_autoLogin); //�Զ���½ʧ�ܣ���鿴��־������
	}
}

int CCurl::captureRequest()
{
	stResultJson stJson; stJson.clearMem();
	char szUrl[MAX1024] = { 0 };
	GetPrivateProfileString("url", "capture", "http://172.16.0.196/api/camera/capture", szUrl, MAX1024, g_strAppSetIniPath.c_str());
	char szTemp[256] = { 0 };
	GetPrivateProfileString("set", "cameraSerial", "C88801031", szTemp, 256, g_strAppSetIniPath.c_str());
	strCameraSerial = szTemp;
	GetPrivateProfileString("set", "waterMark", "", szTemp, 256, g_strAppSetIniPath.c_str());
	strWatermark = szTemp;
	GetPrivateProfileString("set", "waterMark2", "", szTemp, 256, g_strAppSetIniPath.c_str());
	strWatermark2 = szTemp;
	char szPostField[MAX1024] = { 0 };
	//sprintf_s(szPostField, "{\"serial\":\"%s\",\"watermark\":\"%s%s\"}", strCameraSerial.c_str(), G2U(strWatermark.c_str()).c_str(), G2U(strWatermark2.c_str()).c_str());
	sprintf_s(szPostField, "{\"serial\":\"%s\",\"watermark\":\"%s\\n%s\"}", strCameraSerial.c_str(), G2U(strWatermark.c_str()).c_str(), G2U(strWatermark2.c_str()).c_str());
	int returnValue = curlAPI(&stJson, szUrl, szPostField);
	if (returnValue == 0){
		if (stJson.iStatus == 10000){
			if (stJson.jsonVauleData["path"].isNull() == false && stJson.jsonVauleData["path"].isString()){
				strCapturePath = stJson.jsonVauleData["path"].asCString();				
			}
			if (stJson.jsonVauleData["url"].isNull() == false && stJson.jsonVauleData["url"].isString()){
				printf("ץ�ĳɹ���������url��%s\n", stJson.jsonVauleData["url"].asCString());
			}
		}
		else{
			returnValue = stJson.iStatus;
			printf("ץ��ʧ�ܣ�%d, info��%s\n", stJson.iStatus, stJson.strInfo.c_str());
			GET_LOG->logInfo(errorLog, "12-101ץ��ʧ�ܣ�%d, info��%s", stJson.iStatus, stJson.strInfo.c_str());
		}
	}
	else{
		printf("12-100ִ��curlAPI����%d\n", returnValue);
		GET_LOG->logInfo(errorLog, "12-100 captureRequestִ��curlAPI����%d", returnValue);
	}
	return returnValue;
}

int CCurl::signinRequest(const char* szContainerID)
{
	stResultJson stJson; stJson.clearMem();
	char szUrl[MAX1024] = { 0 };
	GetPrivateProfileString("url", "signin", "https://team.tdneed.com/api/source/signIn", szUrl, MAX1024, g_strAppSetIniPath.c_str());
	
	signinId = GetPrivateProfileInt("set", "signin_id", 1,  g_strAppSetIniPath.c_str());
	 
	deviceID = GetPrivateProfileInt("set", "device_id", 1, g_strAppSetIniPath.c_str());
	  
	char szPostField[MAX1024] = { 0 }; 
	/*sprintf_s(szPostField, "{\"container_no\":[\"%s\"],\"shop_plants_id\":\"%d\",\"append_shops_id\":\"%d\",\"camera_devices_id\":\"%d\",\"shop_processes_id\":\"%d\",\"path\":[\"%s\"]}",\
						   szContainerID, signinId, shopID, deviceID, processID, strCapturePath.c_str());*/
	//�ȹ���һ����д������Ȼ���Json�������л����ַ������������£�
	Json::FastWriter jsonWriter;
	Json::Value wValue;
	Json::Value container_no;   // ������������
	Json::Value path_;   // ����path����   	
	container_no.append(szContainerID); 
	wValue["container_no"] = container_no;
	wValue["shop_plants_id"] = signinId;
	wValue["append_shops_id"] = shopID;
	wValue["camera_devices_id"] = deviceID;
	wValue["shop_processes_id"] = processID;
	path_.append(strCapturePath);
	wValue["path"] = path_;
	std::string strWrite = jsonWriter.write(wValue);
	sprintf_s(szPostField, "%s", strWrite.c_str());

	int returnValue = curlAPI(&stJson, szUrl, szPostField, szToken);
	if (returnValue == 0){
		if (stJson.iStatus == 10000){
			printf("ǩ���ɹ���info=��%s\n", stJson.strInfo.c_str());
			returnValue = 0;
		} 
		else{
			returnValue = stJson.iStatus;
			printf("ǩ��ʧ�ܣ�%d, info��%s\n", stJson.iStatus, stJson.strInfo.c_str());
			GET_LOG->logInfo(errorLog, "13-101ǩ��ʧ�ܣ�%d, info��%s\t����%s", stJson.iStatus, stJson.strInfo.c_str(), szPostField);
		}
	}
	else{
		printf("13-100ִ��curlAPI����%d\n", returnValue);
		GET_LOG->logInfo(errorLog, "13-100 captureRequestִ��curlAPI����%d", returnValue);
	}
	return returnValue;
}

//
////�����ӿڷ��ص�json
void CCurl::parseJsonResult(const char* strData, stResultJson* result_){
	unsigned int len1 = strlen(strData);
	if (strcmp(strData, "[]") == 0 || len1 == 0){
		//GET_LOG->logInfo(debugLog, "10-104����json����,û���ҵ�info:%s", strData);
		printf("10-104����json����,�ַ�������:%s", strData);
		return;
	}
	else if (len1 >= MAX8192){
		std::string  strTemp = strData;
		strTemp = strTemp.substr(0, 64);
		//�ַ�����
		printf("�������ַ����ȳ���8k:%s", strTemp.c_str());
		GET_LOG->logInfo(errorLog, "�������ַ����ȳ���8k:%s...", strTemp.c_str());
		return;

	}
	//ʹ��cppJson����
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strData, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		if (root["status"].isNull() == false && root["status"].isInt()){
			result_->iStatus = root["status"].asInt();
			if (result_->iStatus == 10000  ){
				if (false == root["data"].isNull() /*&& root["data"].isObject()*/)
					result_->jsonVauleData = root["data"];				
			}
			else{
				if (root["request_url"].isNull() == false && root["request_url"].isString()){
					result_->strRequestUrl = root["request_url"].asString();
				}
				/*else{
					GET_LOG->logInfo(errorLog, "10-103����json����,û���ҵ�request_url:%s", strData);
					printf("10-103����json����,û���ҵ�request_url:%s", strData);
				}*/
			}
		}
		else{
			GET_LOG->logInfo(errorLog, "10-101����json����,û���ҵ�Status:%s", strData);
			printf("10-101����json����,û���ҵ�Status:%s", strData);
		}
		if (root["info"].isNull() == false && root["info"].isString()){
			result_->strInfo = root["info"].asString();
			result_->strInfo = UTF_82ASCII(result_->strInfo);
			printf("������Info��%s\n", result_->strInfo.c_str());
		}
		else{
			GET_LOG->logInfo(errorLog, "10-102����json����,û���ҵ�info:%s", strData);
			printf("10-102����json����,û���ҵ�info:%s", strData);
		}
	}
	else{
		GET_LOG->logInfo(errorLog, "10-100����json����,str:%s", strData);
		printf("10-100����json����,str:%s", strData);
	}
}

void CCurl::signinFunction(const char* szData)
{
	int returnValue = 0;
	printf("Curl��ִ�лص�����=��ǩ����data:%s\n", szData);
	returnValue = pthis->captureRequest();
	if (returnValue == 0)
	{
		returnValue = pthis->signinRequest(szData);
		if (returnValue == 0)
		{
			GET_PLAYS->addPlay(CPlaySound::end); //ǩ�����
		}
		else if (returnValue == 40001){
			GET_PLAYS->addPlay(CPlaySound::fail5_already_signed); //�Ѿ�ǩ��
		}
		else{
			GET_PLAYS->addPlay(CPlaySound::fail1_exception); //�쳣
		}
	} 
	else{
		GET_PLAYS->addPlay(CPlaySound::fail1_exception); //�쳣
	}
	
}

int CCurl::OnTimer(int id, int iParam /*= 0*/, string str /*= ""*/)
{
	switch (id)
	{
	case Timer_Auto_login:
		autoLogin();
		break;
	default:
		break;
	}
	return 1; //����1 �ö�ʱ�������Ƴ�
}

void CCurl::init()
{
	autoLogin();
}

