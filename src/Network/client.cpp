#include "stdafx.h"
#include "client.h"

Client::Client()
{
	databuffer = NULL;
	mvdcsend = NULL;
	payloadlength = 0;
	login_flag = 1;
	m_connect = 1;
	m_mvdcpack = new MVDCPacket(0,0);
	InitializeCriticalSection(&g_cs);

}

Client::~Client()
{
	delete m_mvdcpack;
	DeleteCriticalSection(&g_cs);
	closesocket(sock);
	WSACleanup();
}

void Client::tcpinitial(char* host)
{
	//char *host ="localhost";
	char *service="3333";

	WSADATA wsadata;

	if(WSAStartup(WAVERS, &wsadata)!=0)
	{
		printf("initalize failed\n");
		WSACleanup();
		exit(1);
	}

	tcpserver(host,service);
	return;
}

void Client::tcpserver(const char *host, const char *service)
{
	const char *transport = "tcp";
	struct protoent *protoin;
	struct sockaddr_in ipaddr;
	struct hostent *hostin;
	struct servent *servin;
	int type;

	memset(&ipaddr,0,sizeof(ipaddr));
	ipaddr.sin_family=AF_INET;

	if(servin=getservbyname(service,transport))
		ipaddr.sin_port=servin->s_port;
	else if((ipaddr.sin_port=htons((u_short)atoi(service)))==0)
	{
		printf("get server information error\n");
		WSACleanup();
		exit(1);
	}

	if((protoin=getprotobyname(transport))==0)
	{
		printf("get protocol information error \n");
		WSACleanup();
		exit(1);
	}

	if(hostin=gethostbyname(host))
		memcpy (&ipaddr.sin_addr,hostin->h_addr,hostin->h_length);
	else if ((ipaddr.sin_addr.s_addr=inet_addr(host))==INADDR_NONE)
	{
		printf("get host IP information error\n");
		WSACleanup();
		exit(1);
	}


	if (strcmp(transport,"udp")==0)
		type=SOCK_DGRAM;
	else
		type=SOCK_STREAM;

	sock=socket(AF_INET,type,protoin->p_proto);

	if(sock==INVALID_SOCKET)
	{
		printf("creat socket error\n");
		WSACleanup();
		exit(1);
	}

	int nErrCode;
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	nErrCode = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&uiRcvBuf,&uiRcvBufLen);
	if(SOCKET_ERROR == nErrCode)
	{
		WSACleanup();
		exit(1);
	}
	uiRcvBuf *= BUF_TIMES;
	nErrCode = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&uiRcvBuf,uiRcvBufLen);
	if(SOCKET_ERROR == nErrCode)
	{
		WSACleanup();
		exit(1);
	}

	unsigned int uiNewRcvBuf;
	nErrCode = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&uiNewRcvBuf, &uiRcvBufLen);
	if(SOCKET_ERROR == nErrCode  || uiNewRcvBuf != uiRcvBuf)
	{
		WSACleanup();
		exit(1);
	}

	if(connect(sock,(struct sockaddr *)&ipaddr, sizeof(ipaddr))==SOCKET_ERROR)
	{
		printf("connect socket error! please start server first\n");
		WSACleanup();
		exit(1);
	}
	return;
}

