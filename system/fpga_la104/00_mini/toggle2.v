module app (
    input wire clk,
    input wire SSEL,
    input wire MOSI,
    input wire SCK,
    output wire MISO
);

reg [1:0] spi_clk_reg = 0;
wire spi_clk_falling_edge;
assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);

reg sampled = 0;
assign MISO = sampled;
always @(posedge clk)
begin
  spi_clk_reg <= {spi_clk_reg[0], SCK};
  if (spi_clk_falling_edge)
  begin
    sampled <= ~sampled;
  end
end

endmodule

