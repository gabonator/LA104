#pragma once
/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK			(	CNTR_CTRM	\
						|	CNTR_WKUPM	\
						|	CNTR_SUSPM	\
						|	CNTR_ERRM	\
						|	CNTR_SOFM	\
						|	CNTR_ESOFM	\
						|	CNTR_RESETM	\
						)

