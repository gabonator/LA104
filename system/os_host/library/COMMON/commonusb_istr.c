/**
 ******************************************************************************
 * @file	usb_istr.c
 * @author	MCD Application Team
 * @version V4.0.0
 * @date	21-January-2013
 * @brief	ISTR events interrupt service routines
 ******************************************************************************
 */
#include <usb_lib.h>
#include "commonusb_pwr.h"
#include "commonusb_istr.h"
#include "commonusb_app.h"

__IO uint8_t common_bIntPackSOF = 0;	/* SOFs received between 2 consecutive packets */
__IO uint32_t common_esof_counter = 0;	/* expected SOF counter */
DEVICE_PROP* common_Device_Property;

/******************************************************************************
 * @brief	ISTR events interrupt service routine
 */
void commonUSB_Istr(void)
{
	uint32_t i = 0;
	__IO uint32_t EP[8];

	uint16_t wIstr = _GetISTR();

#if (IMR_MSK & ISTR_SOF)
	if (wIstr & ISTR_SOF & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_SOF);
		common_bIntPackSOF++;
		if (common_Callback[ccSOF])
			common_Callback[ccSOF]();
	}
#endif

#if (IMR_MSK & ISTR_CTR)
	if (wIstr & ISTR_CTR & wInterrupt_Mask)
	{	/* servicing of the endpoint correct transfer interrupt */
		/* clear of the CTR flag into the sub */
		CTR_LP();
		if (common_Callback[ccCTR])
			common_Callback[ccCTR]();
	}
#endif

#if (IMR_MSK & ISTR_RESET)
	if (wIstr & ISTR_RESET & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_RESET);
		common_Device_Property->Reset();
		if (common_Callback[ccRESET])
			common_Callback[ccRESET]();
	}
#endif

#if (IMR_MSK & ISTR_DOVR)
	if (wIstr & ISTR_DOVR & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_DOVR);
		if (common_Callback[ccDOVR])
			common_Callback[ccDOVR]();
	}
#endif

#if (IMR_MSK & ISTR_ERR)
	if (wIstr & ISTR_ERR & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_ERR);
		if (common_Callback[ccERR])
			common_Callback[ccERR]();
	}
#endif

#if (IMR_MSK & ISTR_WKUP)
	if (wIstr & ISTR_WKUP & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_WKUP);
		common_Resume(RESUME_EXTERNAL);
		if (common_Callback[ccWKUP])
			common_Callback[ccWKUP]();
	}
#endif

#if (IMR_MSK & ISTR_SUSP)
	if (wIstr & ISTR_SUSP & wInterrupt_Mask)
	{
#ifdef USB_SUSPEND_ENABLE
		common_Suspend();
#else
		/* if not possible then resume after xx ms */
		common_Resume(RESUME_LATER);
#endif
		/* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
		_SetISTR((uint16_t)CLR_SUSP);
		if (common_Callback[ccSUSP])
			common_Callback[ccSUSP]();
	}
#endif

#if (IMR_MSK & ISTR_ESOF)
	if (wIstr & ISTR_ESOF & wInterrupt_Mask)
	{	/* clear ESOF flag in ISTR */
		_SetISTR((uint16_t)CLR_ESOF);

		if ((_GetFNR()&FNR_RXDP) != 0)
		{	/* increment ESOF counter */
			common_esof_counter ++;

			/* test if we enter in ESOF more than 3 times with FSUSP =0 and RXDP =1=>> possible missing SUSP flag*/
			if ((common_esof_counter > 3) && ((_GetCNTR() & CNTR_FSUSP)==0))
			{			
				/* this a sequence to apply a force RESET*/
				/* Store CNTR value */
				uint16_t wCNTR = _GetCNTR(); 

				/* Store endpoints registers status */
				for (i = 0; i < 8; i++) EP[i] = _GetENDPOINT(i);

				/*apply FRES */
				wCNTR |= CNTR_FRES;
				_SetCNTR(wCNTR);
 
				/* clear FRES */
				wCNTR &= ~CNTR_FRES;
				_SetCNTR(wCNTR);

				/* poll for RESET flag in ISTR */
				while((_GetISTR() & ISTR_RESET) == 0)
					;

				/* clear RESET flag in ISTR */
				_SetISTR((uint16_t)CLR_RESET);

				/* restore Enpoints */
				for (i = 0; i < 8; i++)
					_SetENDPOINT(i, EP[i]);

				common_esof_counter = 0;
			}
		}
		else
		{
			common_esof_counter = 0;
		}

		/* resume handling timing is made with ESOFs */
		common_Resume(RESUME_ESOF); /* request without change of the machine state */
		if (common_Callback[ccESOF])
			common_Callback[ccESOF]();
	}
#endif
} /* USB_Istr */