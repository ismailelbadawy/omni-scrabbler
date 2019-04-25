#include "MessageStruct.cpp"


class CommInterface
{

virtual int Send(Message)=0;

virtual Message Receive()=0;

};
