# USB/KeyboardAndMouse

A simple keyboard and mouse simulation library.  

## Port guide

This library requires a few procedures to integrate into your program.  

### 1. Return the HID Report Descriptors given in library

HID devices need specific Report Descriptors to let host know how to understand data received. This library's keyboard and mouse HID Report implemention is compatible with BIOS input device format.  

```c
/* Descriptors */
extern const uint8_t KeyRepDesc[];
extern const uint8_t MouseRepDesc[];
```

### 2. Implement portable part

Users may use different USB frameworks. You may need to change code part below to fit your USB framework.  

```c
// Portable implemention
// - You need to implement things below to integrate into your program.
// ##PORT_IMPLEMENTION_BEGIN##

#include <ch32x035_usbfs_device.h>
extern volatile uint8_t USBFS_Endp_Busy[];

// Define the endpoint for keyboard instance.
#define HID_KB_EP DEF_UEP1

// Define the endpoint for mouse instance.
#define HID_MS_EP DEF_UEP2

// Return busy state for given endpoint. 0 for idle, other for busy.
uint8_t HID_KM_GetEndpointBusy(uint8_t ep)
{
    return USBFS_Endp_Busy[ep];
}

// Prepare data for IN endpoint upload.
uint8_t HID_KM_EpUpload(uint8_t ep, uint8_t *buf, uint16_t len)
{
    return USBFS_Endp_DataUp(ep, buf, len, DEF_UEP_CPY_LOAD);
}

// This should be called while OUT transaction for keyboard has occured.
void HID_KB_EpOut_Callback(uint8_t *buf, uint16_t len){
    HID_KB_LED_Status = buf[0];
}

// ##PORT_IMPLEMENTION_END##
```

### 3. Call EpOut_Callback in your USB framework handler

HID keyboards use OUT transcations to get indicated while LEDs(NumsLock, CapsLock, ScrollLock) get changed. If you need to access LED status, please call `void HID_KB_EpOut_Callback(uint8_t *buf, uint16_t len)` in **Endpoint 0 OUT stage, Class Request, HID_SET_REPORT** case, which is like below:

```c
// more code above....
/* end-point 0 data out interrupt */
case USBFS_UIS_TOKEN_OUT | DEF_UEP0:
    if (intst & USBFS_UIS_TOG_OK)
    {
        if ((USBFS_SetupReqType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD)
        {
            if ((USBFS_SetupReqType & USB_REQ_TYP_MASK) == USB_REQ_TYP_CLASS)
            {
                switch (USBFS_SetupReqCode)
                {
                    case HID_SET_REPORT:
                        HID_KB_EpOut_Callback(USBFS_EP0_Buf, USBFS_SetupReqLen);
                        USBFS_SetupReqLen = 0;
                        break;
// more code below....
```