int Client::regist(CString username, CString password, CString &videolist)
{

	FILE *fpt = fopen("clientID.txt","r+");
	if(fpt == NULL)
	{
		printf("Cann't open clientID.txt \n");
		return 0;
	}
	int i;
	fscanf(fpt,"%d",&i);//判断文件中ID号是否为0，如果不为0，则已经注册过;如果为0，则需要注册
	if(i==0)
		rewind(fpt);//将文件指针移动到文件的开头，以便写入ID号时，覆盖掉“0”
	else
	{
		fclose(fpt);
		printf("U have registed, U just need to login! \n");
		return 0;
	}
	//*********************************************
	//发送注册信息：username  rpassword
	int m_length = username.GetLength()+password.GetLength()+2;
	mvdcsend = new BYTE[m_length + 8];

	int buf_length,poffset;
	buf_length = username.GetLength()+1;
	char *buf = new char[buf_length];
	::wcstombs(buf,username.GetBuffer(),username.GetLength());
	buf[username.GetLength()] = '\0';;
	memcpy(mvdcsend+8,buf,buf_length);
	delete []buf;
	buf = NULL;

	poffset = buf_length +8;
	buf_length = password.GetLength()+1;
	buf = new char[buf_length];
	::wcstombs(buf,password.GetBuffer(),password.GetLength());
	buf[password.GetLength()] = '\0';
	memcpy(mvdcsend+poffset,buf,buf_length);
	delete []buf;
	buf = NULL;

	poffset = poffset + buf_length;
	m_mvdcpack->setmvdc(poffset,re_Register,0,1);
	bufferpack();
	send(sock,(char *)mvdcsend,poffset,0);
	delete []mvdcsend;
	mvdcsend = NULL;
	//*********************************************
	//接收注册是否成功的信号
	recv(sock,(char *)header,8,0);
	m_mvdcpack->resetmvdc(header);
	if ( m_mvdcpack->GetIdentifier() == 5)
	{
		printf("regist fail \n");
		return 0;
	}
	else
		if ( m_mvdcpack->GetIdentifier() == 4)
			printf("regist success \n");
	//**********************************************
	//接收注册获得的ID号
	recv(sock,(char *)header,8,0);
	m_mvdcpack->resetmvdc(header);
	payloadlength = m_mvdcpack->GetPacketsize()-8;
	databuffer=new BYTE[payloadlength];
	recv(sock,(char*)databuffer,payloadlength,0);
	i = atoi((char*)databuffer);
	if((i<=999999)&&(i>=100000))
	{
		fprintf(fpt,"%d",i);
		fclose(fpt);
	}
	else
	{
		printf("U get a wrong ID");
		fclose(fpt);
		return 0;
	}
	delete []databuffer;
	//*****************************************************
	//接受视频列表
	rec_videolist();
	videolist = (const unsigned char*)databuffer;
	delete []databuffer;
	databuffer = NULL;
	login_flag = 0;
	return 0;
}
int Client::login(CString username, CString password ,CString &videolist)
{	
	char buf[7];
	FILE *fpt = fopen("clientID.txt","r");
	if(fpt == NULL)
	{
		printf("Cann't open clientID.txt \n");
		return 0;
	}
	int i;
	fscanf(fpt,"%d",&i);//判断文件中ID号是否为0，如果不为0，则已经注册过;如果为0，则需要注册
	if(i==0)
	{
		printf("U don't have ID, U need regist first  \n");
		fclose(fpt);
		return 0;
	}
	else
	{
		itoa(i,buf,10);
		fclose(fpt);
	}

	int m_length = username.GetLength()+password.GetLength()+2;
	mvdcsend = new BYTE[m_length+15];
	m_mvdcpack->setmvdc(m_length+15,re_Login,0,1);
	bufferpack();
	memcpy(mvdcsend+8,buf,7);

	int buf_length,poffset;
	buf_length = username.GetLength()+1;
	char *sendbuf = new char[buf_length];
	::wcstombs(sendbuf,username.GetBuffer(),username.GetLength());
	sendbuf[username.GetLength()] = '\0';;
	memcpy(mvdcsend+15,sendbuf,buf_length);
	delete []sendbuf;
	sendbuf = NULL;

	poffset = buf_length +15;
	buf_length = password.GetLength()+1;
	sendbuf = new char[buf_length];
	::wcstombs(sendbuf,password.GetBuffer(),password.GetLength());
	sendbuf[password.GetLength()] = '\0';
	memcpy(mvdcsend+poffset,sendbuf,buf_length);
	delete []sendbuf;
	sendbuf = NULL;

	send(sock,(char *)mvdcsend,m_length+15,0);
	delete []mvdcsend;
	mvdcsend = NULL;
	//**************************************
	//接收到login是否成功的信号
	recv(sock,(char *)header,8,0);
	m_mvdcpack->resetmvdc(header);
	if ( m_mvdcpack->GetIdentifier() == 5)
	{
		printf("login fail \n");
		return 0;
	}
	else
		if ( m_mvdcpack->GetIdentifier() == 4)
			printf("regist success \n");
	//**************************************
	//接收视频列表
	rec_videolist();
	videolist = (const unsigned char*)databuffer;
	delete []databuffer;
	databuffer = NULL;
	login_flag = 0;
	return 0;
}

