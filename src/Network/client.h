#ifndef CLIENT_H
#define CLIENT_H

#include <atlstr.h>
#include "mvdcpacket.h"

#define BUF_TIMES 30
#define WAVERS MAKEWORD(2,2)

class Client
{
public:
	Client();
	virtual ~Client();

	enum MessageFlag   //channel 0�и���MVDC��ͷ�������λ���ж����͵���ʲô������Ϣ����ȷ����һ������ʽ
	{
		re_Register                    = 0,
		re_Login                       = 1,
		re_GetVideo                    = 2,
		re_Quit                        = 3,
		re_login_success               = 4,
		re_login_fail                  = 5,
	};
public:
	void   tcpinitial(char* host);	
	int    regist(CString username, CString password, CString &videolist);
	int    login(CString username, CString password, CString &videolist);
	void   getvideo(CString videoname,CString savepath);
	void   quit();
	void   rec_videolist();
	int    recvfile(char * videoname);
	void   bufferpack();
public:
	int getconnect(){ return m_connect;}
	int getflag(){return login_flag;}
private:
	void   tcpserver(const char *host, const char *service);

private:

	int login_flag;
	SOCKET sock;
	BYTE header[8];
	BYTE * databuffer;
	BYTE *mvdcsend;
	int payloadlength;
	MVDCPacket* m_mvdcpack;
	int m_connect;
	CRITICAL_SECTION   g_cs;
};
#endif




