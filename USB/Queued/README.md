# USB/Queued

A queued processing middleware library. It can help you handle command-response style USB communication easier.  

## Port guide

This library requires a few procedures to integrate into your program.  

### 1. Implement portable part

Users may use different USB frameworks. You may need to change code part below to fit your USB framework.  

```c
// Portable implemention
// - You need to implement things below to integrate into your program.
// ##PORT_IMPLEMENTION_BEGIN##

#include <ch32x035_usbfs_device.h>

/*
 * UQ_EP_UP: Defines the endpoint to upload data
 */
#define UQ_EP_UP    DEF_UEP1

/*
 * UQ_EP_DN: Defines the endpoint to receive data
 */
#define UQ_EP_DN    DEF_UEP2

extern volatile uint8_t USBFS_Endp_Busy[];

// Prepare data for IN endpoint upload.
uint8_t USBQueue_EPUpload(uint8_t *buf, uint16_t len)
{
 while(USBFS_Endp_Busy[UQ_EP_UP])
 { ;}
 return USBFS_Endp_DataUp(UQ_EP_UP, buf, len, DEF_UEP_DMA_LOAD);
}

// Prepare buffer for OUT transaction.
void USBQueue_SetEPDNAddr(uint8_t *buffer)
{
 USBFSD->UEP2_DMA = (uint32_t) buffer;
}

// Control the OUT(downstream) endpoint ACK status.
void USBQueue_SetEPDNAck(FunctionalState state)
{
 if (DISABLE == state)
 {
  // NAK
  USBFSD->UEP2_CTRL_H &= ~USBFS_UEP_R_RES_MASK;
  USBFSD->UEP2_CTRL_H |= USBFS_UEP_R_RES_NAK;
 }
 else
 {
  // ACK
  USBFSD->UEP2_CTRL_H &= ~USBFS_UEP_R_RES_MASK;
  USBFSD->UEP2_CTRL_H |= USBFS_UEP_R_RES_ACK;
 }
}

// This function should be re-implemented as your need.
__attribute__((weak))
  uint8_t USBQueue_UserProcessor(uint8_t *inData,
  uint8_t inLen, uint8_t *outData)
{
 uint8_t outLen = 0;
 /* Handle request here.
  *  - Input data:
  *    - uint8_t * inData :  incoming USB packet
  *    - uint8_t inLen  : length of incoming USB packet
  *  - Output data:
  *    - uint8_t *outData : USB packet you want to response
  *    - [return value]  : length of your response packet
  */

 return outLen;
}

// ##PORT_IMPLEMENTION_END##
```

### 2. Call EpIN_Handler and EpOUT_Handler in your USB framework handler

This library need 2 handlers for USB data processing.  
You need to call them while IN transaction done or OUT transaction occurs, which is like below:

```c
// more code above....

/* end-point 1 data in interrupt */
case ( USBFS_UIS_TOKEN_IN | DEF_UEP1):
 USBFSD->UEP1_CTRL_H ^= USBFS_UEP_T_TOG;
 USBFSD->UEP1_CTRL_H = (USBFSD->UEP1_CTRL_H & ~USBFS_UEP_T_RES_MASK) | USBFS_UEP_T_RES_NAK;
 USBFS_Endp_Busy[ DEF_UEP1] = 0;
 USBQueue_EpIN_Handler();
 break;

// more code here not shown....

/* end-point 2 data out interrupt */
case USBFS_UIS_TOKEN_OUT | DEF_UEP2:
 USBFSD->UEP2_CTRL_H ^= USBFS_UEP_R_TOG;
 USBQueue_EpOUT_Handler(USBFSD->RX_LEN);
 break;

// more code below....
```

### 3. Call USBQueue_StatusReset before start

To start or restart queue, call USBQueue_StatusReset() first.  
You can add this to USB init function or manually call as need.

### 5. Periodically run USBQueue_DoProcess

For no-OS environments, please call USBQueue_DoProcess() periodically at an acceptable interval. Unless this function is called, no data will be processed. This function is non-blocking, and it returns how many queued data processed.
