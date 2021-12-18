/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implements the service object that dispatches keyboard events to the system.
*/

#include <os/log.h>
#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>
#include <DriverKit/OSCollections.h>
#include <HIDDriverKit/HIDDriverKit.h>

#include "HIDKeyboardDriver.h"

/* struct HIDKeyboardDriver_IVars
 *
 * This structure contains the instance variables for a HIDKeyboardDriver object.
 */
struct HIDKeyboardDriver_IVars
{
    OSArray *elements;
    
    struct {
        OSArray *elements;
    } keyboard;
};

#define _elements   ivars->elements
#define _keyboard   ivars->keyboard

/* init
 *
 * Initializes the object by allocating memory for its instance variables.
 */
bool HIDKeyboardDriver::init()
{
    if (!super::init()) {
        return false;
    }
    
    ivars = IONewZero(HIDKeyboardDriver_IVars, 1);
    if (!ivars) {
        return false;
    }
    
exit:
    return true;
}

/* free
 *
 * Releases the memory for the object's instance variables.
 */
void HIDKeyboardDriver::free()
{
    if (ivars) {
        OSSafeReleaseNULL(_elements);
        OSSafeReleaseNULL(_keyboard.elements);
    }
    
    IOSafeDeleteNULL(ivars, HIDKeyboardDriver_IVars, 1);
    super::free();
}

/* Start
 *
 * Starts the service by getting and parsing the IOHIDElement objects
 *  that the parent event service provides. These elements contain the
 *  information from the device's most recent input report. If the elements
 *  contain keyboard-related data, this method registers the service with
 *  the system, which allows it to handle future input reports for this device.
 */
/// - Tag: Start
kern_return_t
IMPL(HIDKeyboardDriver, Start)
{
    kern_return_t ret;
    
    ret = Start(provider, SUPERDISPATCH);
    if (ret != kIOReturnSuccess) {
        Stop(provider, SUPERDISPATCH);
        return ret;
    }

    os_log(OS_LOG_DEFAULT, "Hello World");
    
    _elements = getElements();
    if (!_elements) {
        os_log(OS_LOG_DEFAULT, "Failed to get elements");
        Stop(provider, SUPERDISPATCH);
        return kIOReturnError;
    }
    
    _elements->retain();
    
    if (!parseElements(_elements)) {
        os_log(OS_LOG_DEFAULT, "No supported elements found");
        Stop(provider, SUPERDISPATCH);
        return kIOReturnUnsupported;
    }
    
    RegisterService();
    
    return ret;
}

/* parseElements
 *
 * This method parses the specified array of IOHIDElement elements, looking for
 *  elements that contain keyboard data. If it finds any, it returns true;
 *  otherwise, it returns false.
 */
/// - Tag: parseElements
bool HIDKeyboardDriver::parseElements(OSArray *elements)
{
    bool result = false;
    
    for (unsigned int i = 0; i < elements->getCount(); i++) {
        IOHIDElement *element = NULL;
        
        element = OSDynamicCast(IOHIDElement, elements->getObject(i));
        
        if (!element) {
            continue;
        }
        
        if (element->getType() == kIOHIDElementTypeCollection ||
            !element->getUsage()) {
            continue;
        }
        
        if (parseKeyboardElement(element)) {
            result = true;
        }
    }
    
    return result;
}


/* parseKeyboardElement
 *
 * This method examines the element to determine if it contains
 *  keyboard-related data, returning true if it does. The method
 *  also saves a reference to the element in the object's instance
 *  variables.
 */
/// - Tag: parseKeyboardElement
bool HIDKeyboardDriver::parseKeyboardElement(IOHIDElement *element)
{
    bool result = false;
    uint32_t usagePage = element->getUsagePage();
    uint32_t usage = element->getUsage();
    
    // Determine whether the element contains keyboard-related data. 
    if (usagePage == kHIDPage_KeyboardOrKeypad) {
        if (usage >= kHIDUsage_KeyboardA &&
            usage <= kHIDUsage_KeyboardRightGUI) {
            result = true;
        }
    }
    
    if (!result) {
        return false;
    }
    
    if (!_keyboard.elements) {
        _keyboard.elements = OSArray::withCapacity(4);
    }
    
    // Save a reference to the element for later.
    _keyboard.elements->setObject(element);
    
exit:
    return result;
}

/* handleReport
 *
 * This overridden method receives the input data from the device, and
 *  hands it off to the handleKeyboardReport method for processing.
 */
/// - Tag: handleReport
void HIDKeyboardDriver::handleReport(uint64_t timestamp,
                                     uint8_t *report __unused,
                                     uint32_t reportLength __unused,
                                     IOHIDReportType type,
                                     uint32_t reportID)
{
    handleKeyboardReport(timestamp, reportID);
}


/* handleKeyboardReport
 *
 * This method processes the subset of elements containing keyboard data.
 *  By the time the driver calls this method, the parent class has already
 *  updated the IOHIDElement objects that you retrieved in your Start method.
 *  As a result, each element contains data from the most recent input report.
 */
/// - Tag: handleKeyboardReport
void HIDKeyboardDriver::handleKeyboardReport(uint64_t timestamp,
                                             uint32_t reportID)
{
    if (!_keyboard.elements) {
        return;
    }
    
    // Iterate over the elements that contain keyboard data.
    for (unsigned int i = 0; i < _keyboard.elements->getCount(); i++) {
        IOHIDElement *element = NULL;
        uint64_t elementTimeStamp;
        uint32_t usagePage, usage, value, preValue;
        
        element = OSDynamicCast(IOHIDElement, _keyboard.elements->getObject(i));
        
        if (!element) {
            continue;
        }
        
        // If the element doesn't contain new data, skip it.
        if (element->getReportID() != reportID) {
            continue;
        }
        
        elementTimeStamp = element->getTimeStamp();
        if (timestamp != elementTimeStamp) {
            continue;
        }
        
        // Get the previous value of the element.
        preValue = element->getValue(kIOHIDValueOptionsFlagPrevious) != 0;
        value = element->getValue(0) != 0;
        
        // If the element's value didn't change, skip it.
        if (value == preValue) {
            continue;
        }
        
        // If the code reaches this point, the element has new data, so
        //  dispatch an event with the new value to the system.
        usagePage = element->getUsagePage();
        usage = element->getUsage();
        
        os_log(OS_LOG_DEFAULT,
               "Dispatching key with ussagPage: 0x%02x usage: 0x%02x value: %d",
               usagePage, usage, value);
        dispatchKeyboardEvent(timestamp, usagePage, usage, value, 0, true);
    }
    
exit:
    return;
}
