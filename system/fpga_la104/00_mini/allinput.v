module app (
    input wire clk,
    input wire SSEL,
    input wire MOSI,
    input wire SCK,
    input wire MISO
);

reg test;
always @(posedge clk) begin
    test <= SSEL & MOSI & SCK;
end

endmodule
