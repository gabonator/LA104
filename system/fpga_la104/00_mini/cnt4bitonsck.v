module app (
    input wire clk,
    input wire SSEL,
    input wire MOSI,
    input wire SCK,
    inout wire MISO
);

reg[3:0] value;
reg[3:0] sending;
reg[1:0] index;
reg enabled;
reg[1:0] spi_ss_reg;
//wire spi_clk_falling_edge;
reg inhibit;
reg transmit;
wire Tx_En;
wire Rx_Data;
wire spi_ss_falling_edge;
wire spi_clk_falling_edge;

assign Tx_En = enabled && ~inhibit;
wire Tx_Data = transmit; //sending[0]; // MSB first
//wire Tx_Data = sending[0]; // MSB first
assign MISO = Tx_En ? Tx_Data : 1'bZ;
assign Rx_Data = MISO;

reg [1:0] spi_clk_reg = 0;

assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);
assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);

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
    value <= 4'b0000;
    enabled <= 1;
    sending <= 4'b1111;
    index <= 2'b00;
    inhibit <= 0;
//    transmit <= 1;
  end

  if (enabled)
  begin
    transmit <= sending[0];
  end
  // default high impedance
  if (spi_clk_falling_edge && enabled)
  begin
    if (inhibit)
    begin
      // HiZ marker between values
      inhibit <= 0;
//      transmit <= sending[0];
    end else
    begin
//      transmit <= sending[0];
      sending[3:0] <= {sending[0], sending[3:1]};
      index <= index + 2'b01;
      if (index == 2'b11)
      begin
        value <= value + 4'b0001;
        sending <= value + 4'b0001;
        inhibit <= 1;
      end
    end
  end
end

endmodule
