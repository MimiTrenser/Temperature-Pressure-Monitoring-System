#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


static int readTemperature(void)
{
    return (char)(10 + rand() % 121); // 0 to 100
}

static int readPressure(void)
{
    return (char)(rand() % 10001);// 0 to 10000
}

static void readConfigVersion(char *out, size_t out_len)
{
    snprintf(out, out_len, "1234");
}
uint32_t getTimeMs()
{
    return (uint64_t)time(NULL) * 1000;
}
#define SizeofPollTable sizeof(polling_table)/sizeof(PollingConfig_t);

PollingConfig_t polling_table[SizeofPollTable] = {{PARAM_TEMP,50,readTemperature,0},
                                                  {PARAM_PRESSURE,200,readPressure,0}};

// Polling thread: polls every 50ms
void* PollingThread(void *arg)
{
    (void)arg;
   // static int  ConfigVerPolledOnce = 0;//?
 
    while (1)
    {
        uint64_t now = getTimeMs();
        for(int i = 0;i < SizeofPollTable;i++)
        {
        if((now - polling_table[i].lastpoll_time) >= polling_table[i].PollInterval_ms)
        {
            int32_t value = polling_table[i].read_fn();
            SetPolledValue(polling_table[i].param,value);
            polling_table[i].lastpoll_time = now;   
        }
        }

        // // -------- Config Version Polled Once--------
        // if(ConfigVerPolledOnce == 0)
        // {
        // entry = GetPolledValue(PARAM_CONFIG_VERSION);
        // readConfigVersion(entry.Values.strValue);
        // entry.Timestamp = time(NULL);

        // SetPolledValue(PARAM_CONFIG_VERSION, &entry);
        // ConfigVerPolledOnce = 1;
        // }

        // // 50 ms
        // usleep(50 * 1000);
    }
    return NULL;
}