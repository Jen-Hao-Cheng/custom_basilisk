%module PIDControl
%{
   #include "PIDControl.h"
%}

%include "swig_conly_data.i"
%constant void Update_PIDControl(void*, uint64_t, uint64_t);
%ignore Update_PIDControl;
%constant void SelfInit_PIDControl(void*, uint64_t);
%ignore SelfInit_PIDControl;
%constant void Reset_PIDControl(void*, uint64_t, uint64_t);
%ignore Reset_PIDControl;

%include "PIDControl.h"

%include "../customBSK/msgPayloadDefC/AngleMsgPayload.h"
%include "../customBSK/msgPayloadDefC/ControlMsgPayload.h"
struct AngleMsg_C;
struct ControlMsg_C;

%pythoncode %{
import sys
protectAllClasses(sys.modules[__name__])
%}
