#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

typedef void (*cdc_receive_callback)(uint8_t* buf, int len);

void cdc_setup(usbd_device* usbd_dev);
void cdc_set_receive_callback(cdc_receive_callback callback);
bool cdc_transmit(uint8_t* buffer, int len);
#endif  //  CDC_H_INCLUDED
