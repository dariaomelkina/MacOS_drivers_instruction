# :mage: Instruction for creating MacOS Drivers 
Operational systems course project at UCU.

### :ukraine: For Ukrainian translation click [here](ukrainian_version.md)

---

## Web version
*coming soon...*

## Table of contents
*coming soon...*

---
# Instruction:
Let's get started and enter the magical world of the drivers creation.

## Annotations:

## Notions and acronyms:
* **Dext** – driver extension.
* **Kext** – kernel extension.
* **SDK** – Software Development Kit.

## Introduction to drivers

## Why we might need to write drivers:

## Details on the task for MacOS:

## MacOS tools: 
In this instruction, we will discuss two possible options, regarding the choice of tools, when writing drivers for 
macOS systems. The first is the I/O kit –– a collection of frameworks libraries, tools, and other resources for 
creating device drivers, and the second is the DriverKit –– a modernized replacement of the I/O Kit.

## More about I/O kit:
When writing drivers for MacOS using I/O Kit, the drivers is essentially an I/O Kit object, which manages a specific
piece of hardware [[1](https://www.oreilly.com/library/view/mac-os-x/0321278542/)].

## More about DriverKit:
*Information about DriverKit is retrieved from and based on the official presentation of the kit, available by the
following [link](https://developer.apple.com/videos/play/wwdc2019/702/).* 

Driver extensions are built using DriverKit, they are now a replacement for I/O Kit device drivers.

They are used to control the following devices:
* USB
* Serial
* NIC (Network Interface Controller)
* HID (Human Interface Device)

As described in the video by the following [link](https://developer.apple.com/videos/play/wwdc2019/702/)
there are several advantages to using System Extensions and DriverKit:


We will try both approaches, starting with the newer, more secure, and, perhaps, an easier one –– DriverKit.

## Drivers using DriverKit framework:
*This –– the first version of the instruction –– is based on the official presentation and demonstration of the 
usage of the DriverKit, which is available at the following [link](https://developer.apple.com/videos/play/wwdc2019/702/).* 

## Dext lifecycle:

## Building:

## Specific example –– USB driver:

## Drivers using I/O Kit collection of frameworks:

## Sources/literature:
1. "MAC OS X Internals: A Systems Approach" by Amit Singh ([link](https://www.oreilly.com/library/view/mac-os-x/0321278542/))
2. Modern Operating Systems, Andrew S. Tanenbaum (mostly chapter 5)
3. 


## Arranged by:
* Daria Omelkina
