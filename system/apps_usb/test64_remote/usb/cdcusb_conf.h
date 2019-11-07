/**
 ******************************************************************************
 * @file    usb_conf.h
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Virtual COM Port Demo configuration  header
 ******************************************************************************
 */

#ifndef __USB_CONF_H
#define __USB_CONF_H

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#define EP_NUM			(4)

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
#define BTABLE_ADDRESS	(0x00)

/* EP0  */
/* rx/tx buffer base address */
#define ENDP0_RXADDR	(0x40)
#define ENDP0_TXADDR	(0x80)

/* EP1  */
/* tx buffer base address */
#define ENDP1_TXADDR	(0xC0)
#define ENDP2_TXADDR	(0x100)
#define ENDP3_RXADDR	(0x110)

/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK			(CNTR_CTRM | CNTR_RESETM)

#endif /* __USB_CONF_H */