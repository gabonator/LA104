#include "cdcusb_app.h"
#include "cdcusb_desc.h"
#include "cdcusb_conf.h"

uint8_t	USB_Tx_Buffer [USB_TX_DATA_SIZE];
RingBuffer_t USB_Tx		= {	USB_Tx_Buffer, sizeof(USB_Tx_Buffer), 0, 0, 0	};
uint8_t	USB_Rx_Buffer [USB_TX_DATA_SIZE];
RingBuffer_t USB_Rx		= {	USB_Rx_Buffer, sizeof(USB_Rx_Buffer), 0, 0, 0	};

bool USB_Tx_Active = false;
extern DEVICE_INFO cdcDevice_Info;

void cdcGet_SerialNum(void)
{
}

bool RingBufferPut(RingBuffer_t * ring, uint8_t data)
{
	uint16_t next = ring->InIndex + 1;
	if (next == ring->Size)
		next = 0;

	if (next != ring->OutIndex)
	{
		__disable_irq();
		ring->Buffer[ring->InIndex] = data;
		ring->InIndex = next;
		__enable_irq();
		return true;
	}

	return false;
}

void USB_Config()
{
}

void USB_Cable_Config (FunctionalState NewState)
{
//dbgPushPrint("cable %d ", NewState);
}

// TODO: these functions are used as common by USB driver. Altering the DeviceState value !!! FIXIT!!!
void Enter_LowPowerMode(void)
{	/* Set the device state to suspend */
//TODO: check, called by mass storage or custom usb driver
//dbgPushPrint("enter_lp");
	cdcbDeviceState = SUSPENDED;
}

void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo = &cdcDevice_Info;

	/* Set the device state to the correct state */
	if (pInfo->Current_Configuration != 0)
		cdcbDeviceState = CONFIGURED;
	else
		cdcbDeviceState = ATTACHED;

//dbgPushPrint("leave_lp %d ", cdcbDeviceState);
}



void USB_Receive_Data(uint8_t * src, uint8_t length)
{
  for (int i=0; i<length; i++)
  {
    RingBufferPut(&USB_Tx, src[i]);
    dbgPushPrint("%c", src[i]);
  }
}

void Handle_USBAsynchXfer (void)
{
	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;
	RingBuffer_t * ring = &USB_Tx;
	if (USB_Tx_Active == false && ring->OutIndex != ring->InIndex)
	{
		if (ring->OutIndex > ring->InIndex)	/* rollback */
			ring->Length = ring->Size - ring->OutIndex;
		else
			ring->Length = ring->InIndex - ring->OutIndex;

		if (ring->Length > VIRTUAL_COM_PORT_DATA_SIZE)
		{
			USB_Tx_ptr = ring->OutIndex;
			USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

			ring->OutIndex += VIRTUAL_COM_PORT_DATA_SIZE;
			ring->Length -= VIRTUAL_COM_PORT_DATA_SIZE;
		}
		else
		{
			USB_Tx_ptr = ring->OutIndex;
			USB_Tx_length = ring->Length;

			ring->OutIndex += ring->Length;
			ring->Length = 0;
		}

		if (ring->OutIndex == ring->Size)
			ring->OutIndex = 0;
		
		USB_Tx_Active = true;
		UserToPMABufferCopy(&ring->Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
		SetEPTxCount(ENDP1, USB_Tx_length);
		SetEPTxValid(ENDP1);
	}
}
