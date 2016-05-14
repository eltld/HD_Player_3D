#include "stdafx.h"
#include "mvdcpacket.h"

MVDCPacket::MVDCPacket(uint32_t psize,uint16_t iden, uint8_t vers,uint8_t chan)
{
	version = vers;
	channel = chan;
	packetsize = psize;
	identifier = iden;
}

MVDCPacket::~MVDCPacket(){}

void MVDCPacket::setmvdc(uint32_t msize,uint16_t miden,uint8_t mchan,uint8_t mvers)
{
	version = mvers;
	channel = mchan;
	packetsize = msize;
	identifier = miden;
}
void MVDCPacket::resetmvdc(BYTE *preceive)
{
	memcpy(&(version),preceive,1);
	memcpy(&(channel),preceive+1,1);
	memcpy(&(packetsize),preceive+2,4);
	memcpy(&(identifier),preceive+6,2);
}

