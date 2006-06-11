//$Id: Dispatch.cpp 3263 2006-06-09 18:05:20Z eds1275 $
#include <pci/Pci.h>
#include "Net.h"
#include "NE2000.h"
#include "MonAMDpcn.h"
#include "LoopBack.h"
#include "Dispatch.h"

using namespace mones;
using namespace MonAPI;

word ConnectionInfo::checksum(byte *data,word size)
{
    dword sum=0;
    for(int i=0;i<=size-2;i+=2){
        sum+=bswap(*(word*)(data+i));
    }
    if(size%2==1){
        sum+=bswap(*(data+size-1)&0xFF);
    }
    return ~(((sum>>16)+sum)&0xFFFF);
}

void ConnectionInfo::CreateIPHeader(Ether* frame,word length,byte protocol)
{
    IP* ip=frame->IPHeader;
    ip->verhead=0x45;       //version & headersize
    ip->tos=0x00;           //tos
    ip->len=bswap(length);  //totallength PING size windows 60 solaris 84
    ip->id=bswap(0xabcd);   //made from PID?
    ip->frag=bswap(0x0000); //flag
    ip->ttl=0x80;           //TTL
    ip->prot=protocol;      
    ip->chksum=0x0;         //for calculating checksum, it should be zero.  
    ip->chksum=bswap(checksum((byte*)ip,(ip->verhead&0x0F)<<2));
}
////////////////////////////////////////////////
void ICMPCoInfo::CreateHeader(Ether* frame,byte* data, word size)
{
    ICMP* icmp=frame->IPHeader->ICMPHeader;
    icmp->type=type;
    icmp->code=0x00;
    icmp->chksum=0x0000;
    icmp->idnum=bswap(idnum);
    icmp->seqnum=bswap(seqnum);    
    memcpy(icmp->data,data,size);
    icmp->chksum=bswap(checksum((byte*)icmp,size+sizeof(ICMP)));
    CreateIPHeader(frame,size+sizeof(ICMP)+sizeof(IP),TYPEICMP);
}

int ICMPCoInfo::Strip(Ether* frame, byte** data)
{
   *data=frame->IPHeader->ICMPHeader->data;
   return bswap(frame->IPHeader->len)-sizeof(IP)-sizeof(ICMP);
}

bool ICMPCoInfo::IsMyPacket(Ether* frame)
{
    if( TYPEICMP  == frame->IPHeader->prot ){
        if( WellKnownSVCreply(frame) ){
            return true;
        }
        ICMP* icmp=frame->IPHeader->ICMPHeader;
        if( ECHOREPLY == icmp->type  &&
            remoteip  == frame->IPHeader->srcip &&
            idnum     == bswap(icmp->idnum) &&
            seqnum    == bswap(icmp->seqnum) )
        {
            return true;
        }
    }
    return false;
}

bool ICMPCoInfo::WellKnownSVCreply(Ether* frame)
{
    ICMP* icmp=frame->IPHeader->ICMPHeader;
    if( ECHOREQUEST==icmp->type ){
        ICMPCoInfo info(dispatcher);
        info.remoteip=frame->IPHeader->srcip;
        info.type=ECHOREPLY;
        info.idnum=bswap(icmp->idnum);
        info.seqnum=bswap(icmp->seqnum);
        dispatcher->Send(icmp->data,bswap(frame->IPHeader->len)-sizeof(IP)-sizeof(ICMP),&info);
        return true;
    }
    return false;
}
/////////////////////////////////////////////////
void UDPCoInfo::CreateHeader(Ether* frame,byte* data, word size)
{
    IP* ip=frame->IPHeader; //for psedo header
    ip->ttl =0x00;
    ip->prot=TYPEUDP;
    ip->chksum=bswap(size+sizeof(UDP));
    /////////
    UDP* udp=frame->IPHeader->UDPHeader;
    udp->srcport=bswap(localport);
    udp->dstport=bswap(remoteport);
    udp->len=bswap(size+sizeof(UDP));
    udp->chksum=0x0000;
    memcpy(udp->data,data,size);
    udp->chksum=bswap(checksum((byte*)&(ip->ttl),size+sizeof(UDP)+12));
    CreateIPHeader(frame,size+sizeof(UDP)+sizeof(IP),TYPEUDP);
}

int UDPCoInfo::Strip(Ether* frame, byte** data)
{   
    *data=frame->IPHeader->UDPHeader->data;
    return bswap(frame->IPHeader->len)-sizeof(IP)-sizeof(UDP);
}

bool UDPCoInfo::IsMyPacket(Ether* frame)
{
    if( TYPEUDP    == frame->IPHeader->prot ){
        if( WellKnownSVCreply(frame) ){
            return true;
        }
        if( remoteip   == frame->IPHeader->srcip &&
            localport  == bswap(frame->IPHeader->UDPHeader->dstport) &&
            remoteport == bswap(frame->IPHeader->UDPHeader->srcport) )
        {
            return true;
        }
    }
    return false;
}

