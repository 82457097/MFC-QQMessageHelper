#pragma once
class QQMessage {
public:
	QQMessage();
	~QQMessage();
	//Ϊ�����дһ�����
	static QQMessage *Instance() {
		static QQMessage object;	//���ɶ���
		return &object;
	}
	//Ϊ�����ļ���ʼ����
	void Start(const wchar_t *msg);

private:
	//�����߳�
	static unsigned int WINAPI runThreadProc(void *parm);
	//���͵�����
	void sendAll();
	//ģ�ⰴ��
	void pressKey(BYTE vkey);

	//������Ϣ
	void sendMsg();

	//��Ҫ����Ϣ������������
	void addToClipBoard();

	CString m_msg;

};

