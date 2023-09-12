/* modify the path to reflect the new module names */
#include "PIDControl.h"
#include "string.h"

/*
 Pull in support files from other modules.  Be sure to use the absolute path relative to Basilisk directory.
 */


/*!
    This method initializes the output messages for this module.
 @return void
 @param configData The configuration data associated with this module
 @param moduleID The module identifier
 */
void SelfInit_PIDControl(PIDControlConfig *configData, int64_t moduleID)
{
    ControlMsg_C_init(&configData->controlOutMsg);
}


/*! This method performs a complete reset of the module.  Local module variables that retain
 time varying states between function calls are reset to their default values.
 @return void
 @param configData The configuration data associated with the module
 @param callTime [ns] time the method is called
 @param moduleID The module identifier
*/
void Reset_PIDControl(PIDControlConfig *configData, uint64_t callTime, int64_t moduleID)
{
    /*! reset any required variables */
    configData->previousError = 0.0;
    configData->accumulativeError = 0.0;
    
    char info[MAX_LOGGING_LENGTH];
    sprintf(info,
     "Reset previousError = %f, accumulativeError = %f in reset, Kp = %f",
     configData->previousError, configData->accumulativeError, configData->Kp);
    _bskLog(configData->bskLogger, BSK_INFORMATION, info);
}

/*! Add a description of what this main Update() routine does for this module
 @return void
 @param configData The configuration data associated with the module
 @param callTime The clock time at which the function was called (nanoseconds)
 @param moduleID The module identifier
*/
void Update_PIDControl(PIDControlConfig *configData, uint64_t callTime, int64_t moduleID)
{
    ControlMsgPayload controlOutMsgBuffer;      /*!< control effort */
    AngleMsgPayload refInMsgBuffer;             /*!< reference input */
    AngleMsgPayload angInMsgBuffer;             /*!< angle */

    double reference = 0;           /*!< reference input */
    double angle = 0;               /*!< angle */
    double error = 0;               /*!< error */

    // always zero the output buffer first
    controlOutMsgBuffer = ControlMsg_C_zeroMsgPayload();
    
    /*! - Read the optional input messages */
    if (AngleMsg_C_isLinked(&configData->refInMsg)) {
        refInMsgBuffer = AngleMsg_C_read(&configData->refInMsg);
        reference = refInMsgBuffer.angle;
    }
    if (AngleMsg_C_isLinked(&configData->angInMsg)) {
        angInMsgBuffer = AngleMsg_C_read(&configData->angInMsg);
        angle = angInMsgBuffer.angle;
    }

    error = reference - angle;
    controlOutMsgBuffer.u = configData->Kp*error;

    configData->previousError = error;

    /*! - write the module output message */
    ControlMsg_C_write(&controlOutMsgBuffer, &configData->controlOutMsg, moduleID, callTime);

    /* this logging statement is not typically required.  It is done here to see in the
     quick-start guide which module is being executed */
    char info[MAX_LOGGING_LENGTH];
    sprintf(info, "error = %f, u = %f", error, controlOutMsgBuffer.u);
    _bskLog(configData->bskLogger, BSK_INFORMATION, info);

    return;
}
