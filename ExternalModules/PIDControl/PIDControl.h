#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_

#include <stdint.h>
#include "architecture/utilities/bskLogging.h"
#include "cMsgCInterface/AngleMsg_C.h"
#include "cMsgCInterface/ControlMsg_C.h"



/*! @brief Top level structure for the sub-module routines. */
typedef struct {
    /* declare module private variables */
    double Kp;
    double Kd;
    double Ki;
    double previousError;
    double accumulativeError;
    
    /* declare module IO interfaces */
    ControlMsg_C controlOutMsg;             //!< control effort 
    AngleMsg_C refInMsg;                    //!< reference input
    AngleMsg_C angInMsg;                    //!< angle 

    BSKLogger *bskLogger;                   //!< BSK Logging
}PIDControlConfig;

#ifdef __cplusplus
extern "C" {
#endif

    void SelfInit_PIDControl(PIDControlConfig *configData, int64_t moduleID);
    void Update_PIDControl(PIDControlConfig *configData, uint64_t callTime, int64_t moduleID);
    void Reset_PIDControl(PIDControlConfig *configData, uint64_t callTime, int64_t moduleID);

#ifdef __cplusplus
}
#endif


#endif
