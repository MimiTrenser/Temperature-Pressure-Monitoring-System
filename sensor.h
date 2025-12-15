#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

#include <stdint.h>

#define STR_LEN 5

typedef enum
{
    PARAM_TEMP ,
    PARAM_PRESSURE ,
    PARAM_CONFIG_VERSION
} ParamId_t;

typedef struct
{
    ParamId_t Param;       // which parameter
    uint32_t PollInterval_ms;
    int (read_fn*)(void);
    uint64_t lastpoll_time;
} PollingConfig_t;

typedef struct
{
    ParamId_t Param;
    uint32_t processInterval_ms;
    uint32_t mini_threshold;
    uint64_t max_threshold;
    uint32_t samplingtime;
    uint64_t violationtime;
}ProcessConfig_t;

typedef struct
{
    uint32_t Temperature;
    uint32_t pressure;
}SensorResult;
extern SensorResult GetSensorResult;



//void SensorSystem_Init(void);
void SetPolledValue(ParamId_t id, const GlobalPolledValue_t *data);
GlobalPolledValue_t GetPolledValue(ParamId_t id);

void* PollingThread(void *arg);
void* ProcessingThread(void *arg);

#endif // SENSOR_SYSTEM_H