module app (
    input wire clk,
    input wire SSEL,
    input wire MOSI,
    input wire SCK,
    output wire MISO
);

assign MISO = SCK & MOSI;

endmodule
