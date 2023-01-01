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
    Verilator::GroupBegin("Transfer %02x, %02x, %02x, %02x", data[0], data[1], data[2], data[3]);
    for (int i=0; i<n; i++)
    {
      uint8_t wr = data[i];
      uint8_t rd = test.SpiTransfer(wr);
      data[i] = rd;
    }
    Verilator::PrintTrace("Received %02x, %02x, %02x, %02x", data[0], data[1], data[2], data[3]);
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
    test.SpiBegin(true);
    test.SpiCs(true);
    uint8_t no_param[3] = {0x0, 0x0, 0x0};
    uint8_t spi_status = 0;
    uint8_t data_read[3];
    uint8_t val_inv[3] = {0x38, 0xAE, 0x3B};
    uint8_t val_led_yellow[3] = {0x0, 0x0, 0x3};
    uint8_t val_led_blue[3] = {0x0, 0x0, 0x4};
    bool ok = true;

    Verilator::PrintTrace("Init");
    spi_send(SPI_INIT, no_param, NULL); // init

    Verilator::PrintTrace("SPI_SEND_BIT_INV");
    spi_send(SPI_SEND_BIT_INV, val_inv, &spi_status); // send values bit inversion
    Verilator::PrintTrace("send inv data, status: 0x%x\n", spi_status);

    spi_send(SPI_READ_REQ_BIT_INV, no_param, NULL); //send read request

    Verilator::PrintTrace("---- READ INVERSION BEGIN");
    spi_read(data_read, &spi_status); // read data inversion
    Verilator::PrintTrace("---- READ INVERSION END");

    for (size_t i = 0; i < 3; i++)
      if (data_read[i] != (0xFF&~val_inv[i]))
      {
         BIOS::DBG::Print("Error: inv read %d: 0x%x, should be 0x%x\n", i, data_read[i], 0xFF&~val_inv[i]);
         ok = false;
      }

    Verilator::PrintTrace("---- YELLOW LED");
    spi_send(SPI_SET_LED, val_led_yellow, &spi_status); // led yellow
    Verilator::PrintTrace("send yellow led, status: 0x%x\n", spi_status);

    Verilator::PrintTrace("---- READ LED");
    spi_send(SPI_READ_REQ_LED, no_param, NULL); //send led read request

    Verilator::PrintTrace("---- LED DATA");
    spi_read(data_read, &spi_status); // read led data
    Verilator::PrintTrace("led_data: 0x%x, 0x%x, 0x%x, status:0x%x\n", data_read[2], data_read[1], data_read[0], spi_status);
    for (size_t i = 0; i < 3; i++)
      if (data_read[i] != val_led_yellow[i])
      {
         BIOS::DBG::Print("Error: yellow read %d: 0x%x, should be 0x%x\n", i, data_read[i], val_led_yellow[i]);
         ok = false;
      }

    spi_send(SPI_SET_LED, val_led_blue, &spi_status); // set led blue
    Verilator::PrintTrace("send blue led, status: 0x%x\n", spi_status);
    Verilator::PrintTrace("---- READ LED");
    spi_send(SPI_READ_REQ_LED, no_param, NULL); //send led read request

    Verilator::PrintTrace("---- LED DATA");
    spi_read(data_read, &spi_status); // read led data

    for (size_t i = 0; i < 3; i++)
      if (data_read[i] != val_led_blue[i])
      {
         BIOS::DBG::Print("Error: blue read %d: 0x%x, should be 0x%x\n", i, data_read[i], val_led_blue[i]);
         ok = false;
      }

    //send 4 values the fastest possible
    for (size_t i = 0; i < 4; i++) 
    {
       int send_value = (i+1)*16;
       spi_send24b(SPI_SEND_VEC, send_value, &spi_status);
       Verilator::PrintTrace("vector send: 0x%x, status: 0x%x\n", send_value, spi_status);
    }

    //send read request, the fpga will send the 4 values
    spi_send(SPI_READ_VEC, no_param, &spi_status);
    Verilator::PrintTrace("read req vector, status: 0x%x\n", spi_status);

    spi_read(data_read, &spi_status);
    Verilator::PrintTrace("read req vector, skip status: 0x%x\n", spi_status);
    //read values the fastest possible
    for (size_t i = 0; i < 4; i++) {
       spi_read(data_read, &spi_status);
       if (data_read[0] != (i+1)*16 || data_read[1] != 0 || data_read[2] != 0)
       {
         ok = false;
         BIOS::DBG::Print("error vect read: 0x%x, 0x%x, 0x%x, status:0x%x\n", data_read[2], data_read[1], data_read[0], spi_status);
       }
    }

    test.SpiCs(false);
    test.SpiEnd();
    return ok;
}