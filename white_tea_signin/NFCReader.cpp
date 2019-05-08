#include "stdafx.h"
#include "NFCReader.h"
#include <minwindef.h>
#include "Log.h"
#include "PlaySound.h"



CNFCReader* pthis = nullptr;

HID_DEVICE g_hDevice = HID_DEVICE(-1);  //g_hDevice must be initialized as -1 before use

CNFCReader* CNFCReader::getInstance(){
	if (pthis == nullptr){
		pthis = new CNFCReader();
	}
	return pthis;

}

void CNFCReader::freeInstance()
{
	if (pthis){ 
		pthis->DeleteTimer(Timer_Read_NFC);
		::Sleep(TIME_INTERVEAL);
		delete pthis;
		pthis = nullptr;
	}	
}

CNFCReader::CNFCReader()
{
	//读取配置=》时间间隔
	tempInterval = GetPrivateProfileIntA("set", "scanNFCReader", 10000, g_strAppSetIniPath.c_str());
	//读取配置，获取正则表达式
	char szTemp[256] = { 0 };
	GetPrivateProfileString("regex", "URLPattern1", "[\\s\\S]*shop.tdneed.com[\\s\\S]*", szTemp, 256, g_strAppSetIniPath.c_str());
	strPattern = szTemp;
	GetPrivateProfileString("regex", "URLPattern2", "shop.tdneed.com[\\s\\S]*", szTemp, 256, g_strAppSetIniPath.c_str());
	strURLPattern2 = szTemp;
	GetPrivateProfileString("regex", "ConPattern1", "[\\s\\S]*[A-Z]-[0-9]{5}-[0-9][\\s\\S]*", szTemp, 256, g_strAppSetIniPath.c_str());
	strConPattern  = szTemp;
	GetPrivateProfileString("regex", "ConPattern2", "[A-Z]-[0-9]{5}-[0-9][\\s\\S]*", szTemp, 256, g_strAppSetIniPath.c_str());
	strConPattern2 = szTemp;
}
CNFCReader::~CNFCReader()
{
	
}
 
void CNFCReader::init()
{
	AddTimer(Timer_Read_NFC, tempInterval);
	printf("NFC读取卡器就绪!\n");
	double tempf = tempInterval / 1000.0;
	GET_LOG->logInfo(infoLog, "NFC读取卡器就绪,启动定时器%0.1f秒读取！", tempf);//%w.hf表示输出的总宽度是w小数点后保留h位

}

bool CNFCReader::openDev( bool logInfo_ /*= true*/)
{
	int status;

	//=================== Connect the reader ===================
	//Check whether the reader is connected or not
	//If the reader is already open , close it firstly
	if (FALSE != Sys_IsOpen(g_hDevice))
	{
		status = Sys_Close(&g_hDevice);
		if (status != 0)
		{
			if (logInfo_)
			{
				printf("关闭NFC读卡器失败 !\n");
				GET_LOG->logInfo(errorLog, "关闭NFC读卡器失败！");
			}
		}
	}

	//Connect
	status = Sys_Open(&g_hDevice, 0, 0x0416, 0x8020);
	if (status != 0)
	{
		if (logInfo_)
		{
			printf("打开NFC读卡器失败 !\n");
			GET_LOG->logInfo(errorLog, "打开NFC读卡器失败！");
		}
		return false;
	}


	//========== Init the reader before operating the card ==========
	//Close antenna of the reader
	status = Sys_SetAntenna(g_hDevice, 0);
	if (status != 0)
	{
		if (logInfo_)
		{
			printf("设置NFC读卡器启动探测失败 !\n");
			GET_LOG->logInfo(errorLog, "设置NFC读卡器启动探测失败！");
		}
		return false;
	}
	Sleep(5); //Appropriate delay after Sys_SetAntenna operating

	//Set the reader's working mode
	status = Sys_InitType(g_hDevice, 'A');
	if (status != 0)
	{
		if (logInfo_)
		{
			printf("初始化NFC读卡器失败 !\n");
			GET_LOG->logInfo(errorLog, "初始化NFC读卡器失败！");
		}
		return false;
	}
	Sleep(5); //Appropriate delay after Sys_InitType operating

	//Open antenna of the reader
	status = Sys_SetAntenna(g_hDevice, 1);
	if (status != 0)
	{
		if (logInfo_)
		{
			printf("设置NFC读卡器启动探测失败 !\n");
			GET_LOG->logInfo(errorLog, "设置NFC读卡器启动探测失败！");
		}
		return false;
	}
	Sleep(5); //Appropriate delay after Sys_SetAntenna operating

	//==================== Success Tips ====================
	//Beep 200 ms
	status = Sys_SetBuzzer(g_hDevice, 20);
	if (status != 0)
	{
		if (logInfo_)
		{
			printf("设置NFC读卡器蜂鸣器失败 !\n");
			GET_LOG->logInfo(errorLog, "设置NFC读卡器蜂鸣器失败！");
		}
		return false;
	}

	//Tips
	//printf("Connect reader succeed !\n");
	//if (startTimer){
		//AddTimer(Timer_Read_NFC, tempInterval);
		//printf("NFC读取卡器就绪!\n");
		//double tempf = tempInterval / 1000.0;
		//GET_LOG->logInfo(infoLog, "NFC读取卡器就绪,启动定时器%0.1f秒读取！", tempf);//%w.hf表示输出的总宽度是w小数点后保留h位
	//}
	return true;
}

