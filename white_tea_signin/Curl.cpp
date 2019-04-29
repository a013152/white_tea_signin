#include "stdafx.h"
#include "Curl.h"

#include "cppJson/json/json.h"

CCurl* pthis = nullptr;
CCurl::CCurl() :recIndex(0), shopID(0),processID(0)
{
	memset(recBuffer, 0, MAX8192);
	memset(szToken, 0, MAX1024);
}


CCurl* CCurl::getInstance()
{
	if (pthis == nullptr){
		pthis = new CCurl();
	}
	return pthis;
}

CCurl::~CCurl()
{
}



size_t CCurl::s_CallBack(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return pthis->callBack(ptr, size, nmemb, stream);
}

size_t CCurl::callBack(void *ptr, size_t size, size_t nmemb, void *stream)
{ 
	memcpy(&recBuffer[recIndex], ptr, size*nmemb);
	recIndex += size*nmemb;
	return size*nmemb;
}

//������½��json����
void CCurl::parseJsonLogin(const std::string strData)
{
	string recJsonInfo = "", recJsonInfoANSI = "";
	int recJsonStatus = 0;


	//ʹ��cppJson����
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strData, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		if (root["status"].isNull() == false && root["status"].isInt()){
			stCurrentResult.iStatus = root["status"].asInt();
			if (stCurrentResult.iStatus == 10000){
				if (root["data"]["token"].isNull() == false && root["data"]["token"].isString()){
					strcpy_s(szToken, MAX1024, root["data"]["token"].asCString());  //��ȡtoken
				}
				if (root["data"]["shops"].isNull() == false && root["data"]["shops"].isArray()){
					for (size_t i = 0; i < root["data"]["shops"].size(); i++){
						if (root["data"]["shops"][i]["shops_id"].isNull() == false){
							shopID = root["data"]["shops"][i]["shops_id"].asInt();
							break;
						}
					}
				}
				printf("������token��%s\n������shopid��%d\n", szToken, shopID);
			}
			else{
				if (root["request_url"].isNull() == false && root["request_url"].isString()){
					stCurrentResult.strRequestUrl = root["request_url"].asString();
				}
			}
		}
		if (root["info"].isNull() == false && root["info"].isString()){
			stCurrentResult.strInfo = root["info"].asString();
			stCurrentResult.strInfo = UTF_82ASCII(stCurrentResult.strInfo);
			printf("������Info��%s\n", stCurrentResult.strInfo.c_str());
		}
		int code = root["code"].asInt();    // ���ʽڵ㣬code = 100 
	}
}



