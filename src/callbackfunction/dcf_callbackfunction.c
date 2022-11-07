/*



*/

#include"dcf_callbackfuntion.h"
#include<stdio.h>
int ConsensusLogCbFunc(unsigned int stream_id, unsigned long long paxosIdx,
                       const char *buf, unsigned int len, unsigned long long lsn, int error_no)
{

}
int ReceiveLogCbFunc(unsigned int stream_id, unsigned long long index,
                     const char *buf, unsigned int len, unsigned long long lsn)
{
}

int PromoteOrDemote(unsigned int stream_id, dcf_role_t new_role)
{
}
int DCFExceptionCbFunc(unsigned int stream_id, dcf_exception_t dcfException)
{
    printf("Enter DCFExceptionCbFunc and stream_id is %u, dcf exception is %d",
           stream_id, dcfException);
}
int ElectionCbFunc(unsigned int stream_id, unsigned int new_leader)
{
}

int ProcessMsgCbFunc(unsigned int stream_id, unsigned int src_node_id,
                     const char *msg, unsigned int msg_size)
{
}
void DcfThreadShmemInit(void)
{
}