bool UDPCoInfo::WellKnownSVCreply(Ether* frame)
{   
    if( frame->IPHeader->UDPHeader->dstport==bswap(DAYTIME)){
        remoteip=frame->IPHeader->srcip;
        localport=DAYTIME;
        remoteport=bswap(frame->IPHeader->UDPHeader->srcport);
        char* data="I can't see a clock.";    
        dispatcher->Send((byte*)data,20,this);
        return true;
    }
    return false;
}

///////////////////////////////////////////////////

int TCPCoInfo::Strip(Ether* frame, byte** data)
{
    *data=frame->IPHeader->TCPHeader->data;
    return bswap(frame->IPHeader->len)-sizeof(IP)-sizeof(TCP);
}

bool TCPCoInfo::IsMyPacket(Ether* frame)
{
    if( TYPETCP    == frame->IPHeader->prot){
        if( HandShakePASV(frame) ){
            return false;
        }else if( WellKnownSVCreply(frame) ){
            return true;
        }else if( remoteip   == frame->IPHeader->srcip &&
                  localport  == bswap(frame->IPHeader->TCPHeader->dstport) &&
                  remoteport == bswap(frame->IPHeader->TCPHeader->srcport) )
        {
            return true;
        }
    }
    return false;
}

bool TCPCoInfo::WellKnownSVCreply(Ether* frame)
{    
    if( frame->IPHeader->UDPHeader->dstport==bswap(DAYTIME)){
        remoteip=frame->IPHeader->srcip;
        localport=DAYTIME;
        remoteport=bswap(frame->IPHeader->UDPHeader->srcport);
        char* data="I can't see a clock.";    
        dispatcher->Send((byte*)data,20,this);
        return true;
    }
    return false;
}

void TCPCoInfo::CreateHeader(Ether* frame,byte* data, word size)
{      
    HandShakeACTV(frame);
    IP* ip=frame->IPHeader; //for psedo header
    ip->ttl =0x00;
    ip->prot=TYPETCP;
    ip->chksum=bswap(size+sizeof(TCP));
    /////////
    TCP* tcp=frame->IPHeader->TCPHeader;
    tcp->srcport=bswap(localport);
    tcp->dstport=bswap(remoteport);
    tcp->seqnumber=seqnum;
    tcp->acknumber=acknum;
    tcp->offset=0xF0 & ((size+sizeof(TCP))<<2);
    tcp->flags=0x3F&flags;   
    tcp->window=bswap(window);
    tcp->chksum=0x0000;
    tcp->urgent=0;
    tcp->option=0;
    memcpy(tcp->data,data,size);
    tcp->chksum=bswap(checksum((byte*)&(ip->ttl),size+sizeof(TCP)+12));
    CreateIPHeader(frame,size+sizeof(TCP)+sizeof(IP),TYPETCP);
    printf("CreateHeader\n");
}

//   ~ESTABLISHED -> LISTNING   @timerAPI   ==
bool TCPCoInfo::HandShakeACTV(Ether* frame)
{  
    printf("ACTV %d\n",status);
    if(status==LISTENING){    
        seqnum=bswapl(1);
        acknum=bswapl(1);
        status=SYN_SENT; 
        flags=SYN;
        window=0;
        dispatcher->Send(NULL,0,this);
        return true;
    }
    if(status==SYN_RCVD){
        return true;
    }
    if(status==SYN_SENT){
        //status=ESTABLISHED;
        return true;
    }
    //LISTNING   -> SYN_SENT               send SYN
    //SYN_RCVD   -> SYN_RCVD               send SYN+ACK
    //SYN_SENT   -> ESTABLISHED            send ACK

    //CLOSE_WAIT -> LAST_ACK               send FIN
    //TIME_WAIT  -> TIME_WAIT              send ACK
    //ESTABLISHED->ESTABLISHED             send -
    return false;
}

bool TCPCoInfo::HandShakePASV(Ether* frame)
{    
    printf("PASV %d\n",status);
    if( status == SYN_SENT){
        seqnum=bswapl(1);
        acknum=bswapl(1);
        status=SYN_SENT; 
        flags=ACK;
        window=10;
        dispatcher->Send(NULL,0,this);
        return true;
    }   
    //CLOSE=LISTING ( reading or not reading)
    //LISTENING  -> SYN_RCVD               rcv SYN  (send SYN+ACK)
    //SYN_SENT   -> SYN_SENT               rcv SYN+ACK (send ACK)
    //SYN_RCVD   -> ESTABLISHED            rcv ACK

    //ESTABLISHED-> CLOSE_WAIT             rcv FIN
    //FIN_WAIT2  -> TIME_WAIT              rcv ACK
    //LAST_ACK   -> LISTING                rcv ACK
    //ESTABLISHED-> ESTABLISHED            rcv -
    return false;
}

void TCPCoInfo::Close()
{ 
    printf("TCPCloseSec.\n");
    //ESTABLISHED  -> FIN_WAIT1 
}
