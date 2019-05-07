#include "stdafx.h"
#include "PlaySound.h"
#include <io.h>
#include "process.h"
#include "Log.h"

CPlaySound* pthis = NULL;
CPlaySound::CPlaySound() :playLoopFlag(true) 
{
	readSoundPath();
	InitializeCriticalSection(&m_csSync);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hPlayEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventArray[0] = m_hShutdownEvent;
	m_hEventArray[1] = m_hPlayEvent;

	_beginthread(CPlaySound::playLoop_, 0, 0);

}

//读取配置文件中音频路径
void CPlaySound::readSoundPath()
{
	vtSoundPath.clear();
	int num = GetPrivateProfileIntA("sound" ,"num" ,0,g_strAppSetIniPath.c_str());
	char szKeyName[256] = { 0 }, szTemp[256] = {0};
	for (int i = 1; i <= num; i++){
		sprintf_s(szKeyName, "path%d", i);
		GetPrivateProfileString("sound", szKeyName, "", szTemp, 256, g_strAppSetIniPath.c_str());
		vtSoundPath.push_back(getAppDir()+string(szTemp));
	}
	GetPrivateProfileString("set", "waterMark", "", szTemp, 256, g_strAppSetIniPath.c_str());
}

//线程循环
void CPlaySound::playLoop()
{
	ResetEvent(m_hShutdownEvent);//设置退出读事件无信号
	DWORD dwEvent = 0;
	while (playLoopFlag == true)
	{
		//等待事件： 0关闭， 1读取
		dwEvent = WaitForMultipleObjects(2, m_hEventArray, FALSE, INFINITE);
		Sleep(10);
		switch (dwEvent)
		{
			//关闭事件
		case 0:
		{
				  GET_LOG->logInfo(infoLog, "正常结束播放工作，线程退出。");
				  return;
		}
			//播放事件
		case 1:
		{
				  string strPath;
				  /** 进入临界段 */
				  EnterCriticalSection(&m_csSync);
				   if (vtPlayTask.size() > 0){
					  for (vector<string>::iterator it = vtPlayTask.begin(); it != vtPlayTask.end();){
						  strPath = *it;
						  //	SND_FILENAME表示pszSound参数指定的是文件名（pszSound还可以指定资源、内存音乐、系统音乐等等）；
						  //	SND_ASYNC：用异步方式播放声音，PlaySound函数在开始播放后立即返回；
						  //	SND_SYNC：同步播放声音，在播放完后PlaySound函数才返回；
						  //	SND_LOOP一遍遍的重复播放声音，必须与SND_ASYNC标志一块使用。
						  if (PlaySound(strPath.c_str(), NULL, SND_FILENAME | SND_SYNC)){  //同步播放
							  vtPlayTask.erase(it);  //播放完成则移出队列
						  }
						  it = vtPlayTask.begin();
					  }
				  }
				  /** 离开临界段 */
				  LeaveCriticalSection(&m_csSync);
				
				  ::Sleep(50);
				  break;
		}
		}
		
	}  
}

CPlaySound* CPlaySound::getInstance()
{
	if (pthis == NULL)
	{
		pthis = new CPlaySound();
	}
	return pthis;
}

void CPlaySound::freeInstance()
{
	if (pthis){
		SetEvent(pthis->getShutdownEvent());

		/** 等待线程退出 */
		::Sleep(TIME_INTERVEAL);
		
		delete pthis;
	}
}

void CPlaySound::playLoop_(void* p)
{
	pthis->playLoop();
}

CPlaySound::~CPlaySound()
{
	DeleteCriticalSection(&m_csSync);
	playLoopFlag = false;
	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = NULL;
	}
	if (m_hPlayEvent)
	{
		CloseHandle(m_hPlayEvent);
		m_hPlayEvent = NULL;
	}
}

void CPlaySound::addPlay(soundType type_)
{
	string strPath = vtSoundPath[type_];
	if (_access(strPath.c_str(), 0) !=-1){ 
		/** 进入临界段 */
		EnterCriticalSection(&m_csSync);
		vtPlayTask.push_back(strPath);
		/** 离开临界段 */
		LeaveCriticalSection(&m_csSync);
		SetEvent(m_hPlayEvent);
	}
}

void CPlaySound::init()
{

}
