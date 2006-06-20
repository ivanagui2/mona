//$Id: Connection.cpp 3287 2006-06-18 07:17:51Z eds1275 $
#include "TCPInfo.h"
#include "Dispatch.h"

using namespace mones;
using namespace MonAPI;


int TCPCoInfo::Strip(Ether* frame, byte** data)
{
    *data=frame->IPHeader->TCPHeader->data;
    return bswap(frame->IPHeader->len)-sizeof(IP)-sizeof(TCP);
}

bool TCPCoInfo::IsMyPacket(Ether* frame)
{
    if( TYPETCP == frame->IPHeader->prot){ //YES! MYPACKET
        if( TransStateByPKT(frame) ){
            if( WellKnownSVCreply(frame) ){
                return true;
            }else if( remoteip   == frame->IPHeader->srcip &&
                  localport  == bswap(frame->IPHeader->TCPHeader->dstport) &&
                  remoteport == bswap(frame->IPHeader->TCPHeader->srcport) )
            {
                return true;
            }
        }
    }
    return false;
}


void TCPCoInfo::CreateHeader(Ether* frame,byte* data, word size)
{
    IP* ip=frame->IPHeader; //for psedo header
    ip->ttl =0x00;
    ip->prot=TYPETCP;
    ip->chksum=bswap(size+sizeof(TCP));
    /////////
    TCP* tcp=frame->IPHeader->TCPHeader;
    tcp->srcport=bswap(localport);
    tcp->dstport=bswap(remoteport);
    tcp->seqnumber=bswapl(seqnum);
    tcp->acknumber=bswapl(acknum);
    tcp->offset=0xF0 & ((sizeof(TCP))<<2);//options.
    tcp->flags=0x3F&flags;   
    tcp->window=bswap(window);
    tcp->chksum=0x0000;
    tcp->urgent=0;
    memcpy(tcp->data,data,size);
    tcp->chksum=bswap(checksum((byte*)&(ip->ttl),size+sizeof(TCP)+12));
    CreateIPHeader(frame,size+sizeof(TCP)+sizeof(IP),TYPETCP);
    //printf("CreateHeader%d %d\n",seqnum,acknum);
}

bool TCPCoInfo::WellKnownSVCreply(Ether* frame)
{    
    if( frame->IPHeader->UDPHeader->dstport==bswap(DAYTIME)){
        remoteip=frame->IPHeader->srcip;
        localport=DAYTIME;    
        remoteport=bswap(frame->IPHeader->TCPHeader->srcport);// srcport!!!
        seqnum=bswapl(frame->IPHeader->TCPHeader->acknumber);
        acknum=bswapl(frame->IPHeader->TCPHeader->seqnumber)+1;
        status=FIN_WAIT1;
        flags=PSH|ACK;
        window=1407;
        char* data="why don't you buy high quality Rolex replica?";    
        dispatcher->Send((byte*)data,45,this);
        return true;
    }
    return false;
}

void TCPCoInfo::Close()
{     
    if( status == ESTAB ){
        //seqnum;
        //acknum;
        status=FIN_WAIT1;
        flags=FIN|ACK;
        window=1400; 
        printf("ESTAB->FIN_WAIT1\n");
        dispatcher->Send(NULL,0,this);
    }
}

/*      CLOSED=1,
        LISTEN,
        SYN_SENT,
        SYN_RCVD,
        ESTAB,
        FIN_WAIT1,
        FIN_WAIT2,
        CLOSE_WAIT,
        LAST_ACK,
        TIME_WAIT */
//=EVENT==========STAT.TRANS.============ACTION=================WHO
  //MSGACTVOpen    CLOSED->SYN_SENT       sendSYN               @THIS
//MSGOpen         CLOISED->LISTEN        -
//MSGWrite        LISTEN->ERROR.
//MSGClose        SYN_RCVD->FIN_WAIT1    sendFIN
//MSGClose        ESTAB->FIN_WAIT1       sendFIN
  //recvSYN|ACK     SYN_SENT-> ESTAB       sendACK replyMSGOPEN @THIS
