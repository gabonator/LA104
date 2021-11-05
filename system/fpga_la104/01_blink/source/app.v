module app (
    input wire clk,
    output wire MISO
);

reg [32:0] counter;
assign MISO = counter[24];

always @(posedge clk) begin
    counter <= counter + 1;
end

endmodule
