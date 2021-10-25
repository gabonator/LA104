module app (
    input wire clk,
    output wire miso
);

reg [32:0] counter;
assign miso = counter[25];

always @(posedge clk) begin
    counter <= counter + 1;
end

endmodule
