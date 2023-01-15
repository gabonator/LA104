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
reg inhibit;
reg transmit;

wire Tx_En;
assign Tx_En = enabled && ~inhibit;
wire Tx_Data = transmit;
assign MISO = Tx_En ? Tx_Data : 1'bZ;
wire Rx_Data;
assign Rx_Data = MISO;

reg [1:0] spi_clk_reg = 0;
wire spi_ss_falling_edge;
wire spi_clk_falling_edge;
assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);
assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);

always @(posedge clk)
begin
  spi_ss_reg <= {spi_ss_reg[0], SSEL};
  spi_clk_reg <= {spi_clk_reg[0], SCK};

  if (SSEL)
  begin
    $write("off");
    enabled <= 0;
  end
  if (spi_ss_falling_edge)
  begin
    $write("reset");
    value <= 4'b0110;
    enabled <= 1;
    sending <= 4'b1111;
    index <= 2'b00;
    inhibit <= 0;

    transmit <= 1; // write
  end

  if (spi_clk_falling_edge && enabled)
  begin
    if (inhibit)
    begin
      $write("bit %d, inhibit %d", index, sending[3]);
      inhibit <= 0;
      transmit <= sending[3]; // write
    end else
    begin
      $write("bit %d, transmit %d", index, sending[2]);
      transmit <= sending[2]; // write
      sending[3:0] <= {sending[2:0], sending[3]};
      index <= index + 2'b01;
      if (index == 2'b11)
      begin
        $write("bit %d, last bit", index);
        value <= value + 4'b0001;
        sending <= value + 4'b0001;
        inhibit <= 1;
      end
    end
  end
end

endmodule
