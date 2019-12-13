#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

extern void cdc_setup(usbd_device* usbd_dev);

#endif  //  CDC_H_INCLUDED
