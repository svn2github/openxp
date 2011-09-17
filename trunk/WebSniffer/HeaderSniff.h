#ifndef __HEADERSINFF__H__
#define __HEADERSINFF__H__
#pragma once
#include <afxsock.h>
#include <Iphlpapi.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")



#define UDP_HEAD_LEN		8			//UDP头长度
#define PSEUDO_HEAD_LEN		12			//Pseudo头长度
#define ICMP_HEAD_LEN		4			//ICMP头长度


#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST      _WSAIOW(IOC_VENDOR,2)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)
#define SIO_ABSORB_RTRALERT   _WSAIOW(IOC_VENDOR,5)
#define SIO_UCAST_IF          _WSAIOW(IOC_VENDOR,6)
#define SIO_LIMIT_BROADCASTS  _WSAIOW(IOC_VENDOR,7)
#define SIO_INDEX_BIND        _WSAIOW(IOC_VENDOR,8)
#define SIO_INDEX_MCASTIF     _WSAIOW(IOC_VENDOR,9)
#define SIO_INDEX_ADD_MCAST   _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST   _WSAIOW(IOC_VENDOR,11)

typedef struct tagPROTN2T
{ 
	int  nProto;
	char *pProtoText;
}PROTN2T; 


//IP头
typedef struct tagIPHEADER
{
	unsigned char  header_len:4;
	unsigned char  version:4;   
	unsigned char  tos;            // type of service
	unsigned short total_len;      // length of the packet
	unsigned short ident;          // unique identifier
	unsigned short flags;          
	unsigned char  ttl;            
	unsigned char  proto;          // protocol ( IP , TCP, UDP etc)
	unsigned short checksum;       
	unsigned int   sourceIP;
	unsigned int   destIP;
}IPHEADER;

struct TCPPacketHead 
{
	WORD SourPort;
	WORD DestPort;
	DWORD SeqNo;
	DWORD AckNo;
	BYTE HLen;
	BYTE Flag;
	WORD WndSize;
	WORD ChkSum;
	WORD UrgPtr;
};

struct ICMPPacketHead 
{
	BYTE Type;
	BYTE Code;
	WORD ChkSum;
};

struct UDPPacketHead 
{
	WORD SourPort;
	WORD DestPort;
	WORD Len;
	WORD ChkSum;
};


#define PROTO_NUM  11 
const PROTN2T aOfProto[PROTO_NUM+1]= 
{  
	{IPPROTO_IP  , "IP"  },
	{IPPROTO_ICMP, "ICMP"},  
	{IPPROTO_IGMP, "IGMP"}, 
	{IPPROTO_GGP , "GGP" },  
	{IPPROTO_TCP , "TCP" },  
	{IPPROTO_PUP , "PUP" },  
	{IPPROTO_UDP , "UDP" },  
	{IPPROTO_IDP , "IDP" },  
	{IPPROTO_ND  , "NP"  },  
	{IPPROTO_RAW , "RAW" },  
	{IPPROTO_MAX , "MAX" },
	{NULL		 , ""    } 
};  

static char* GetProtoName(BYTE byProto)
{
	for(int i = 0; i < PROTO_NUM; i++)
	{
		if(aOfProto[i].nProto == byProto)
		{
			return aOfProto[i].pProtoText;
		}	
	}
	return aOfProto[PROTO_NUM].pProtoText;
}

#endif