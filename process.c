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

// Processing thread runs every 200ms
void* ProcessingThread(void *arg)
{
    (void)arg;
    static uint16_t tempViolationTime = 0;
    static uint16_t pressureViolationTime = 0;
    static char lastConfigVersion[STR_LEN] = {0};
    uint8_t SamplingTime = 400;

    while (1)
    {
        GlobalPolledValue_t tempdata = GetPolledValue(PARAM_TEMP);
        char v = tempdata.Values.Value;

        // normal processing 
        processTempAction(tempdata.Values.Value);

        if (v < tempdata.LowerThreshold || v > tempdata.UpperThreshold)
            {
                tempViolationTime += 200;
            }
        else
            {
                tempViolationTime = 0;

            }

        if(tempViolationTime >= SamplingTime)
            {
                sendNotification_Temp(tempdata.Values.Value);
                SetPolledValue(PARAM_TEMP, &tempdata);
                tempViolationTime = 0;
            }
        

        // ---------- Pressure ---------- 
        GlobalPolledValue_t pressuredata = GetPolledValue(PARAM_PRESSURE);
        char p = pressuredata.Values.Value;

        // normal processing
        processPressureAction(data.Values.Value);

        if (p < pressuredata.LowerThreshold || p > pressuredata.UpperThreshold)
            {
                pressureViolationTime += 200;
            }
        else
            {
                pressureViolationTime = 0;

            }
            
        if(pressureViolationTime >= SamplingTime)
            {
                sendNotification_Pressure(pressuredata.Values.Value);
                SetPolledValue(PARAM_TEMP, &pressuredata);
                pressureViolationTime = 0;
            }

        // ------------ Config Version ------------
        GlobalPolledValue_t configdata = GetPolledValue(PARAM_CONFIG_VERSION);
        if(strcmp(lastConfigVersion,configdata.Values.strValue) != 0)
        {
            char *configstr = (char *)malloc(strlen(configdata.Values.strValue)+1);//Dynamic memory allocation
            if(configstr != NULL)
            {
            strcpy(configstr,configdata.Values.strValue);
            printf("Config Version Changed\n");
            free(configstr);
            }
            strcpy(lastConfigVersion,configdata.Values.strValue);
        }
        
        // processing interval: 200 ms
        usleep(200 * 1000);
    }
    return NULL;
}