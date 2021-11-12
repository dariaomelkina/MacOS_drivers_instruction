# :mage: Instruction for creating macOS Drivers 
Operational systems course project at UCU.

*Warning: instruction is unfinished.*

### :ukraine: For Ukrainian translation click [here](ukrainian_version.md)

---

## Web version
*coming soon...*

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
    1. [Dext lifecycle](#dext-lifecycle)
    1. [Building](#building)
    1. [Specific example](#specific-example)
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

Typical Unix systems provide a file-system-based user interface to devices—a user-space process addresses a device through device special files (or simply device files) that conventionally reside in the /dev/ directory. Older systems had a static /dev/, wherein the constituent device files were explicitly created or deleted, and device major numbers were statically assigned. Newer systems, including Mac OS X, manage devices more dynamically. For example, the Mac OS X device file system allows device files to be dynamically created or deleted and major numbers to be automatically assigned on device file creation.

## macOS tools: 
In this instruction, we will discuss two possible options, regarding the choice of tools, when writing drivers for 
macOS systems. The first is the I/O kit –– a collection of frameworks libraries, tools, and other resources for 
creating device drivers, and the second is the DriverKit –– a modernized replacement of the I/O Kit.

## More about I/O kit:
When writing drivers for macOS using I/O Kit, the drivers is essentially an I/O Kit object, which manages a specific
piece of hardware [[1](https://www.oreilly.com/library/view/mac-os-x/0321278542/)].

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


## Drivers using DriverKit framework:
*This –– the first version of the instruction –– is based on the official presentation and demonstration of the 
usage of the DriverKit, which is available at the following [link](https://developer.apple.com/videos/play/wwdc2019/702/).* 

## Dext lifecycle:
*coming soon...*

## Building:
*coming soon...*

## Specific example:

## Drivers using I/O Kit collection of frameworks:
*coming soon...*

## Sources/literature:
1. "MAC OS X Internals: A Systems Approach" by Amit Singh ([link](https://www.oreilly.com/library/view/mac-os-x/0321278542/))
2. Modern Operating Systems, Andrew S. Tanenbaum (mostly chapter 5)
3. [System Extensions and DriverKit video presentation](https://developer.apple.com/videos/play/wwdc2019/702/)


## Arranged by:
* Daria Omelkina
