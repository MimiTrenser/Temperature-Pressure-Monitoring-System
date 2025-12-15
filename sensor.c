#include "sensor.h"
#include <pthread.h>
#include <string.h>
#include <time.h>

/* internal globals */
static GlobalPolledValue_t gPolledValues[PARAM_MAX];
static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

void SensorSystem_Init(void)
{
    pthread_mutex_lock(&gMutex);

    // init Temperature entry
    gPolledValues[PARAM_TEMP] = (GlobalPolledValue_t){
    .Param = PARAM_TEMP;
    .Values.Value = 0;
    .Timestamp = time(NULL);
    .LowerThreshold = 20;
    .UpperThreshold = 70;
    };

    // init Pressure entry
    gPolledValues[PARAM_PRESSURE] = (GlobalPolledValue_t){
    .Param = PARAM_PRESSURE;
    .Values.Value = 0;
    .Timestamp = (uint32_t)time(NULL);
    .LowerThreshold = 30;
    .UpperThreshold = 5000;
    };

    // initialize Config Version entry
    gPolledValues[PARAM_CONFIG_VERSION].Param = PARAM_CONFIG_VERSION;
    strcpy(gPolledValues[PARAM_CONFIG_VERSION].Values.strValue, "1234");

    pthread_mutex_unlock(&gMutex);
}

void SetPolledValue(ParamId_t id, const GlobalPolledValue_t *data)
{
    pthread_mutex_lock(&gMutex);
    gPolledValues[id] = *data; //copying updated values to global structure
    pthread_mutex_unlock(&gMutex);
}

GlobalPolledValue_t GetPolledValue(ParamId_t id)
{
    GlobalPolledValue_t copy;
    memset(&copy, 0, sizeof(copy));//Initialize structure variale copy or clear it
    //GlobalPolledValue_t *copy = (GlobalPolledValue_t*)malloc(sizeof(GlobalPolledValue_t));
    pthread_mutex_lock(&gMutex);
    copy = gPolledValues[id];
    pthread_mutex_unlock(&gMutex);
    return copy;
}