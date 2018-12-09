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
#include "cdcusb_prop.h"
#include "cdcusb_pwr.h"
#include "cdcusb_istr.h"

extern __IO uint16_t wIstr;			/* ISTR register last read value */
__IO uint8_t cdcbIntPackSOF = 0;	/* SOFs received between 2 consecutive packets */
__IO uint32_t esof_counter = 0;	/* expected SOF counter */
__IO uint32_t wCNTR = 0;
extern DEVICE_PROP cdcDevice_Property;

/* function pointers to non-control endpoints service routines */
void (*cdcpEpInt_IN[7])(void) = {
	cdcEP1_IN_Callback,
	cdcEP2_IN_Callback,
	cdcEP3_IN_Callback,
	cdcEP4_IN_Callback,
	cdcEP5_IN_Callback,
	cdcEP6_IN_Callback,
	cdcEP7_IN_Callback,
};

void (*cdcpEpInt_OUT[7])(void) = {
	cdcEP1_OUT_Callback,
	cdcEP2_OUT_Callback,
	cdcEP3_OUT_Callback,
	cdcEP4_OUT_Callback,
	cdcEP5_OUT_Callback,
	cdcEP6_OUT_Callback,
	cdcEP7_OUT_Callback,
};

/******************************************************************************
 * @brief	ISTR events interrupt service routine
 */
void cdcUSB_Istr(void)
{
	uint32_t i = 0;
	__IO uint32_t EP[8];

	uint16_t wIstr = _GetISTR();

#if (IMR_MSK & ISTR_SOF)
	if (wIstr & ISTR_SOF & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_SOF);
		cdcbIntPackSOF++;
#	ifdef SOF_CALLBACK
		cdcSOF_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_CTR)
	if (wIstr & ISTR_CTR & wInterrupt_Mask)
	{	/* servicing of the endpoint correct transfer interrupt */
		/* clear of the CTR flag into the sub */
		CTR_LP();
#	ifdef CTR_CALLBACK
		cdcCTR_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_RESET)
	if (wIstr & ISTR_RESET & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_RESET);
		cdcDevice_Property.Reset();
#	ifdef RESET_CALLBACK
		cdcRESET_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_DOVR)
	if (wIstr & ISTR_DOVR & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_DOVR);
#	ifdef DOVR_CALLBACK
		cdcDOVR_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_ERR)
	if (wIstr & ISTR_ERR & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_ERR);
#	ifdef ERR_CALLBACK
		cdcERR_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_WKUP)
	if (wIstr & ISTR_WKUP & wInterrupt_Mask)
	{
		_SetISTR((uint16_t)CLR_WKUP);
		CDC_Resume(RESUME_EXTERNAL);
#	ifdef WKUP_CALLBACK
		cdcWKUP_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_SUSP)
	if (wIstr & ISTR_SUSP & wInterrupt_Mask)
	{
#ifdef USB_SUSPEND_ENABLE
		CDC_Suspend();
#else
		/* if not possible then resume after xx ms */
		CDC_Resume(RESUME_LATER);
#endif
		/* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
		_SetISTR((uint16_t)CLR_SUSP);
#	ifdef SUSP_CALLBACK
		cdcSUSP_Callback();
#	endif
	}
#endif

#if (IMR_MSK & ISTR_ESOF)
	if (wIstr & ISTR_ESOF & wInterrupt_Mask)
	{	/* clear ESOF flag in ISTR */
		_SetISTR((uint16_t)CLR_ESOF);

		if ((_GetFNR()&FNR_RXDP) != 0)
		{	/* increment ESOF counter */
			esof_counter ++;

			/* test if we enter in ESOF more than 3 times with FSUSP =0 and RXDP =1=>> possible missing SUSP flag*/
			if ((esof_counter > 3) && ((_GetCNTR() & CNTR_FSUSP)==0))
			{			
				/* this a sequence to apply a force RESET*/
				/* Store CNTR value */
				wCNTR = _GetCNTR(); 

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

				esof_counter = 0;
			}
		}
		else
		{
			esof_counter = 0;
		}

		/* resume handling timing is made with ESOFs */
		CDC_Resume(RESUME_ESOF); /* request without change of the machine state */

#	ifdef ESOF_CALLBACK
		cdcESOF_Callback();
#	endif
	}
#endif
} /* USB_Istr */