//��½
void CCurl::login01(string strAccount, string strPassword)
{
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "https://team.tdneed.com/api/login"); //ͬ���̳ǵ�¼
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	struct curl_slist *headers = NULL;
	curl_easy_setopt(curl, CURLOPT_HEADER, 0); //����Ӧͷ��Ϣ����Ӧ��һ�𴫸�write_data  
	headers = curl_slist_append(headers, "Content-type: application/json");  //����json��ʽ
	headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate"); //��ȡ���ļ�ʱ����
	headers = curl_slist_append(headers, "Accept: application/json");  //����json��ʽ
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Pragma: no-cache");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //	
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);//���ó�ʱʱ��	
	curl_easy_setopt(curl, CURLOPT_POST, 1);//����CURLOPT_POST֮��������POST����
	//��������
	char szData1[1024] = { 0 };
	sprintf_s(szData1, "{\"account\":\"%s\",\"password\":\"%s\"}", strAccount.c_str(), strPassword.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szData1);  //-data ����
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(szData1));  //-data ����
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, s_CallBack);

	recIndex = 0;
	memset(recBuffer, 0, MAX8192);

	CURLcode ret = curl_easy_perform(curl);
	if (ret != CURLE_OK)
	{
		printf("��½curlִ�д�����:%d ,������Ϣ %s\n", ret, curl_easy_strerror(ret));
	}
	else{
		printf("��½curlִ�гɹ�\n");
		parseJsonLogin(string(recBuffer));
	}
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
}
//
////�����ӿڷ��ص�json
void CCurl::parseJsonResult(const std::string strData){
	cJSON * json = cJSON_Parse(strData.c_str());
//	if (json == nullptr){
//		printf("��������002\n");
//		return;
//	}
//	//�ж�status
//	if (1 == cJSON_HasObjectItem(json, "status"))
//	{
//		cJSON *nodeStatus = cJSON_GetObjectItem(json, "status");
//		if (nodeStatus && nodeStatus->type == cJSON_Number){
//			stCurrentResult.iStatus = nodeStatus->valueint;
//		}
//	}
//	//�ж�info
//	if (1 == cJSON_HasObjectItem(json, "info")){
//		cJSON *nodeInfo = cJSON_GetObjectItem(json, "info"); //��ȡ��ʾ��Ϣ
//		if (nodeInfo && nodeInfo->type == cJSON_String){
//			stCurrentResult.strInfo = nodeInfo->valuestring;
//			stCurrentResult.strInfo = UTF_82ASCII(stCurrentResult.strInfo);
//			printf("������Info��%s\n", stCurrentResult.strInfo.c_str());
//		}
//	}
//	//�ж�status
//	if (stCurrentResult.iStatus == 10000){
//		//�ж�data
//		if (1 == cJSON_HasObjectItem(json, "data")){
//			cJSON *nodeData = cJSON_GetObjectItem(json, "data");
//			if (nodeData){
//				stCurrentResult.strData = UTF_82ASCII(string(cJSON_Print(nodeData)));
//			}
//		}
//	}
//	else{
//		//�ж�request_url
//		if (1 == cJSON_HasObjectItem(json, "request_url")){
//			cJSON *nodeRequestUrl = cJSON_GetObjectItem(json, "request_url");
//			if (nodeRequestUrl && nodeRequestUrl->type == cJSON_String){
//				stCurrentResult.strRequestUrl = nodeRequestUrl->valuestring;
//			}
//		}
//	}
//	//
//	//��JSON�ṹ��ռ�õ����ݿռ��ͷ�
//	cJSON_Delete(json);
}
//��ȡ����
void CCurl::getProcessList(){
	CURL *curl = curl_easy_init();
	struct curl_slist *headers = NULL;
	curl_easy_setopt(curl, CURLOPT_URL, "https://team.tdneed.com/api/source/query/process/list");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0); //����Ӧͷ��Ϣ����Ӧ��һ�𴫸�write_data  
	headers = curl_slist_append(headers, "Content-type: application/json");  //����json��ʽ "{\"type\":\"2\",\"append_shops_id\":\"9\"}" �����ַ�"type=2&append_shops_id=9"
	headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate"); //��ȡ���ļ�ʱ����
	headers = curl_slist_append(headers, "Accept: application/json");  //����json��ʽ
	headers = curl_slist_append(headers, "Cache-Control: no-cache");
	headers = curl_slist_append(headers, "Pragma: no-cache");
	char szTemp[1024] = { 0 };
	sprintf_s(szTemp, "Authorization: Bearer %s", szToken);
	headers = curl_slist_append(headers, szTemp);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // 
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 0); // 
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); 
	//���ó�ʱʱ��
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
	//����CURLOPT_POST֮��������POST����
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	//��������
	char szData1[MAX1024] = { 0 };
	sprintf_s(szData1, "{\"type\":\"2\",\"append_shops_id\":\"%d\"}", shopID);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szData1);  //-data ����
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(szData1));  //-data ����
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, s_CallBack);
	recIndex = 0;
	memset(recBuffer, 0, MAX8192);
	CURLcode ret = curl_easy_perform(curl);
	if (ret != CURLE_OK)
	{
		printf("��ȡ����curlִ�д�����:%d ,������Ϣ %s\n", ret, curl_easy_strerror(ret));
	}
	else{
		printf("��ȡ����curlִ�гɹ�\n");
		parseJsonResult(string(recBuffer));
		if (stCurrentResult.strData.empty() == false)
			printf("��ȡ����data:%s\n", stCurrentResult.strData.c_str());
	}
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
}