void Client::getvideo(CString videoname, CString savepath)
{
	savepath += videoname;//////////////////////
	int namelength = videoname.GetLength()+1;
	payloadlength = namelength +8; 
	char *videobuf = new char[namelength];
	::wcstombs(videobuf,videoname.GetBuffer(),videoname.GetLength());
	videobuf[namelength-1] = '\0';

	mvdcsend = new BYTE[payloadlength];
	m_mvdcpack->setmvdc(payloadlength,re_GetVideo,0,1);
	bufferpack();
	memcpy(mvdcsend+8,videobuf,namelength);

	send(sock,(char *)mvdcsend,payloadlength,0);

	delete []mvdcsend;
	mvdcsend = NULL;

	int pathlength = savepath.GetLength()+1;
	char *path = new char[pathlength];
	::wcstombs(path,savepath.GetBuffer(),savepath.GetLength());
	path[pathlength-1] = '\0';
	recvfile(path);
	delete []videobuf;
	videobuf = NULL;
	delete []path;	
	path = NULL;
	return;
}

void Client::quit()
{
	mvdcsend = new BYTE[8];
	m_mvdcpack->setmvdc(38,re_Quit,0,1);
	bufferpack();
	send(sock,(char *)mvdcsend,8,0);
	m_connect = 0;
}

void Client::rec_videolist()
{
	recv(sock,(char *)header,8,0);
	m_mvdcpack->resetmvdc(header);
	payloadlength = m_mvdcpack->GetPacketsize()-8;
	databuffer=new BYTE[payloadlength+1];
	databuffer[payloadlength] = '\0';
	recv(sock,(char*)databuffer,payloadlength,0);
	//printf("%s",(char *)databuffer);
	//delete []databuffer;
}

int Client::recvfile(char *videoname)
{

	FILE *file=fopen(videoname,"wb+");
	BYTE recvbuffer[35000];

	uint16_t packetnum=0;
	BYTE *buf;
	buf=new BYTE[100];
	buf[0]=0;
	fwrite(buf,sizeof(BYTE),1,file);
	int i=0;
	long recvlength,interrecvlength;


	do
	{   		
		recvlength=recv(sock,(char *)header,8,0);
		m_mvdcpack->resetmvdc(header);

		if(m_mvdcpack->GetChannel()!=0)
		{
			payloadlength = m_mvdcpack->GetPacketsize()-8;

			recvlength=recv(sock,(char*)recvbuffer,payloadlength,0);

			while(recvlength<payloadlength)
			{
				
				interrecvlength=recv(sock,(char*)recvbuffer+recvlength,payloadlength-recvlength,0);  //2010/7/25 改过
				recvlength=recvlength+interrecvlength;
				Sleep(50);
			}

			fwrite(recvbuffer,sizeof(BYTE),payloadlength,file);

		}

	}while(m_mvdcpack->GetChannel()!=0);

	fclose(file);
	delete buf;	
	buf = NULL;
	return 0;

}


void Client::bufferpack()
{
	uint8_t Version = m_mvdcpack->GetVersion();
	uint8_t Channel = m_mvdcpack->GetChannel();
	uint32_t Packetsize = m_mvdcpack->GetPacketsize();
	uint16_t Identifier = m_mvdcpack->GetIdentifier();
	memcpy(mvdcsend,&Version,1);
	memcpy(mvdcsend+1,&Channel,1);
	memcpy(mvdcsend+2,&Packetsize,4);
	memcpy(mvdcsend+6,&Identifier,2);
}