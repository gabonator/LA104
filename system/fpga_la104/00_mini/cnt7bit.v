module app (
    input wire clk,
    input wire SSEL,
    input wire MOSI,
    input wire SCK,
    inout wire MISO
);

reg[7:0] value;
reg[7:0] sending;
reg[2:0] index;
reg enabled;
reg[1:0] spi_ss_reg;
wire inhibit = (index == 3'b000);

wire Tx_En;
wire Rx_Data;
assign Tx_En = enabled && ~inhibit;
assign MISO = Tx_En ? Tx_Data : 1'bZ;
assign Rx_Data = MISO;

reg [1:0] spi_clk_reg = 0;

wire spi_ss_falling_edge;
wire spi_clk_falling_edge;
assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);
assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);
wire Tx_Data = sending[0];
/*
  0: z000 0000
     z000 0011
     z000 0010
     z000 0101
     z000 0100
     z000 1111

  1: z000 0001
     z000 0001
     z000 0000
     z000 0010
     z000 0011
     z000 0011
 
  4: z001 0000
     z010 0000
     z011 0000
     z100 0000
     z101 0000

  6: z000 0100
     z000 1000
     z000 1100
     z001 0000
     z001 0100

*/ 
//wire Tx_Data = index[0]; 

always @(posedge clk)
begin
  spi_ss_reg <= {spi_ss_reg[0], SSEL};
  spi_clk_reg <= {spi_clk_reg[0], SCK};

  if (SSEL)
  begin
    enabled <= 0;
  end
  if (spi_ss_falling_edge)
  begin
    value <= 8'b00010000;
    enabled <= 1;
    sending <= 8'b11001010;
    index <= 3'b000;
//    inhibit <= 0;
  end

  if (spi_clk_falling_edge && enabled)
  begin
    index <= index + 3'b001;
//    if (inhibit)
//    begin
//      inhibit <= 0;
//    end else
//    begin
      sending[7:0] <= {sending[6:0], sending[7]};
      if (index == 3'b110)
      begin
        value <= value + 8'b00000001;
        sending <= value + 8'b00000001;
//        inhibit <= 1;
      end
//    end
  end
end

endmodule
