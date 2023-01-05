module app(input clk, input SCK, input SSEL, input MOSI, output MISO);
  reg wr_en = 0;
  reg [7:0] wr_data = 0;
  reg [2:0] counter_read = 0;
  reg [1:0] spi_clk_reg = 0;
  reg [1:0] spi_ss_reg = 0;
  reg [1:0] mosi_reg = 0;
  reg miso_out_reg = 0;
  reg wr_reg_full = 0;
  reg [7:0] wr_data_reg = 0;
  reg wr_queue_full = 0;
  reg [7:0] wr_data_queue = 0;
  assign MISO = miso_out_reg;
  wire spi_ss_falling_edge;
  wire spi_clk_falling_edge;
  wire spi_clk_rising_edge;
  wire spi_ss_low;
  assign spi_clk_rising_edge = (spi_clk_reg[1:0] == 2'b01);
  assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);
  assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);
  assign spi_ss_low = (spi_ss_reg[0] == 0);

  always @(posedge clk)
  begin
    spi_ss_reg <= {spi_ss_reg[0], SSEL};
    spi_clk_reg <= {spi_clk_reg[0], SCK};
    mosi_reg <= {mosi_reg[0], MOSI};

    if(spi_ss_falling_edge == 1)
    begin
      $write("<reset>\n");
      counter_read <= 0;
      wr_reg_full <= 0;
      wr_queue_full <= 0;
      wr_data <= 8'h4e;
      wr_en <= 1;
    end else if (spi_ss_low) begin
      if(spi_clk_falling_edge) begin
        $write("step %d, rd:%d, wr:%d wr-avail:%d", 
                counter_read, mosi_reg[0], wr_data_reg[0], wr_reg_full); 

        miso_out_reg <= 0; 
        if (wr_reg_full == 1) begin
          $write("WRITE:%d", wr_data_reg[0]); 
          miso_out_reg <= wr_data_reg[0];
          wr_data_reg[7:0] <= {wr_data_reg[0], wr_data_reg[7:1]};
        end

        $write("READ:%d", mosi_reg[0]); 
        counter_read <= counter_read + 1;

//        if(counter_read == 7) begin
//          wr_reg_full <= 0;
//          wr_data <= 8'h13;
//          wr_data <= wr_data + 8'h13;
//          wr_en <= 1;
//        end

      end

      //write to the queue
      if (wr_en == 1 && (~wr_reg_full) && (~wr_queue_full) ) begin
        $write("QUEUE-A");
        wr_queue_full <= 1;
        wr_data_queue <= wr_data;
        wr_en <= 0;
      end

      if(wr_queue_full == 1 && counter_read == 0) begin
         $write("LOAD QUEUE");
         wr_data_reg <= wr_data_queue;
         wr_queue_full <= 0;
         wr_reg_full <= 1;
      end     
    end
  end
endmodule
