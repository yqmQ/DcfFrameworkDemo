/*
 * Copyright (c) 2021 Huawei Technologies Co.,Ltd.
 *
 * openGauss is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 *
 * test_main.c
 *    DCF test main
 *
 * IDENTIFICATION
 *    test/test_main/test_main.c
 *
 * -------------------------------------------------------------------------
 */
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "dcf_interface.h"
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef WIN32
#include <windows.h>
#define cm_sleep(ms) Sleep(ms)
#else
#include <signal.h>
static inline void cm_sleep(int ms)
{
    struct timespec tq, tr;
    tq.tv_sec = ms / 1000;
    tq.tv_nsec = (ms % 1000) * 1000000;

    (void)nanosleep(&tq, &tr);
}
#endif

#ifndef _WIN32
static void sig_proc(int signal)
{
    exit(0);
}
#endif
bool isleader = false;

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
int usr_cb_after_writer(unsigned int stream_id, unsigned long long index,
                        const char *buf, unsigned int size, unsigned long long key, int error_no)
{
    return dcf_set_applied_index(stream_id, index);
}

int usr_cb_consensus_notify(unsigned int stream_id, unsigned long long index,
                            const char *buf, unsigned int size, unsigned long long key)
{
    return dcf_set_applied_index(stream_id, index);
}
void Exit_Dcf(inputBuffer *buf)
{
    free(buf);
    if (dcf_stop() == 0)
    {
        printf("Dcf Stop.Bye.\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("Dcf stop fail.\n");
    }
}
int usr_cb_status_changed_notify(unsigned int stream_id, dcf_role_t new_role)
{
    isleader = true;
    return 0;
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
void Dcf_Expression_Write(bool isleader, inputBuffer *input_buffer, char *writeContents, unsigned long long *writeIndex)
{

    if (isleader)
    {
        if (dcf_write(1, writeContents, input_buffer->inputLength, 0, writeIndex) != 0)
        {
            printf("write fail.\n");
        }
        else
        {
            printf("dcf write succeed,size=%d,index =%d.\n", input_buffer->inputLength, *writeIndex);
            printf("\n");
        }
    }
    else
    {
        if (dcf_universal_write(1, writeContents, input_buffer->inputLength, 0, writeIndex) != 0)
        {
            printf("universal write fail.\n");
        }
        else
        {
            printf("dcf universal write succeed,size=%d,index = %d.\n", input_buffer->inputLength, *writeIndex);
        }
    }
}

void Dcf_Expression_Read(unsigned int streamId, unsigned long long ReadIndex, char *ReadBuffer, unsigned int ReadLength)
{
    if (dcf_read(1, ReadIndex, ReadBuffer, ReadLength) == -1)
    {
        printf("dcf read fail.\n");
    }
    else
    {
        printf("dcf read succeed.\n");
    }
}

int main(int argc, char *argv[])
{
#ifndef _WIN32
    if (signal(SIGUSR1, sig_proc) == SIG_ERR)
    {
        printf("register SIGUSR1 sig_proc failed!\n");
    }
    if (signal(SIGUSR2, sig_proc) == SIG_ERR)
    {
        printf("register SIGUSR2 sig_proc failed!\n");
    }
#endif
    char *dcf_start_config = (char *)malloc(1024);
    if (dcf_start_config == NULL)
    {
        printf("Can't allocate memory.\n");
    }
    char *writeContents = (char *)malloc(1024);
    if (writeContents == NULL)
    {
        printf("Can't allocate memory.\n");
    }
    Read_Dcf_start_Config(dcf_start_config);
    inputBuffer *input_buffer = newInputBuffer();
    int node_id = 1;
    int ret;
    printf("dcf lib version: %s\r\n", dcf_get_version());
    int ret_log = 0;
    int ret_data = 0;
    if (node_id == 1)
    {
        ret_log = dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log");
        ret_data = dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data");
    }
    else if (node_id == 2)
    {
        ret_log = dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log");
        ret_data = dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data");
    }
    else if (node_id == 3)
    {
        ret_log = dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log");
        ret_data = dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data");
    }
    else if (node_id == 4)
    {
        ret_log = dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log");
        ret_data = dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data");
    }
    else if (node_id == 5)
    {
        ret_log = dcf_set_param("LOG_PATH", "../primary_data_dir/dcf_log");
        ret_data = dcf_set_param("DATA_PATH", "../primary_data_dir/dcf_data");
    }

    if (ret_data != 0 && ret_log != 0)
    {
        printf("set param DATA_PATH fail\n");
    }

    ret = dcf_set_param("LOG_FILE_PERMISSION", "640");
    if (ret != 0)
    {
        printf("set permission data fail.\n");
    }
    ret = dcf_set_param("LOG_PATH_PERMISSION", "750");
    if (ret != 0)
    {
        printf("set permission log fail.\n");
    }

    ret = dcf_set_param("FLOW_CONTROL_CPU_THRESHOLD", "80");
    if (ret != 0)
    {
        printf("set param FLOW_CONTROL_CPU_THRESHOLD fail\n");
    }
    ret = dcf_set_param("FLOW_CONTROL_NET_QUEUE_MESSAGE_NUM_THRESHOLD", "100");
    if (ret != 0)
    {
        printf("set param FLOW_CONTROL_NET_QUEUE_MESSAGE_NUM_THRESHOLD fail\n");
    }
    ret = dcf_set_param("FLOW_CONTROL_DISK_RAWAIT_THRESHOLD", "12000");
    if (ret != 0)
    {
        printf("set param FLOW_CONTROL_DISK_RAWAIT_THRESHOLD fail\n");
    }
    ret = dcf_set_param("LOG_LEVEL", "RUN_ERR|RUN_WAR|RUN_INF|DEBUG_ERR|DEBUG_WAR|DEBUG_INF|MEC|OPER|TRACE|PROFILE");
    if (ret != 0)
    {
        printf("set param LOG_LEVEL fail.\n");
    }

    // register callback fiunction
    if (dcf_register_after_writer(usr_cb_after_writer) != 0)
    {
        printf("dcf_register_after_writer fail\n");
    }
    if (dcf_register_consensus_notify(usr_cb_consensus_notify) != 0)
    {
        printf("dcf_register_consensus_notify fail\n");
    }
    if (dcf_register_status_notify(usr_cb_status_changed_notify) != 0)
    {
        printf("dcf_register_status_notify fail\n");
    }

    // dcf start
    if (dcf_start(node_id, dcf_start_config) == 0)
    {
        printf("start success, node_id=%d\n", node_id);
    }
    else
    {
        printf("start fail,node_id=%d\n", node_id);
        return -1;
    }

    Print_REPL();
    long long count = 0;
    unsigned long long i = 0;
    unsigned long long int writeIndex = 0;
    char readbuffer[2048];
    unsigned long long lastindex = 10;
    unsigned long long appliedindex = 10;
    unsigned long long leadlastindex = 10;
    unsigned long long last_disk_index = 0;
    unsigned long long selectIndex = -1;
    unsigned long long data_last_commit_index;
    unsigned long long cluster_min_applied_idx;

    do
    {
        Print_Prompt();
        readCommand(input_buffer);
        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
            free(dcf_start_config);
            free(writeContents);
            Exit_Dcf(input_buffer);
        }
        // expression_write

        if (dcf_get_leader_last_index(1, &leadlastindex) == -1)
        {

            printf("dcf_get_leader_last_index fail.\n");
        }
        else
        {
            printf("The leader last index is %ld.\n", leadlastindex);
        }
        if (dcf_get_node_last_disk_index(1, node_id, &last_disk_index) == -1)
        {

            printf("dcf_node_last_disk_index fail.\n");
        }
        else
        {
            printf("The node last disk index is %ld.\n", last_disk_index);
        }

        if (dcf_get_data_commit_index(1, node_id, &data_last_commit_index) == -1)
        {

            printf("dcf data last commit index fail.\n");
        }
        else
        {
            printf("The data last commit index is %ld.\n", data_last_commit_index);
        }

        if (dcf_get_cluster_min_applied_idx(1, &cluster_min_applied_idx) == -1)
        {

            printf("dcf_get_cluster min applied index fail.\n");
        }
        else
        {
            printf("The cluster min applid  index is %ld.\n", cluster_min_applied_idx);
        }
        printf("\n\n");
        ssize_t cmp_insert = strncmp(input_buffer->buffer, "insert", 6);
        if (cmp_insert == 0)
        {
            int arg_size = sscanf(input_buffer->buffer, "insert %s", writeContents);
            if (arg_size < 1)
            {
                printf("arg insert error.\n");
            }
            Dcf_Expression_Write(isleader, input_buffer, writeContents, &writeIndex);
        }
        ssize_t cmp_select = strncmp(input_buffer->buffer, "select", 6);
        if (cmp_select == 0)
        {

            int arg_select = sscanf(input_buffer->buffer, "select %ld", &selectIndex);
            if (arg_select < 1)
            {
                printf("arg select error.\n");
            }
            Dcf_Expression_Read(1, selectIndex, readbuffer, 1024);
            printf("*************************************************************************\n");
            printf("The  index %ld read contents is %s.\n", selectIndex, readbuffer);
            printf("\n\n");
        }

        // log truncate
        if ((++count) % 10000 == 0)
        {
            if (dcf_truncate(1, count / 2) != 0)
            {
                printf("truncate fail.\n");
            }
            else
            {
                printf("truncate succeed index = %lld.\n", count / 2);
            }
        }

        cm_sleep(200);
    } while (1);
    return 0;
}
