#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


static int readTemperature(void)
{
    return (char)(10 + rand() % 80);
}

static int readPressure(void)
{
    return (char)(rand() % 120);
}

static void readConfigVersion(char *out, size_t out_len)
{
    snprintf(out, out_len, "1234");
}

// Polling thread: polls every 50ms
void* PollingThread(void *arg)
{
    (void)arg;
    static int  ConfigVerPolledOnce = 0;
    while (1)
    {
        GlobalPolledValue_t entry;

        // -------- Temperature --------
        entry = GetPolledValue(PARAM_TEMP);//reads the previous data
        entry.Values.Value = readTemperature();
        entry.Timestamp = time(NULL);

        SetPolledValue(PARAM_TEMP, &entry);

        // -------- Pressure -------- 
        entry = GetPolledValue(PARAM_PRESSURE);
        entry.Values.Value = readPressure();
        entry.Timestamp = time(NULL);

        SetPolledValue(PARAM_PRESSURE, &entry);

        // -------- Config Version Polled Once--------
        if(ConfigVerPolledOnce == 0)
        {
        entry = GetPolledValue(PARAM_CONFIG_VERSION);
        readConfigVersion(entry.Values.strValue);
        entry.Timestamp = time(NULL);

        SetPolledValue(PARAM_CONFIG_VERSION, &entry);
        ConfigVerPolledOnce = 1;
        }

        // 50 ms
        usleep(50 * 1000);
    }
    return NULL;
}