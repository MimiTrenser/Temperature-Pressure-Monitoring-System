#include "sensor.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include<stdbool.h>

/* Notification handlers (replace with proper notification mechanism) */
static void sendNotification_Temp(int value)
{
    printf("[NOTIFY] TEMP OUT OF RANGE -> %d\n", value);
}

static void sendNotification_Pressure(int value)
{
    printf("[NOTIFY] PRESSURE OUT OF RANGE -> %d\n", value);
}

static void processTempAction(int v)
{

    printf("processTemp: %d\n", v);
}

static void processPressureAction(int v)
{
    printf("processPressure: %d\n", v);
}

#define SizeofProcessTable sizeof(process_table)/sizeof(ProcessConfig_t);
PollingConfig_t process_table[SizeofProcessTable] = {{PARAM_TEMP,50,10,70,400,0},
                                                     {PARAM_PRESSURE,400,100,5000,0}};

// Processing thread runs every 400ms
void* ProcessingThread(void *arg)
{
    (void)arg;

    while (1)
    {
        for(int i = 0;i < SizeofProcessTable ; i++)
        {
            int32_t Data = GetPolledValue(process_table[i].Param);
            if(Data < process_table[i].mini_threshold || Data > process_table[i].mini_threshold)
            {
                process_table[i].violationtime += process_table[i].processInterval_ms;//0,200,400
            }
            else
            {
                processPressureAction(Data);
                process_table[i].violationtime = 0;

            }
            if(process_table[i].violationtime >= process_table[i].samplingtime)
            {
                sendNotification_Temp(tempdata.Values.Value);
                process_table[i].violationtime = 0;
            }
        }
    }
    return NULL;
}