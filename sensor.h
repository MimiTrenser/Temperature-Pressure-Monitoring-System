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

// union to hold either integer or small string 
typedef union
{
    char   Value;              // temperature / pressure//??????
    char  strValue[STR_LEN];   // config version e.g. "1234"
} Values;

// global polled structure
typedef struct
{
    ParamId_t Param;       // which parameter
    Values   Value;         // value
    uint32_t  Timestamp;   // each time updates param value time is notted

    char   LowerThreshold;
    char   UpperThreshold;
} GlobalPolledValue_t;

void SensorSystem_Init(void);
void SetPolledValue(ParamId_t id, const GlobalPolledValue_t *data);
GlobalPolledValue_t GetPolledValue(ParamId_t id);

void* PollingThread(void *arg);
void* ProcessingThread(void *arg);

#endif // SENSOR_SYSTEM_H