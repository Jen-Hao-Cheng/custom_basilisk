
from Basilisk.architecture import bskLogging
from Basilisk.architecture import messaging  # import the message definitions
from Basilisk.ExternalModules import PIDControl  # import the module that is to be tested
# from Basilisk.ExternalModules.architecture import messaging
# Import all of the modules that we are going to be called in this simulation
from Basilisk.utilities import SimulationBaseClass
from Basilisk.utilities import macros
from Basilisk.utilities import unitTestSupport  # general support file with common unit test functions


def run():
    taskName = "Task"               # arbitrary name (don't change)
    processName = "Process"         # arbitrary name (don't change)
    
    # Create a sim module as an empty container
    scSim = SimulationBaseClass.SimBaseClass()
    
    # Create test thread
    processRate = macros.sec2nano(0.01)     # update process rate update time
    proc = scSim.CreateNewProcess(processName)
    proc.addTask(scSim.CreateNewTask(taskName, processRate))
    
    # Construct algorithm and associated C++ container
    pidControlConfig = PIDControl.PIDControlConfig()  # update with current values
    pidControlWrap = scSim.setModelDataWrap(pidControlConfig)
    pidControlWrap.ModelTag = "PID"           # update python name of test module
    
    # Add test module to runtime call list
    scSim.AddModelToTask(taskName, pidControlWrap, pidControlConfig)
    
    # PID Control parameter
    pidControlConfig.Kp = 10
    # Kd
    # Ki
    
    # Create input message and size it because the regular creator of that message
    # is not part of the test.
    refMessageData = messaging.AngleMsgPayload()  # Create a structure for the reference input
    refMessageData.angle = 1
    refInputMsg = messaging.AngleMsg().write(refMessageData)
    
    angMessageData = messaging.AngleMsgPayload()
    angMessageData.angle = 0
    angInputMsg = messaging.AngleMsg().write(angMessageData)
    
    
    # Setup logging on the test module output message so that we get all the writes to it
    # dataLog = pidControlConfig.controlOutMsg.recorder()
    # scSim.AddModelToTask(taskName, dataLog)
    
    # connect the message interfaces
    pidControlConfig.refInMsg.subscribeTo(refInputMsg)
    pidControlConfig.angInMsg.subscribeTo(angInputMsg)
    
    # Need to call the self-init and cross-init methods
    scSim.InitializeSimulation()
    print("---------------------------------------------------------")
    #
    scSim.TotalSim.SingleStepProcesses()
    
    return

if __name__ == "__main__":
    run()