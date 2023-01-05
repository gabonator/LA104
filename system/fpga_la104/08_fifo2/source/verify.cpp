#include "../../common/testbench.h"

#define STATUS_FPGA_RECV_OFFSET 6 //fpga has received data
#define STATUS_FPGA_SEND_OFFSET 7 //fpga has sent data

#define STATUS_FPGA_RECV_MASK (0x1<<STATUS_FPGA_RECV_OFFSET)
#define STATUS_FPGA_SEND_MASK (0x1<<STATUS_FPGA_SEND_OFFSET)


#define SPI_NOP 0x00
#define SPI_INIT 0x01
#define SPI_SEND_BIT_INV 0x02
#define SPI_READ_REQ_BIT_INV 0x03
#define SPI_SET_LED 0x04
#define SPI_READ_REQ_LED 0x05
#define SPI_SEND_VEC 0x06
#define SPI_READ_VEC 0x07

CFpgaTestBench test;

void xfer_spi(uint8_t *data, int n)
{
    // first transfer is corrupted on real device for unknown reason (ff at beginning)
    if (n==4)
      Verilator::GroupBegin("Transfer %02x, %02x, %02x, %02x", data[0], data[1], data[2], data[3]);
    else
      Verilator::GroupBegin("Transfer %02x", data[0]);
    for (int i=0; i<n; i++)
    {
      uint8_t wr = data[i];
      uint8_t rd = test.SpiTransfer(wr);
      data[i] = rd;
    }
    if (n==4)
      Verilator::PrintTrace("Received %02x, %02x, %02x, %02x", data[0], data[1], data[2], data[3]);
    else
      Verilator::PrintTrace("Received %02x", data[0]);
    Verilator::GroupEnd();
}

int spi_read(uint8_t val[3], uint8_t *status)
{
   uint8_t nop_command[] = {0x00, 0x00, 0x00, 0x00}; //nop
   uint8_t status_recv = 0;
   Verilator::PrintTrace("####RD ");

   xfer_spi(nop_command, 4);
   status_recv = nop_command[0];

   if (!(status_recv & STATUS_FPGA_SEND_MASK))
     Verilator::PrintTrace("Status without SEND mask (%02x, %02x)", status_recv, STATUS_FPGA_SEND_MASK);

   val[0] = nop_command[1];
   val[1] = nop_command[2];
   val[2] = nop_command[3];

   if(status != NULL)
      *status = status_recv;

   return true;
}

int spi_send(uint8_t cmd, uint8_t val[3], uint8_t *status)
{
   uint8_t to_send[] = {cmd, val[0], val[1], val[2]};
   uint8_t status_recv = 0;

   Verilator::PrintTrace("####SE (%02x %02x %02x %02x)", cmd, val[0], val[1], val[2]);

   to_send[0] = cmd;
   to_send[1] = val[0];
   to_send[2] = val[1];
   to_send[3] = val[2];
   xfer_spi(to_send, 4);
   status_recv = to_send[0];
   
   if(status != NULL)
      *status = status_recv;
   return true;
}

int spi_send24b(uint8_t cmd, uint32_t val24b, uint8_t *status)
{
   uint8_t param[3] = {uint8_t(val24b&0xff), uint8_t((val24b>>8)&0xff), uint8_t((val24b>>16)&0xff)};
   return spi_send(cmd, param, status);
}

bool verify()
{
    uint8_t data;
    test.SpiBegin(true);
#ifndef _VERILATOR
    // skip first byte
    test.SpiCs(true);
    Verilator::PrintTrace("Sending first dummy byte 00");
    xfer_spi(&data, 1);
    test.SpiCs(false);
    test.SpiCs(false);
#endif
    test.SpiCs(false);
    // give some time to init
    Verilator::PrintTrace("Starting!");
    test.SpiCs(true);
    test.SpiCs(true);
    test.SpiCs(true);

    data = 0x13;
    BIOS::DBG::Print("wr: 0x%02x ", data);
    xfer_spi(&data, 1);
    BIOS::DBG::Print("rd: 0x%02x\n", data);
    data = 0xab;
    BIOS::DBG::Print("wr: 0x%02x ", data);
    xfer_spi(&data, 1);
    BIOS::DBG::Print("rd: 0x%02x\n", data);

    data = 0xcd;
    BIOS::DBG::Print("wr: 0x%02x ", data);
    xfer_spi(&data, 1);
    BIOS::DBG::Print("rd: 0x%02x\n", data);
    test.SpiCs(false);
    test.SpiEnd();
    return true;
}