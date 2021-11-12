//
//  ExampleUSBDriver.cpp
//  ExampleUSBDriver
//
//

#include <os/log.h>

#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>

#include "ExampleUSBDriver.h"

kern_return_t
IMPL(ExampleUSBDriver, Start)
{
    kern_return_t ret;
    ret = Start(provider, SUPERDISPATCH);
    os_log(OS_LOG_DEFAULT, "Hello World");
    return ret;
}
