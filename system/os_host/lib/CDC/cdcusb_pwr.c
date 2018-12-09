/**
 ******************************************************************************
 * @file	usb_pwr.c
 * @author	MCD Application Team
 * @version	V4.0.0
 * @date	21-January-2013
 * @brief	Connection/disconnection & power management
 ******************************************************************************
 */
#include <usb_lib.h>
#include "cdcusb_conf.h"
#include "cdcusb_pwr.h"
#include "cdcusb_app.h"

bool remotewakeupon = false; // GABO

struct {
	__IO RESUME_STATE eState;
	__IO uint8_t bESOFcnt;
} CDC_ResumeS;

/******************************************************************************
 * @brief	PowerOn
 * @retval	USB_SUCCESS.
 */
USB_RESULT CDC_PowerOn(void)
{
	uint16_t wRegVal;

	wRegVal = CNTR_FRES;		/* CNTR_PWDN = 0 */
	_SetCNTR(wRegVal);

	wInterrupt_Mask = 0;		/* CNTR_FRES = 0 */
	_SetCNTR(wInterrupt_Mask);
	_SetISTR(0);				/* Clear pending interrupts */

	/* Set interrupt mask */
	wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
	_SetCNTR(wInterrupt_Mask);

	USB_Cable_Config(ENABLE);	/* cable plugged-in */

	return USB_SUCCESS;
}

/******************************************************************************
 * @brief	Handles switch-off conditions
 * @retval	USB_SUCCESS.
 */
USB_RESULT CDC_PowerOff()
{
	_SetCNTR(CNTR_FRES);				/* disable all interrupts and force USB reset */
	_SetISTR(0);						/* clear interrupt status register */

	USB_Cable_Config(DISABLE);			/* Disable the Pull-Up */
	_SetCNTR(CNTR_FRES + CNTR_PDWN);	/* switch-off device */

	/* sw variables reset */
	/* ... */

	return USB_SUCCESS;
}

/******************************************************************************
 * @brief	Sets suspend mode operating conditions
 * @retval	USB_SUCCESS.
 */
void CDC_Suspend(void)
{
	uint32_t i = 0;
	uint16_t wCNTR;
	__IO uint32_t savePWR_CR;

	/* suspend preparation */
	/* ... */
	
	/* Store CNTR value */
	wCNTR = _GetCNTR();	

	/* This a sequence to apply a force RESET to handle a robustness case */
	/* Store endpoints registers status */

	uint32_t eps[8]; // GABO
	for (i = 0; i < 8; i++)
		eps[i] = _GetENDPOINT(i); //pInformation->EP[i] = _GetENDPOINT(i);

	/* unmask RESET flag */
	wCNTR |= CNTR_RESETM;
	_SetCNTR(wCNTR);

	/* apply FRES */
	wCNTR |= CNTR_FRES;
	_SetCNTR(wCNTR);

	/* clear FRES */
	wCNTR &= ~CNTR_FRES;
	_SetCNTR(wCNTR);

	/* poll for RESET flag in ISTR */
	while ((_GetISTR() & ISTR_RESET) == 0)
		;

	/* clear RESET flag in ISTR */
	_SetISTR((uint16_t)CLR_RESET);

	/* restore Enpoints */
	for (i = 0; i < 8; i++)
		_SetENDPOINT(i, /*pInformation->EP[i]*/ eps[i]);

	/* Now it is safe to enter macrocell in suspend mode */
	wCNTR |= CNTR_FSUSP;
	_SetCNTR(wCNTR);

	/* force low-power mode in the macrocell */
	wCNTR = _GetCNTR();
	wCNTR |= CNTR_LPMODE;
	_SetCNTR(wCNTR);

	/* prepare entry in low power mode (STOP mode) */
	/* Select the regulator state in STOP mode*/
	savePWR_CR = PWR->CR;
	/* Clear PDDS and LPDS bits */
	/* Set LPDS bit according to PWR_Regulator value */
	PWR->CR = (savePWR_CR & ~(uint32_t)(PWR_CR_LPDS | PWR_CR_PDDS)) | PWR_Regulator_LowPower;

	/* Set SLEEPDEEP bit of Cortex System Control Register */
#if defined (STM32F30X) || defined (STM32F37X)
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
#else
	SCB->SCR |= SCB_SCR_SLEEPDEEP;
#endif

	/* enter system in STOP mode, only when wakeup flag in not set */
	if ((_GetISTR() & ISTR_WKUP) == 0)
	{
		__WFI();
		/* Reset SLEEPDEEP bit of Cortex System Control Register */
#if defined (STM32F30X) || defined (STM32F37X)
		SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
#else
		SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);
#endif
	}
	else
	{	/* Clear Wakeup flag */
		_SetISTR(CLR_WKUP);
		/* clear FSUSP to abort entry in suspend mode	*/
		wCNTR = _GetCNTR();
		wCNTR &= ~CNTR_FSUSP;
		_SetCNTR(wCNTR);

		/* restore sleep mode configuration */ 
		/* restore Power regulator config in sleep mode */
		PWR->CR = savePWR_CR;

		/* Reset SLEEPDEEP bit of Cortex System Control Register */
#if defined (STM32F30X) || defined (STM32F37X)		
		SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
#else
		SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP);
