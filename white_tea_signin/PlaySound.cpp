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

//��ȡ�����ļ�����Ƶ·��
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

//�߳�ѭ��
void CPlaySound::playLoop()
{
	ResetEvent(m_hShutdownEvent);//�����˳����¼����ź�
	DWORD dwEvent = 0;
	while (playLoopFlag == true)
	{
		//�ȴ��¼��� 0�رգ� 1��ȡ
		dwEvent = WaitForMultipleObjects(2, m_hEventArray, FALSE, INFINITE);
		Sleep(10);
		switch (dwEvent)
		{
			//�ر��¼�
		case 0:
		{
				  GET_LOG->logInfo(infoLog, "�����������Ź������߳��˳���");
				  return;
		}
			//�����¼�
		case 1:
		{
				  string strPath;
				  /** �����ٽ�� */
				  EnterCriticalSection(&m_csSync);
				   if (vtPlayTask.size() > 0){
					  for (vector<string>::iterator it = vtPlayTask.begin(); it != vtPlayTask.end();){
						  strPath = *it;
						  //	SND_FILENAME��ʾpszSound����ָ�������ļ�����pszSound������ָ����Դ���ڴ����֡�ϵͳ���ֵȵȣ���
						  //	SND_ASYNC�����첽��ʽ����������PlaySound�����ڿ�ʼ���ź��������أ�
						  //	SND_SYNC��ͬ�������������ڲ������PlaySound�����ŷ��أ�
						  //	SND_LOOPһ�����ظ�����������������SND_ASYNC��־һ��ʹ�á�
						  if (PlaySound(strPath.c_str(), NULL, SND_FILENAME | SND_SYNC)){  //ͬ������
							  vtPlayTask.erase(it);  //����������Ƴ�����
						  }
						  it = vtPlayTask.begin();
					  }
				  }
				  /** �뿪�ٽ�� */
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

		/** �ȴ��߳��˳� */
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
		/** �����ٽ�� */
		EnterCriticalSection(&m_csSync);
		vtPlayTask.push_back(strPath);
		/** �뿪�ٽ�� */
		LeaveCriticalSection(&m_csSync);
		SetEvent(m_hPlayEvent);
	}
}

void CPlaySound::init()
{

}
