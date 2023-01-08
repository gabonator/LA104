/*
  MISO is at high impedance when CS=1, otherwise square signal with period clk/8
*/
module app (
    input wire clk,
    input wire SSEL,
    inout wire MISO
);

reg [32:0] counter;
reg test;

assign Tx_En = ~SSEL;
wire Tx_Data = counter[3];
assign MISO = Tx_En ? Tx_Data : 1'bZ;
assign Rx_Data = MISO;

always @(posedge clk) begin
    counter <= counter + 1;
    test <= Rx_Data;
end

endmodule
