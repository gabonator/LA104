module app (
    input wire clk,
    input wire rst_n,
    output wire led1,
    output wire led2
);

    wire clk_pll_o;

    inpll pll_inst (
        .clkin(clk),        // PLL.clkin MUST connect to PIN_XX_GB
        .clkfb(clk_pll_o),
        .pllen(1'b1),
        .resetn(rst_n),
//        .resetn(),
        .clkout0en(1'b1),
        .clkout1en(1'b0),
        .clkout2en(1'b0),
        .clkout3en(1'b0),
        .clkout0(clk_pll_o),
        .clkout1(),
        .clkout2(),
        .clkout3(),
        .lock()
    );

    reg [24:0] counter1;
    assign led1 = counter1[24];

    always @(posedge clk_pll_o) begin
        counter1 <= counter1 + 1;
    end


    reg [24:0] counter2;
    assign led2 = counter2[8];

    always @(posedge clk) begin
        counter2 <= counter2 + 1;
    end

endmodule
