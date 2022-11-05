#include <stdio.h>
#include <stdlib.h>
#include "dcf_interface.h"
#include <stdbool.h>
#include <time.h>
void Print_REPL()
{
    printf("This is a DcfFrameworkDemo.\n");
    printf("\n");
}
void Print_Prompt()
{
    printf("DcfFrameDemo > ");
}
// CallBack function
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

static bool RegisterDcfCallBacks()
{
    if (dcf_register_after_writer(ConsensusLogCbFunc) != 0)
    {
        printf("WARNING,Failed to register ConsensusLogCbFunc.");
        return false;
    }
    if (dcf_register_consensus_notify(ReceiveLogCbFunc) != 0)
    {
        printf("WARNING,Failed to register ReceiveLogCbFunc.");
        return false;
    }
    if (dcf_register_status_notify(PromoteOrDemote) != 0)
    {
        printf("WARNING,Failed to register PromoteOrDemote.");
        return false;
    }
    if (dcf_register_exception_report(DCFExceptionCbFunc) != 0)
    {

        printf("WARNING,Failed to register PromoteOrDemote.");
        return false;
    }
    if (dcf_register_election_notify(ElectionCbFunc) != 0)
    {

        printf("WARNING,Failed to register PromoteOrDemote.");
        return false;
    }
    if (dcf_register_msg_proc(ProcessMsgCbFunc) != 0)
    {
        printf("WARNING,Failed to register ProcessMsgCbFunc.");
        return false;
    }
    if (dcf_register_thread_memctx_init(DcfThreadShmemInit) != 0)
    {
        printf("WARNING,Failed to register DcfThreadShmemInit.");
        return false;
    }
    return true;
}

bool Set_dcf_param()
{
    if (dcf_set_param("ELECTION_TIMEOUT", "100") != 0)
    {
        return false;
    }

    if (dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data") != 0)
    {

        return false;
    }

    if (dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log") != 0)
    {

        return false;
    }

    if (dcf_set_param("INSTANCE_NAME", "DcfFrameworkDemo") != 0)
    {

        return false;
    }

    if (dcf_set_param("INSTANCE_NAME", "DcfFrameworkDemo") != 0)
    {

        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{ // Read_Buffer is designed for dcf_read.
    char *Read_Buffer;
    char *dcf_start_config = "[{\"stream_id\" : 1, \"node_id\" : 1, \"ip\" : \"172.19.0.202\", \"port\" : 20200,\"role\" : \"LEADER\"}, {\"stream_id\" : 1, \"node_id\" : 2, \"ip\" : \"172.19.0.203\", \"port\" : 20200, \"role\" : \"FOLLOWER\"}, {\"stream_id\" : 1, \"node_id\" : 3, \"ip\" : \"172.19.0.204\", \"port\" : 20200, \"role\" : \"FOLLOWER\"} ]";
    if (!Set_dcf_param())
    {
        printf("Set Dcf params fail,exit!\n");
        exit(EXIT_FAILURE);
    }
    if (!RegisterDcfCallBacks())
    {
        printf("Register Dcf CallBack Function fail!\n");
        exit(EXIT_FAILURE);
    };
    // dcf start
    bool is_start_dcf = false;
    double duration = 5;
    clock_t start;
    clock_t print_last_time;
    print_last_time = clock();
    start = clock();
    while (((double)(clock() - start) / CLOCKS_PER_SEC) < duration)
    {
        if (dcf_start(1, dcf_start_config) != 0)
        {

            if (((double)(clock() - print_last_time) / CLOCKS_PER_SEC) > 1)
            {
                printf("Dcf start fail! Trying to start dcf in %.2fs...\n", duration);
                printf("\n");
                print_last_time = clock();
            }
        }
        else
        {
            is_start_dcf = true;
            printf("DCF START SUCCESS!\n");
            break;
        }
    }
    if (!is_start_dcf)

    {
        printf("WARNING,CAN NOT START DCF IN %.2fs.\n", duration);

        printf("\n");
    }

    Print_REPL();
    // 测试框架
    // do
    // {
    //     Print_Prompt();
    //     printf("hello\n");
    //     scanf("%s", &Command_buffer);
    //     dcf_write();
    //     // // dcf_read(1,index,Read_Buffer,length);
    //     // printf("%s\n", Read_Buffer);
    // } while (true);

    return 0;
}