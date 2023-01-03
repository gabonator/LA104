// receive: byte2 | byte1 | byte0 | opcode/status
//read all the data, but can write only the two bytes as opcode contains metadata

module spi_slave(input wire clk, input wire reset,
      input wire SPI_SCK, input wire SPI_SS, input wire SPI_MOSI, output wire SPI_MISO,
      output wire wr_buffer_free, input wire wr_en, input wire [23:0] wr_data,
      output reg rd_data_available, input wire rd_ack, output reg [31:0] rd_data
   );

   reg [4:0] counter_read; //max 32

   reg [1:0] spi_clk_reg;
   reg [1:0] spi_ss_reg;
   wire spi_ss_falling_edge;
   wire spi_ss_rising_edge;

   reg [1:0] mosi_reg;
   reg miso_out_reg;
   reg [7:0] state_rd;

   reg wr_reg_full;
   reg [23:0] wr_data_reg; //written data to send to spi/miso
   reg wr_queue_full;
   reg [23:0] wr_data_queue; //waiting to be written in the register, avoid a write while communcating with SPI

   reg buffer_rd_ack;
   reg [31:0] rd_data_local;

   //states
   parameter IDLE = 0, INIT=IDLE+1, RD_WAIT_DATA=INIT+1/*, RD_WAIT_ACK=RD_WAIT_DATA+1, WR_WAIT_DATA=RD_WAIT_DATA+1+1, WR_WAIT_ACK=WR_WAIT_DATA+1*/;

   assign SPI_MISO = miso_out_reg;
   wire spi_clk_rising_edge;
   wire spi_clk_falling_edge;
   assign spi_clk_rising_edge = (spi_clk_reg[1:0] == 2'b01);
   assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);
   assign spi_ss_rising_edge = (spi_ss_reg[1:0] == 2'b01);
   assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);

   initial begin
      counter_read = 0;
      spi_clk_reg = 0;
      spi_ss_reg = 0;
      mosi_reg = 0;
      miso_out_reg = 0;
      state_rd = INIT;
      wr_reg_full = 0;
      wr_data_reg = 24'hcafe77;
      wr_queue_full = 0;
      wr_data_queue = 0;

      buffer_rd_ack = 0;
      rd_data = 0;
      rd_data_local = 0;

      rd_data_available = 0;
   end

   assign wr_buffer_free = ((~wr_queue_full) & (~wr_reg_full) & (~wr_en));

   always @(posedge clk)
   begin
      if(reset == 1) begin
         $write("<reset>\n");
         rd_data <= 0;
         rd_data_local <= 0;
         rd_data_available <= 0;
         state_rd <= INIT;
      end else begin

         spi_clk_reg <= {spi_clk_reg[0], SPI_SCK};
         mosi_reg <= {mosi_reg[0], SPI_MOSI};
         spi_ss_reg <= {spi_ss_reg[0], SPI_SS};
         $write("MOSI:%x/%d", mosi_reg, SPI_MOSI);

         if (spi_ss_falling_edge == 1 || spi_ss_rising_edge == 1) begin
            counter_read <= 0;
         end

         if(spi_clk_rising_edge == 1'b1) begin //default on spi clk
            miso_out_reg <= 0; //default
         end

         case (state_rd)
         INIT : begin // wait the init opcode from host (0x1) and nothing else
           $write("state_rd=INIT");
            if(spi_clk_rising_edge == 1'b1) begin
               $write("ADD counter-read=%d, bit:%d", counter_read, mosi_reg[0]);

               if(counter_read >= 31) begin //finish recv
                  $write("INIT-ADD full read=%02x %02x %02x %02x\n", rd_data_local[7:0],
                    rd_data_local[15:8], rd_data_local[23:16], rd_data_local[31:24]);
               end
               rd_data_local[31:0] <= {mosi_reg[0], rd_data_local[31:1]};
               counter_read <= counter_read + 1;

               if(counter_read == 5) begin //status, write master to slave successful
                  $write("MISO: status mark INIT"); 
                  miso_out_reg <= 1;
               end

               if(counter_read >= 31) begin //finish recv
                  $write("INIT-ADD first byte=%02x\n", rd_data_local[8:1]);

                  if(rd_data_local[8:1] == 8'h1) begin //received init opcode, otherwise ignore
                    $write("SWITCH INIT TO RD_WAIT_DATA\n");

                     state_rd <= RD_WAIT_DATA;
                  end
                  else begin
                    $write("SWITCH INIT FAILED!!!\n");
                  end      
                  counter_read <= 0;
               end

            end
         end
         RD_WAIT_DATA : begin
           $write("state_rd=RD_WAIT_DATA");
            if(spi_clk_rising_edge == 1'b1) begin
               $write("ADD counter:%d, mosi:%d, hasData:%d (%08x)", counter_read, mosi_reg[0], 
                 wr_reg_full, wr_data_reg);

               if(counter_read == 5 && rd_data_available == 0) begin //status, write master to slave successful
                  $write("MISO: status mark WAIT"); 
                  miso_out_reg <= 1;
               end

               if (wr_reg_full == 1) begin //something ready to be written

                  //bits 0-7 reserved for status, starting to write wr_data_reg
                  //one clock before to be sent the next on miso
                  if(counter_read == 6) begin //status, read master to slave successful
                     $write("MISO: status with data"); 
                     miso_out_reg <= 1;
                  end else 
`ifdef _VERILATOR
                  if(counter_read == 7) begin //status, read master to slave successful
                     $write("MISO: STATUS_FPGA_SEND_MASK"); 
                     miso_out_reg <= 1;
                  end else
                  if(counter_read >= 8 /*&& counter_read <= 31*/) begin
`else
                  if(counter_read >= 7 /*&& counter_read <= 31*/) begin
`endif
                     miso_out_reg <= wr_data_reg[0];
                     wr_data_reg[23:0] <= {wr_data_reg[0], wr_data_reg[23:1]};
                  end
               end

               rd_data_local[31:0] <= {mosi_reg[0], rd_data_local[31:1]};
               counter_read <= counter_read + 1;

               if(counter_read >= 31) begin //finish recv

                  if (wr_reg_full == 1) begin //something was written, now free
                     $write("QUEUE RELEASE"); 
                     wr_reg_full <= 0;
                     wr_data_reg <= 24'h00; //clear write buffer
                  end

                  if(rd_data_available == 0) begin
                     rd_data_available <= 1;
                     rd_data <= {mosi_reg[0], rd_data_local[31:1]};
                     //$write("rd_data=%x/%x", rd_data_local[8:1], rd_data);
                     $write("rd_data=%x", {mosi_reg[0], rd_data_local[31:1]});

                  end
                  state_rd <= RD_WAIT_DATA;
                  counter_read <= 0;
               end
            end
         end
         default : begin
         end
         endcase

         if(rd_ack == 1 && rd_data_available == 1 && buffer_rd_ack == 0) begin
            buffer_rd_ack <= 1;
         end

         if(buffer_rd_ack == 1 && counter_read == 0) begin
            rd_data_available <= 0;
            buffer_rd_ack <= 0;
         end

         //write to the queue
         if (wr_en == 1 && (~wr_reg_full) && (~wr_queue_full) ) begin
           $write("QUEUE-A");
            wr_queue_full <= 1;
            wr_data_queue <= wr_data;
         end

         //move from queue to reg only when no com (counter_read == 0)
         if(wr_queue_full == 1 && counter_read == 0) begin
            $write("QUEUE-B");
            wr_data_reg <= wr_data_queue;
            wr_queue_full <= 0;
            wr_reg_full <= 1;
         end
      end
   end
endmodule
