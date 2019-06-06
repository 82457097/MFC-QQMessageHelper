#include "stdafx.h"
#include "QQMessage.h"


QQMessage::QQMessage() { 
}

QQMessage::~QQMessage() { 
}

//Ϊ�����ļ���ʼ����
void QQMessage::Start(const wchar_t *msg) {
	//������Ҫ���͵���Ϣ
	m_msg = CString(msg);
	//����һ���߳�
	_beginthreadex(NULL, 0, QQMessage::runThreadProc, NULL, 0,NULL);
}

//�����߳� �̵߳����
unsigned int QQMessage::runThreadProc(void *parm) {
	QQMessage::Instance()->sendAll();	//������������
	return 0;
}

//���͵�����
void QQMessage::sendAll() {
	addToClipBoard();
	int errorTime = 0;
	int flag = 0;

	while (errorTime <= 4) {
		//1.�ҵ����� QQȺ���� YYȺ���� �һ����...
		HWND qqhWnd = FindWindow(L"TXGuiFoundation", L"QQ"); //ͨ������Spy++�ҵ�QQ�Ĵ��� ������һ�����
		//2.�ҵ���QQ�����ǲ������������ǰ��
		HWND front_hWnd = GetForegroundWindow();	//�õ���ǰ��ǰ��Ĵ���

		if (qqhWnd != front_hWnd) {
			if (!SetForegroundWindow(qqhWnd)) {
				//���������ǰ ��Ҫ��QQ���ڷŵ���ǰ��
				MessageBox(NULL, L"�Ҳ���QQ���ڣ�", NULL, NULL);
				break;
			}
		}
		if (errorTime == 0) {
			//ģ�ⰴ�� keybd_event();
			pressKey(VK_TAB);
			pressKey(VK_TAB);
		}
		pressKey(VK_DOWN); 
		pressKey(VK_RETURN);

		HWND curFrontWnd = GetForegroundWindow();
		if (curFrontWnd == qqhWnd) {
			++errorTime;
		}
		errorTime = 0;
		if(flag)
			sendMsg();
		flag++;
	}
}

//ģ�ⰴ��
void QQMessage::pressKey(BYTE vkey) {
	//ģ�ⰴ�� keybd_event();
	keybd_event(vkey, 0, 0, 0);	//���°���
	Sleep(100);
	keybd_event(vkey, 0, KEYEVENTF_KEYUP, 0);	//���𰴼�
	Sleep(100);
}

//������Ϣ
void QQMessage::sendMsg() { //���ڲ�����ϢȺ�����������������£�����
	//����Ctrl+v
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(100);
	keybd_event(0x56, 0, 0, 0);
	Sleep(100);

	//����ctrl+v
	keybd_event(0x56, 0, KEYEVENTF_KEYUP, 0);
	Sleep(100);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(100);

	//���͹ر�
	pressKey(VK_RETURN);
	pressKey(VK_ESCAPE);
}

//��Ҫ����Ϣ������������
void QQMessage::addToClipBoard() {
	//�ڴ濽��
	int memLenth = m_msg.GetLength() * sizeof(wchar_t) + 2;
	HANDLE hGlobalMemery = GlobalAlloc(GHND, memLenth);
	void *memAddr = GlobalLock(hGlobalMemery);
	ZeroMemory(memAddr, memLenth);
	memcpy(memAddr, (const void*)m_msg.GetBuffer(), memLenth);
	GlobalUnlock(hGlobalMemery);

	//�������������
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::SetClipboardData(CF_UNICODETEXT,hGlobalMemery);
	::CloseClipboard();
}