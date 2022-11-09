#include <stdio.h>
#include <stdlib.h>
#include "dcf_interface.h"
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct
{

    char *buffer;
    size_t bufferLength;
    ssize_t inputLength;

} inputBuffer;

inputBuffer *newInputBuffer()
{
    inputBuffer *buf = (inputBuffer *)malloc(sizeof(inputBuffer));
    buf->buffer = NULL;
    buf->bufferLength = 0;
    buf->inputLength = 0;

    return buf;
}
void readCommand(inputBuffer *buf)
{
    ssize_t byteRead = getline(&(buf->buffer), &(buf->bufferLength), stdin);
    if (byteRead <= 0)
    {
        printf("FAIL TO READ COMMAND\n.");
    }

    buf->inputLength = byteRead - 1;
    buf->buffer[byteRead - 1] = 0;
}

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
    if (dcf_set_param("ELECTION_TIMEOUT", "3") != 0)
    {
        return false;
    }

    if (dcf_set_param("RUN_MODE", "0") != 0)
    {
        return false;
    }

    if (dcf_set_param("FLOW_CONTROL_CPU_THRESHOLD", "100") != 0)
    {
        return false;
    }

    if (dcf_set_param("MEC_AGENT_THREAD_NUM", "10") != 0)
    {
        return false;
    }

    if (dcf_set_param("MEC_REACTOR_THREAD_NUM", "1") != 0)
    {
        return false;
    }

    if (dcf_set_param("CONNECT_TIMEOUT", "6000") != 0)
    {
        return false;
    }
    if (dcf_set_param("REP_APPEND_THREAD_NUM", "2") != 0)
    {
        return false;
    }
    // Can't set MEC_FRAGMENT_NUM
    // if (dcf_set_param("MEC_FRAGMENT_NUM", "64") != 0)
    // {
    //     return false;
    // }
    if (dcf_set_param("STG_POOL_INIT_SIZE", "32") != 0)
    {
        return false;
    }
    if (dcf_set_param("STG_POOL_MAX_SIZE", "2048") != 0)
    {
        return false;
    }

    if (dcf_set_param("MEC_POOL_MAX_SIZE", "200") != 0)
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

    if (dcf_set_param("SSL_CIPHER", "ECDHE-ECDSA-AES256-GCM-SHA384:") != 0)
    {
        return false;
    }

    return true;
}

void init_dcf_SSL()
{
}

void Exit_Dcf(inputBuffer *buf)
{
    free(buf);
    free(buf->buffer);
    if (dcf_stop() != 0)
    {
        printf("Exit Dcf fail.\n");
    }
    else
    {
        printf("Dcf Stop.Bye.\n");
        exit(EXIT_SUCCESS);
    }
}

void Read_Dcf_start_Config(char *config)
{
    int fd = open("../DcfFrameworkDemoConfig.json", O_RDONLY);
    if (fd == -1)
    {
        printf("can not open the DcfFrameworkConfig.json file.\n");
        exit(EXIT_FAILURE);
    }
    int len = read(fd, config, 1024);
    if (len < 0)
    {
        printf("The start config is NULL.\n");
    }

    close(fd);
}

int main(int argc, char *argv[])
{ // Read_Buffer is designed for dcf_read.
    char *dcf_start_config = (char *)malloc(1024);
    if (dcf_start_config == NULL)
    {
        printf("Can't allocate memory.\n");
    }
    Read_Dcf_start_Config(dcf_start_config);
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
            printf("\n\n");
            break;
        }
    }
    if (!is_start_dcf)

    {
        printf("WARNING,CAN NOT START DCF IN %.2fs.\n", duration);
    }
    inputBuffer *input_buffer = newInputBuffer();
    int Command_buffer;
    Print_REPL();
    // Test framework
    do
    {
        Print_Prompt();
        readCommand(input_buffer);
        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
            free(dcf_start_config);
            Exit_Dcf(input_buffer);
        }
        // if (dcf_write(1,) != 0)
        // {
        //     printf("dcf write fail.\n");
        // }
        // // dcf_read(1,index,Read_Buffer,length);
        printf("%s\n", input_buffer->buffer);
    } while (true);

    return 0;
}