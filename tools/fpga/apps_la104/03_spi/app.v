module app ( 
    input wire clk, 
    input wire rst_n, 
    output wire led, 
    output [17:0] gpio 
); 
 
 
wire clk_pll_o; 
wire wireout;
 
reg [7:0] datain;
reg [7:0] dataout;

assign wireout = dataout[7:0];

alta_spi spi_inst (
  .DataOut(wireout)
);
 /*
	reg [7:0] datain;
	reg [7:0] dataout;


 alta_spi spi_inst (
	  .Clk(),
	  .Rst(),
  .WrRdn(),
  .Strobe(),
  .DataIn(datain),
  .Address(),
  .Mi(),
  .Si(), 
  .Scki(), 
  .Csi(),
  .Wakeup(),
  .Irq(),
  .Ack(),
  .So(),
  .Soe(), 
  .Mo(), 
  .Moe(), 
  .Scko(), 
  .Sckoe(),
  .Cso(),
  .Csoe(),
  .DataOut(dataout)
  ); 
 */
    inpll pll_inst ( 
        .clkin(clk),        // PLL.clkin MUST connect to PIN_XX_GB 
        .clkfb(clk_pll_o), 
        .pllen(1'b1), 
        .resetn(rst_n), 
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
 
    reg [24:0] counter; 
 
    assign led = counter[24]; 
    assign gpio[17:0] = counter[23:6]; 
 
    always @(posedge clk_pll_o or negedge rst_n) begin 
        if (!rst_n)
		  begin
				counter <= 25'b0; 
				dataout <= 8'b1;
		  end
        else 
            counter <= counter + 1; 
    end 
 
endmodule 
