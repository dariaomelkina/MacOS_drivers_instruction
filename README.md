# :mage: Instruction for creating macOS Drivers 
Operational systems course project at UCU.

*Warning: instruction is unfinished.*

### :ukraine: For Ukrainian translation click [here](ukrainian_version.md)

---

## Web version
*coming (moderately) soon...*

## Table of contents
1. [Instruction](#instruction)
1. [Annotations](#annotations)
1. [Notions and acronyms](#notions-and-acronyms)
1. [Introduction to drivers](#introduction-to-drivers)
1. [Details on the task for macOS](#details-on-the-task-for-macos)
1. [macOS tools](#macos-tools)
    1. [More about I/O kit](#more-about-io-kit)
    1. [More about DriverKit](#more-about-driverkit)
1. [Drivers using DriverKit framework](#drivers-using-driverkit-framework)
    1. [Starting](#starting)
    1. [Building up the project](#building-up-the-project)
    1. [Information about the driver and matching](#information-about-the-driver-and-matching)
    1. [Installing Your driver](#installing-your-driver)
1. [Drivers using I/O Kit collection of frameworks](#drivers-using-io-kit-collection-of-frameworks)
1. [Sources/literature](#sourcesliterature)

---
# Instruction:
Let's get started and enter the magical world of the drivers creation, and May the Force be with You.

## Annotations:
*coming soon...*

## Notions and acronyms:
* **Dext** – driver extension.
* **Kext** – kernel extension.
* **SDK** – Software Development Kit.
* **HID** - Human Interface Device.

## Introduction to drivers:
Essentially, a driver is a specific code, which controls a corresponding I/O device, attached to the computer [2]. 
In other words, drivers can be viewed as a bridge between computer peripherals and the rest of the system. [3] 
So, it is a mean of communication and control. 
 
Andrew Tanenbaum’s “Modern Operating Systems” [2] provide a good overview for the drivers, which run in the kernel 
space, but in this tutorial we will mostly focus on the drivers, which run in the user space. 

## Details on the task for macOS:
As told in Amit Singh's "Mac OS X Internals. A Systems Approach" [1], even though usually writing drivers can be considered
difficult, the macOS driver architecture is helpful in this regard. One of the most appealing advantages is that it
supports user space drivers (importance of which we will discuss a little later –– in the DriverKit section).

The book, mentioned above provides a great overview of drivers architecture and task of writing them for the macOS systems. 
Here I will include some of the details, that might be important for understanding, when only starting working with
drivers in general and macOS drivers in particular.

Usually, typical Unix systems use device special files (which reside in the /dev/ directory) for the user interface 
with devices. Newer systems (macOS included) also manage devices more dynamically –– 
they allow to dynamically create or delete (and automatically assign) these device files. 
macOS provides device files for storage devices, serial devices, pseudo-terminals, and several pseudo-devices.

*more to be added later...*

## macOS tools: 
In this instruction, we will discuss two possible options, regarding the choice of tools, when writing drivers for 
macOS systems. The first is the I/O kit –– a collection of frameworks libraries, tools, and other resources for 
creating device drivers, and the second is the DriverKit –– a modernized replacement of the I/O Kit.

## More about I/O kit:
The I/O Kit is a collection of both kernel-level and user-level software, that is used as a simplified driver 
development mechanism. The I/O Kit also coordinates the use of device drivers. [1]

When writing drivers for macOS using I/O Kit, the drivers is essentially an I/O Kit object, which manages a specific
piece of hardware. [1]

## More about DriverKit:
*Information about DriverKit is retrieved from and based on the official presentation of the kit, available by the 
following [link](https://developer.apple.com/videos/play/wwdc2019/702/) and on the 
[official documentation](https://developer.apple.com/documentation/driverkit).* 

DriverKit is an SDK, which was introduced during the Worldwide Developers Conference in 2019 along with System 
Extensions. All DriverKit frameworks are based on the I/O Kit, but they were modernized to be more reliable, 
secure, and comfortable for development. DriverKit is used to build Driver Extensions (dexts) in the user space. 

Driver extensions are built using DriverKit, they are now a replacement for I/O Kit device drivers.

They are used to control the following devices:
* USB
* Serial
* NIC (Network Interface Controller)
* HID (Human Interface Device)

As described in the video by the following [link](https://developer.apple.com/videos/play/wwdc2019/702/) 
there are several advantages to using System Extensions and DriverKit:
* Unlike kexts, System Extensions run in the user space, bugs in them cannot compromise the kernel
* System Extensions have no restrictions on dynamic memory allocation, synchronization, and latency
* Building, testing, and debugging can all be performed on one machine
* Enabled full debugger support
* There is no need to restart the machine if the extension crushes
* Kernel and other applications will not stop running if the extension crushes
* DriverKit provides full modern replacement of the previously discussed I/O kit 
* DriverKit drivers run in user space, where, just like System Extensions, they cannot compromise kernel and shutter the security 
* These extensions are available to all the users, even for tasks, previously performed only by kexts
* Et cetera

Furthermore, starting with new versions of macOS, kexts will be deprecated. 
It is [officially stated](https://developer.apple.com/documentation/iokit/) by the Apple developer website, 
that devices supported on macOS 11 and later require DriverKit instead of I/O Kit. 

We will try both approaches, starting with the newer, more secure, and, perhaps, an easier one –– DriverKit framework.

---

## Drivers using DriverKit framework:
*This –– the first version of the instruction –– is based on the 
[official guidelines](https://developer.apple.com/documentation/driverkit/creating_a_driver_using_the_driverkit_sdk) [3]
for writing drivers with DriverKit SDK and the sample code [5].* 

## Starting:
To start a project, we will create it in the Xcode, which provides a base template for creating DriverKit drivers.

Open Xcode and create a new project (You can also add drivers code to a pre-existing project, Xcode gives an option for this):

![](illustrations/illust1.png)

Choose a DriverKit driver:

![](illustrations/illust2.png)

Choose options:

![](illustrations/illust3.png)

P.S. You can choose another name, which You would prefer, and specify Your organization identifier.

Now You should be able to see a somewhat similar window:

![](illustrations/illust4.png)

Congratulation! We are *almost* done.

## Building up the project:
Let's try to build a basic keyboard driver using the template we obtained just now and explore it at the same time. 
For this task we will use parts of code from the official apple documentation [3].

Firstly, what do we have in the template for the driver creation? Take a look at the directory, 
which is called the same way as Your project itself:

![](illustrations/illust5.png)

* ***.cpp** –– is a file with main C++ source file.
* ***.iig** –– is an IOKit interface generator header file. 
* ***.entitlements** –– is  a default entitlements file. We will discuss what are entitlements. 
and how to populate that file in the next section.
* ***.plist** –– is a file with specific information to support the loading and installation of the driver.

Let's begin to go through code of the Keyboard device from the documentation.
Firstly, let's check out contents of the *.iig file (DriverExample.iig in my case):
```c++
#ifndef DriverExample_h
#define DriverExample_h

#include <Availability.h>
#include <DriverKit/IOService.iig>

class DriverExample: public IOService
{
public:
    virtual kern_return_t
    Start(IOService * provider) override;
};

#endif /* DriverExample_h */
```
It will look just like that. *(Note that this information applies on the November 13th of 2021, 
and there can be slight changes of API in the millennia You currently live in)*

Here, ```IOService``` –– is a a base class of all the drivers. We can continue working with it, but it would be better to 
work with something more specific. There are different families of classes, provided by the DriverKit and for our case 
we will choose a class for handling HID events. Why exactly HID (Human Interface Device)? It is because keyboard belongs 
to such devices and we want a class to somehow obtain information on what happens with it. The exact class we would
choose instead of the base one is the ```IOUserHIDEventService```.

That is how class in Your *.iig file should look like now (plus the new additional include):
```c++
#include <HIDDriverKit/IOUserHIDEventService.iig>

class DriverExample: public IOUserHIDEventService
{
public:
    virtual kern_return_t
    Start(IOService * provider) override;
};
```

We will also need to implement some init and free methods, so we will now add them to header file, too. 
That is how the whole file should look like now:
```c++
#ifndef DriverExample_h
#define DriverExample_h

#include <Availability.h>
#include <DriverKit/IOService.iig>
#include <HIDDriverKit/IOUserHIDEventService.iig>

class DriverExample: public IOUserHIDEventService
{
public:
    virtual bool init() override;
    virtual void free() override;
    
    virtual kern_return_t
    Start(IOService * provider) override;
};

#endif /* DriverExample_h */
```

Now let's move on to our main source code file –– Your *.cpp file (DriverExample.cpp in my case). It should currently 
look like that:
```c++
#include <os/log.h>

#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>

kern_return_t
IMPL(DriverExample, Start)
{
    kern_return_t ret;
    ret = Start(provider, SUPERDISPATCH);
    os_log(OS_LOG_DEFAULT, "Hello World");
    return ret;
}
```

To work with a HID service we need some more includes, so let's add them:
```c++
#include <DriverKit/OSCollections.h>
#include <HIDDriverKit/HIDDriverKit.h>
```

When the system will instantiate your driver's service class, it its init method. Let's add code of this method: 
```c++
struct DriverExample_IVars
{
    OSArray *elements;
    
    struct {
        OSArray *elements;
    } keyboard;
};


bool DriverExample::init()
{
    if (!super::init()) {
        return false;
    }
    
    ivars = IONewZero(DriverExample_IVars, 1);
    if (!ivars) {
        return false;
    }
    
exit:
    return true;
}
```
(This code goes after the ```#include "YouProjectName.h"``` and before the implementation of the start of the service)

Here, during initialization time we allocate space for the driver's variables –– 
elements and a keyboard (that contains elements) in our case 
(here You can see variables in the ```DriverExample_IVars``` structure). 

So, following this example, You would need to define a structure with variables, that You driver requires and
and allocate this structure in the ```init()``` method.

We allocated instance variables for the keyboard driver, so now we need a method to free the memory from them
(further examples of code are from/based on code from [5]):
```c++
void DriverExample::free()
{
    if (ivars) {
        OSSafeReleaseNULL(ivars->elements);
        OSSafeReleaseNULL(ivars->keyboard.elements);
    }
    
    IOSafeDeleteNULL(ivars, DriverExample_IVars, 1);
    super::free();
}
```
This ```free()``` method will be called before unloading our service.

Now, let's start organizing our ```Start``` method:
```c++
kern_return_t
IMPL(DriverExample, Start)
{
   kern_return_t ret;
    
   ret = Start(provider, SUPERDISPATCH);
   if (ret != kIOReturnSuccess) {
      Stop(provider, SUPERDISPATCH);
      return ret;
   }
   
   //
   // Here the code of the startup tasks will go
   //

   RegisterService();
    
   return ret;
}
```
This method will be called, when the system will be ready to process information from the device.

In these method driver performs all the various start up tasks: variables initializations, changing device settings,
allocating memory for data buffers, et cetera.

Now let's add some start up tasks to the code. This code is based on the sample from [5].
```c++
kern_return_t
IMPL(DriverExample, Start)
{
    kern_return_t ret;
    
    ret = Start(provider, SUPERDISPATCH);
    if (ret != kIOReturnSuccess) {
        Stop(provider, SUPERDISPATCH);
        return ret;
    }

    os_log(OS_LOG_DEFAULT, "Hello from Your first DriverKit driver!");
    
    ivars->elements = getElements();
    if (!ivars->elements) {
        os_log(OS_LOG_DEFAULT, "Failed to get elements");
        Stop(provider, SUPERDISPATCH);
        return kIOReturnError;
    }
    
    ivars->elements->retain();

    os_log(OS_LOG_DEFAULT, "The startup task is now finished.");
    
    RegisterService();
    
    return ret;
}
```
To actually work with data from the keyboard, You would also need to parse arguments after retaining them. Parsing
sample code is also available at [5].


Congratulations! That is actually Your first DriverKit driver! Even though it doesnt really do anything with data 
from the keyboard (it just retains it) it is, nevertheless, a driver. 
Yet it is not The End –– in order to run that driver You need to perform some more, less code-oriented, steps.

*more detailed instruction coming soon...*

## Information about the driver and matching:
*coming soon...*

## Installing Your driver:
*coming soon...*

---

## Drivers using I/O Kit collection of frameworks:
*coming (moderately) soon...*

---

## Sources/literature:
1. "MAC OS X Internals: A Systems Approach" by Amit Singh ([link](https://www.oreilly.com/library/view/mac-os-x/0321278542/))
2. Modern Operating Systems, Andrew S. Tanenbaum (mostly chapter 5)
3. [Creating a Driver Using the DriverKit SDK](https://developer.apple.com/documentation/driverkit/creating_a_driver_using_the_driverkit_sdk) 
4. [System Extensions and DriverKit video presentation](https://developer.apple.com/videos/play/wwdc2019/702/)
5. [Handling Keyboard Events from a Human Interface Device](https://developer.apple.com/documentation/hiddriverkit/handling_keyboard_events_from_a_human_interface_device)


## Arranged by:
* [Daria Omelkina](https://github.com/dariaomelkina)

Special Thanks goes to everyone on the Apple team who created instruments, discussed in this example, 
documentation for them and code samples, and made everything available on the web.
