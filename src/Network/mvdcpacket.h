#ifndef MVDCPACKET_H

#define MVDCPACKET_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#ifndef INTYPES_DEFINED

#define INTYPES_DEFINED

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#endif


using namespace std;

class MVDCPacket
{

public:
	MVDCPacket(uint32_t psize,uint16_t iden,uint8_t vers=1,uint8_t chan=0);
	~MVDCPacket();
	uint8_t  GetVersion()      {return version;}
	uint8_t GetChannel()        {return channel;}
	uint32_t GetPacketsize()       {return packetsize;}
	uint16_t GetIdentifier()      {return identifier;}
	void setmvdc(uint32_t msize,uint16_t miden,uint8_t mchan,uint8_t mvers);
	void resetmvdc( BYTE *preceive);

	
	
private:
	uint8_t version;
	uint8_t channel;
	uint32_t packetsize;
	uint16_t identifier;
};

#endif