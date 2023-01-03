//`include "spi_slave.v"

//opcodes:
//0x00 nop
//0x01 init
//0x02 write 16bits inverted
//0x03 read 16bits inverted
//0x04 write leds (16bits LSB)
//0x05 read leds (16bits LSB)
//0x06 write vector, the computer will send 4 * 24bit values
//0x07 read vector, the fpga will send 4 * 24bit values


module app(input clk, input SCK, input SSEL, input MOSI, output MISO);


//module top(input [3:0] SW, input clk, output LED_R, output LED_G, output LED_B, input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO, input [3:0] SW);

   reg [3:0] wSW = 0;
   reg wLED_R = 0;
   reg wLED_G = 0;
   reg wLED_B = 0;
   wire [3:0] SW = wSW[3:0];
   wire LED_R = wLED_R;
   wire LED_G = wLED_G;
   wire LED_B = wLED_B;

   reg spi_reset;
   wire spi_wr_buffer_free;
   reg spi_wr_en;
   reg [23:0] spi_wr_data;
   wire spi_rd_data_available;
   reg spi_rd_data_available_buf;
   reg spi_rd_ack;
   wire [31:0] spi_rd_data;

   parameter /*NOP=0, INIT=1, */WR_INVERTED=2, RD_INVERTED=3, WR_LEDS=4, RD_LEDS=5, WR_VEC=6, RD_VEC=7;

   spi_slave spi_slave_inst(.clk(clk), .reset(spi_reset),
      .SPI_SCK(SCK), .SPI_SS(SSEL), .SPI_MOSI(MOSI), .SPI_MISO(MISO),
      .wr_buffer_free(spi_wr_buffer_free), .wr_en(spi_wr_en), .wr_data(spi_wr_data),
      .rd_data_available(spi_rd_data_available), .rd_ack(spi_rd_ack), .rd_data(spi_rd_data)
   );

   reg [2:0] led;

   reg [31:0] spi_recv_data_reg;
   reg handle_data;

   reg [23:0] reg_bits_inversion;

   reg [23:0] vector [0:4];
   reg [2:0] vec_ptr;
   reg sending_vector;

   assign LED_R = ~led[0];
   assign LED_G = ~led[1];
   assign LED_B = ~led[2];

   integer i;

   initial begin

      for(i = 0; i < 4; i=i+1) begin
         vector[i] = 0;
      end

      spi_reset = 0;
      spi_wr_en = 0;
      spi_wr_data = 0;
      spi_rd_ack = 0;

      vec_ptr = 0;
      sending_vector = 0;

      led = 0;
      spi_recv_data_reg = 0;
      handle_data = 0;
   end

   always @(posedge clk)
   begin

      //defaults
      spi_rd_ack <= 0;
      spi_wr_en <= 0;

      spi_rd_data_available_buf <= spi_rd_data_available;

      if(spi_rd_data_available == 1 && spi_rd_data_available_buf == 0) begin // rising edge
         spi_recv_data_reg <= spi_rd_data;
         spi_rd_ack <= 1;
         handle_data <= 1;
      end

      //sends the 4-24bit vector with spi
      if(sending_vector == 1 && spi_wr_buffer_free == 1) begin
            $write("---- pushing vector idx %d = %06x", vec_ptr, vector[vec_ptr]);

         spi_wr_en <= 1;
         spi_wr_data[23:0] <= vector[vec_ptr];
         if(vec_ptr < 3) begin
            vec_ptr <= vec_ptr+1;
         end else begin
            vec_ptr <= 0;
            sending_vector <= 0;
         end
      end

      if(handle_data == 1) begin
         $write("---- top:handle_data=%d, spi_recv_data_reg=%08x", handle_data, spi_recv_data_reg);
         case(spi_recv_data_reg[7:0])
            WR_INVERTED: begin
            $write("---- WR_INVERTED\n");

               reg_bits_inversion[23:0] <= ~spi_recv_data_reg[31:8];
            end
            RD_INVERTED: begin
            $write("---- RD_INVERTED, sending: %06x\n", reg_bits_inversion[23:0]);

               spi_wr_en <= 1;
               spi_wr_data[23:0] <= reg_bits_inversion[23:0];
            end
            WR_LEDS: begin
            $write("---- WR_LEDS %x\n", spi_recv_data_reg[26:24]);

               led[2:0] <= spi_recv_data_reg[26:24];
            end
            RD_LEDS: begin
            $write("---- RD_LEDS %x\n", led[2:0]);

               spi_wr_en <= 1;
               spi_wr_data[23:0] <= {5'b0, led[2:0], 16'b0};
            end
            WR_VEC: begin
            $write("---- WR_REC [%d] = %06x\n", vec_ptr, spi_recv_data_reg[31:8]);

               vector[vec_ptr] <= spi_recv_data_reg[31:8];
               if(vec_ptr < 3)
               begin
                  vec_ptr <= vec_ptr+1;
               end else begin
                  vec_ptr <= 0;
               end
            end
            RD_VEC: begin
            $write("---- RD_VEC\n");

               sending_vector <= 1;
            end
         endcase
         handle_data <= 0;
      end
   end

endmodule