#endif
	}
}

/******************************************************************************
 * @brief	Handles wake-up restoring normal operations
 */
void CDC_Resume_Init(void)
{
	uint16_t wCNTR;

	/* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
	/* restart the clocks */
	/* ...	*/

	/* CNTR_LPMODE = 0 */
	wCNTR = _GetCNTR();
	wCNTR &= (~CNTR_LPMODE);
	_SetCNTR(wCNTR);

	/* restore full power */
	/* ... on connected devices */
	Leave_LowPowerMode();

	/* reset FSUSP bit */
	_SetCNTR(IMR_MSK);

	/* reverse suspend preparation */
	/* ... */
}

/******************************************************************************
 * @brief	This is the state machine handling resume operations and
 *			timing sequence. The control is based on the Resume structure
 *			variables and on the ESOF interrupt calling this subroutine
 *			without changing machine state.
 * @param	a state machine value (RESUME_STATE)
 *			RESUME_ESOF doesn't change ResumeS.eState allowing
 *			decrementing of the ESOF counter in different states.
 */
void CDC_Resume(RESUME_STATE eResumeSetVal)
{
	uint16_t wCNTR;

	if (eResumeSetVal != RESUME_ESOF)
		CDC_ResumeS.eState = eResumeSetVal;

	switch (CDC_ResumeS.eState)
	{
	case RESUME_EXTERNAL:
		if (remotewakeupon == false)
		{
			CDC_Resume_Init();
			CDC_ResumeS.eState = RESUME_OFF;
		}
		else
		{	 /* RESUME detected during the RemoteWAkeup signalling => keep RemoteWakeup handling */
			CDC_ResumeS.eState = RESUME_ON;
		}
		break;
	case RESUME_INTERNAL:
		CDC_Resume_Init();
		CDC_ResumeS.eState = RESUME_START;
		remotewakeupon = true;
		break;
	case RESUME_LATER:
		CDC_ResumeS.bESOFcnt = 2;
		CDC_ResumeS.eState = RESUME_WAIT;
		break;
	case RESUME_WAIT:
		CDC_ResumeS.bESOFcnt--;
		if (CDC_ResumeS.bESOFcnt == 0)
			CDC_ResumeS.eState = RESUME_START;
		break;
	case RESUME_START:
		wCNTR = _GetCNTR();
		wCNTR |= CNTR_RESUME;
		_SetCNTR(wCNTR);
		CDC_ResumeS.eState = RESUME_ON;
		CDC_ResumeS.bESOFcnt = 10;
		break;
	case RESUME_ON:	
		CDC_ResumeS.bESOFcnt--;
		if (CDC_ResumeS.bESOFcnt == 0)
		{
			wCNTR = _GetCNTR();
			wCNTR &= (~CNTR_RESUME);
			_SetCNTR(wCNTR);
			CDC_ResumeS.eState = RESUME_OFF;
			remotewakeupon = false;
		}
		break;
	case RESUME_OFF:
	case RESUME_ESOF:
	default:
		CDC_ResumeS.eState = RESUME_OFF;
		break;
	}
}