module app (
    input wire clk,
    input wire SSEL,
    input wire MOSI,
    input wire SCK,
    output wire MISO
);

reg sampled;
assign MISO = sampled;
always @(posedge clk)
begin
  sampled <= SCK;
end

endmodule
