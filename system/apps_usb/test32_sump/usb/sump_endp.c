/**
 ******************************************************************************
 * @file    usb_endp.c
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Endpoint routines
 ******************************************************************************
 */
#include <usb_lib.h>
#include <usb_core.h>
#include <usb_mem.h>
#include "sump_desc.h"
#include "sump_app.h"
#include "sump_conf.h"

void USB_TxBufferHandler();
void USB_RxBufferHandler();

void EP1_IN_Callback()
{
  USB_TxBufferHandler();
}

void EP3_OUT_Callback()
{
  USB_RxBufferHandler();
}

void (*cdcpEpInt_IN[7])(void) = {EP1_IN_Callback, 0, 0, 0, 0, 0, 0 };
void (*cdcpEpInt_OUT[7])(void) = {0, 0, EP3_OUT_Callback, 0, 0, 0, 0 };
void (*cdcpCallbacks[8])(void) = {0, 0, 0, 0, 0, 0, 0, 0};

// UART -> USB
int onTransmitAvailable();
int onTransmitGet();

// USB -> UART
bool onReceiveAvailable(int);
void onReceivePut(int);

volatile uint8_t rx_buffer_new_data = 0;
volatile uint8_t tx_buffer_busy = 0;

bool isTransferPossible()
{
  return cdcbDeviceState == CONFIGURED;
}

void USB_TxBufferHandler(void)
{
  // package has been sent
  tx_buffer_busy = 0;

  // check for next package
  IRQ_Disable();
  if( !tx_buffer_busy && onTransmitAvailable() && isTransferPossible() ) {
    uint16_t *pma_addr = (uint16_t *)(PMAAddr + (ENDP1_TXADDR<<1));
    int sentSize = 0;
    do {
      int low = onTransmitGet();
      int high = 0;
      if (onTransmitAvailable())
      {
        high = onTransmitGet();
        sentSize += 2;
      } else
      {
        sentSize ++;
      }

      uint16_t word = low | (high<<8);

      *pma_addr++ = word;
      pma_addr++; // PMA strange write operation
    } while (onTransmitAvailable() && sentSize < VIRTUAL_COM_PORT_DATA_SIZE-2);

    if (sentSize > 0)
    {
      // send to IN pipe
      SetEPTxCount(ENDP1, sentSize);
      // send buffer
      SetEPTxValid(ENDP1);
      tx_buffer_busy = 1;
    }
  }

  IRQ_Enable();
}

void USB_RxBufferHandler(void)
{
  int count;

  // put package into buffer
  rx_buffer_new_data = 1;

  // atomic operation to avoid conflict with other interrupts
  IRQ_Disable();

  if( rx_buffer_new_data && (count=GetEPRxCount(ENDP3)) ) 
  {
    // check if buffer is free
    if( onReceiveAvailable(count) ) 
    {
      uint16_t *pma_addr = (uint16_t *)(PMAAddr + (ENDP3_RXADDR<<1));
      int data = 0;
      for(int i=0; i<count; ++i)
      {
        if ((i&1) == 0)
        {
          data = *pma_addr++;
          onReceivePut(data & 0xff);
        } else
        {
          data >>= 8;
          onReceivePut(data & 0xff);
        }
      }

      // notify, that data has been put into buffer
      rx_buffer_new_data = 0;

      // release OUT pipe
      SetEPRxValid(ENDP3);
    }
  }
  IRQ_Enable();
}