std::string CNFCReader::readNfc()
{
	int status = 0;
	int i = 0;
	BYTE pageAddr = 0;
	BYTE reqMode = 0x52; //request all card
	WORD tagType = 0x0044;
	string str = "";
	BYTE length = 0;
	BYTE arryUID[7] = { 0 };
	BYTE UriIdentifierCode = 0;
	int uriFieldLen = 0;
	int len1 = 0;
	int len2 = 0;
	BYTE bytesBuffer1[200] = { 0 };
	BYTE bytesBuffer2[200] = { 0 };

	static int carPreStatus = -1;
	static BYTE carPreUID[7] = { 0 };//NFC卡片之前的状态
	static string carPreData ;

	//Check whether the reader is connected or not
	if (FALSE == Sys_IsOpen(g_hDevice))
	{
		if (openDev(false) == false)
		{
			if (carPreStatus == 196)
				return "";
			carPreStatus = 196;
			printf("打开失败，请先连接读卡器!\n");
		}
		else{
			carPreStatus = 0;
			memset(carPreUID, 0, 7);
			carPreData.clear();
		}
		return "";
	}

	//========================= Request card ===========================
	status = TyA_Request(g_hDevice, reqMode, &tagType);
	 
	if (status != 0 && status == 20){
		if (carPreStatus != status){
			carPreStatus = status;
			memset(carPreUID, 0, 7);
			carPreData.clear();
		}
		else{
			return "";
		}
	}
	if (status == 196){
		//数据传输超时
		if (carPreStatus != status){
			carPreStatus = status;
			GET_LOG->logInfo(errorLog, "访问读卡器超时，请重新连接nfc读卡器!");
			printf("访问读卡器超时，请重新连接nfc读卡器!\n", status);
			return "";
		}
		else{
			//openDev(false, false);
			g_hDevice = HID_DEVICE(-1);
			return "";
		}
		

	}
	if (status != 0)
	{
		if (status == 20)
			printf("访问读卡器成功，请放入NFC卡……!\n");
		else 
			printf("查询NFC卡失败，错误码%d!\n",status);
		return "";
	}
	carPreStatus = status;

	status = TyA_NTAG_AnticollSelect(g_hDevice, arryUID, &length);
	if (status != 0)
	{
		printf("NTAG卡防冲撞并激活失败，错误码%d !\n",status);
		return "";
	}
	else{
		//对比UID,判断是不是相同的卡片
		bool bSame = true;
		for (int i = 0; i < length; i++){
			if (arryUID[i] != carPreUID[i]){
				bSame = false;
				break;
			}
		}
		if (bSame == false){
			memcpy(carPreUID, arryUID, 7);
		}
		if (bSame == true && carPreData.empty() == false){
			
			return carPreData;
		}
	}

	//=========================== Read data ===========================

	BYTE byteLen = 4;
	for (i = 0; i < 144; i = i + 4)
	{
		pageAddr = 4 + i / 4; //begin to write from Page 4
		status = TyA_NTAG_Read(g_hDevice, pageAddr, &bytesBuffer2[i], &byteLen);
		if (status != 0)
		{
			printf("读取失败: %d 页 !\n", pageAddr);
			return "";
		}
	}


	//=========================== TLV data ===========================
	len2 = 0;
	bool flag1 = (bytesBuffer2[len2++] == 0x01); //The factory default data of NTAG203
	bool flag2 = (bytesBuffer2[len2++] == 0x03); //The factory default data of NTAG203
	bool flag3 = (bytesBuffer2[len2++] == 0xA0); //The factory default data of NTAG203
	bool flag4 = (bytesBuffer2[len2++] == 0x10); //The factory default data of NTAG203
	bool flag5 = (bytesBuffer2[len2++] == 0x44); //The factory default data of NTAG203
	bool flag6 = (bytesBuffer2[len2++] == 0x03);

	len1 = bytesBuffer2[len2++]; //长度 

	memcpy(&bytesBuffer1[0], &bytesBuffer2[len2], len1);//

	len2 = len2 + len1;

	bool flag7 = (bytesBuffer2[len2++] == 0xFE);

	//================++========= Get UID ==++==========================
	UriIdentifierCode = bytesBuffer1[4];//Get URI identifier code

	uriFieldLen = bytesBuffer1[2] - 1;//Get payload field length


	bool flag8 = (bytesBuffer1[0] == 0xD1); //SR = 1, TNF = 0x01 (NFC Forum Well Known Type), ME=1, MB=1
	bool flag9 = (bytesBuffer1[1] == 0x01); //Length of the Record Type (1 byte)
	bool flag10 = (bytesBuffer1[2] == 1 + uriFieldLen); //Length of the payload(identifier code length + URI field length)
	bool flag11 = (bytesBuffer1[3] == 0x55); //The URI record type (“U”)
	bool flag12 = (bytesBuffer1[4] == UriIdentifierCode);

	//判断类型 Url / Txt
	int indexPayload = 0;
	if (0x55 == bytesBuffer1[3]){
		//url 逻辑
		indexPayload = 5;

	}
	else if (0x54 == bytesBuffer1[3]){
		//txt
		indexPayload = 5;
	}
	else if(0x54 == bytesBuffer1[4]){
		//txt
		indexPayload = 7;
	}

	if (uriFieldLen != -1){
		BYTE bytePayload[256] = { 0 };
		memcpy(bytePayload, &bytesBuffer1[indexPayload], uriFieldLen);
		int index1 = 0;
		while (bytePayload[index1] == 0x0)
		{
			index1++;
		}

		str += string((char*)&bytePayload[index1]);		
	}	
	//========================== Success Tips ==========================
	printf("读取到%s\n请拿开nfc卡\n",str.c_str());
	Sys_SetBuzzer(g_hDevice, 10); //Beep 100ms	
	//========================== do logic ==========================		
	Sleep(110);
	carPreData = str;
	GET_LOG->logInfo(infoLog, "读取到nfc信息：%s", str.c_str());
	doAnalyzeNFC(str);
	return str;
}

int CNFCReader::OnTimer(int id, int iParam, string str)
{
	switch (id)
	{
	case Timer_Read_NFC:
		readNfc();
		break;
	default:
		break;
	}
	return 1; //返回1 该定时器不会推出
}

void CNFCReader::doAnalyzeNFC(const string inputStr)
{
	// 播放提示音 开始签到请稍等
	GET_PLAYS->addPlay(CPlaySound::begin);
	//使用正则表达式判断
	std::regex regex_(strConPattern2.c_str(), regex::icase);
	
	std::smatch matchResult;
	string strTag;

	//正则匹配
	if (std::regex_search(inputStr, matchResult, regex_))
	{ 		 
		for (size_t i = 0; i < matchResult.size(); ++i)
		{
			strTag =  matchResult[i];
		}
	}
	//是否匹配
	if (strTag.empty() == false){
		if (m_pFun != nullptr){
			m_pFun(strTag.c_str());
		}
	}
	else{
		// 播放提示音 不是有效的采茶证件
		GET_PLAYS->addPlay(CPlaySound::fail2_invalid);
	} 
}