//recvSYN         SYN_SENT->SYN_RCVD     sendACK
//recvSYN         LISTEN->SYN_RCVD       sendSYN|ACK
//recvACK         SYN_RSVD->ESTAB        -
//recvFIN         ESTAB->CLOSE_WAIT      sendACK
//TRANS.CLOS_WAIT CLOSE_WAIT->LAST_ACK   sendFIN
//recvFIN|ACK     LAS_ACK->CLOSED
//recvFIN|ACK     FIN_WAIT1->FIN_WAIT2   -
//recvFIN         FIN_WAIT2->TIME_WAIT   sendACK
//recvACK         FIN_WAIT1->CLOSING     sendACK
//recvFIN|ACK     CLOSEING->TIME_WAIT    TRANS.CLOSED delay2MIN.
//MSGRead         ESTAB->ESTAB           -
//MSGWrite        ESTAB->ESTAB           -
//MSGRead         OTHERS->OTHRES         replyERROR
//MSGWrite        OTHERS->OTHERS         replyERROR
//MSGClose        OTHERS->CLOSED         -

bool TCPCoInfo::TransStateByMSG(dword msg)
{
    if( status == CLOSED  && isPasv==false && msg==MSG_NET_OPEN){
        seqnum=bswapl(1);
        acknum=bswapl(1);
        status=SYN_SENT;
        flags=SYN;    
        window=1401;
        printf("CLOSED->SYN_SENT\n");
        dispatcher->Send(NULL,0,this);
        return false;
    }
    if( status == CLOSED && isPasv==true ){
        status = LISTEN;
        printf("LISTENING\n");
    }
    if( status == ESTAB ){
        flags=NORM;
        printf("XX");
    }

    return false; //Not Dispose Packet.
    //return true //Dispose Packet.
}

bool TCPCoInfo::TransStateByPKT(Ether* frame)
{
    byte* data;
    byte rflag= frame->IPHeader->TCPHeader->flags;
    ////ACTIVE OPEN CLOSE
    //printf("%x %d\n",rflag,status);
    if( (status == SYN_SENT) && (rflag == SYN|ACK) ){
        printf("<<%d>>\n",bswap(frame->IPHeader->id));
        remoteip=frame->IPHeader->srcip;
        seqnum=bswapl(frame->IPHeader->TCPHeader->acknumber);
        acknum=bswapl(frame->IPHeader->TCPHeader->seqnumber)+1;
        status=ESTAB; 
        flags=ACK;
        window=1402;
        printf("SYN_SENT->ESTAB");
        Message::reply(&msg, netdsc);
        dispatcher->Send(NULL,0,this);
        return false;
    }
    if( (status == FIN_WAIT1) && (rflag == (FIN|ACK)) ){
        remoteip=frame->IPHeader->srcip;
        seqnum=bswapl(frame->IPHeader->TCPHeader->acknumber);
        acknum=bswapl(frame->IPHeader->TCPHeader->seqnumber)+1;
        status=TIME_WAIT; 
        flags=ACK;
        window=1403;
        printf("FIN_WAIT1->TIME_WAIT %d %d\n",status,rflag);
        Message::reply(&msg, netdsc);//close
        dispatcher->Send(NULL,0,this);
        dispatcher->RemoveConnection(this,500);
        return false;
    }
    if( Strip(frame,&data) == 0 ){
        if( (status == ESTAB) && (rflag == ACK) ){
            return true; //write_bottom_half.
        }
        return false; //Dispose.
    }
    /////PASV OPEN CLOSE////////////////////
    if( (status == LISTEN ) && (rflag == SYN ) ){
        printf("<<%d>>\n",bswap(frame->IPHeader->id));
        remoteip=frame->IPHeader->srcip;
        remoteport=bswap(frame->IPHeader->TCPHeader->srcport);// srcport!!!
        seqnum=bswapl(frame->IPHeader->TCPHeader->acknumber);
        acknum=bswapl(frame->IPHeader->TCPHeader->seqnumber)+1;
        status=SYN_RCVD;
        flags=SYN|ACK;
        window=1404;
        printf("LISTEN->SYN_RCVD %d %d\n",seqnum,acknum);
        dispatcher->Send(NULL,0,this);
        return false;
    }

    return true; 
    //return false; 
}

void TCPCoInfo::SendACK(Ether* frame)
{
     byte* tmp;
     int size=Strip(frame,&tmp);
     if( size == 0 ) size =1 ;
     remoteip=frame->IPHeader->srcip;
     seqnum=bswapl(frame->IPHeader->TCPHeader->acknumber);
     acknum=bswapl(frame->IPHeader->TCPHeader->seqnumber)+size;
     status=ESTAB; 
     flags=ACK;    
     window=1405;
     //printf("SENDING READ ACK");
     dispatcher->Send(NULL,0,this);
}