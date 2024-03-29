module memory(clk, rst, fibout);
input clk;
input rst;
output [7:0] fibout;

reg[8:0] address;
reg[7:0] dataIn;
wire[7:0] dataOut;
reg wrEnable;

reg[7:0] regFibOut;
assign fibout = regFibOut;

alta_ram4k gabosram(
  .Clk0(clk),
  .Clk1(~1'b0),
  .ClkEn0(1'b1),
  .ClkEn1(~1'b0),
  .AsyncReset0(1'b0),
  .AsyncReset1(~1'b0),
  .AddressA(address),
  .DataInA(dataIn),
  .WeRenA(1'b0),
  .ByteEnA(),
  .AddressB(),
  .DataInB(),
  .WeRenB(~1'b0),
  .ByteEnB(),
  .DataOutA(dataOut),
  .DataOutB()
);
defparam gabosram.CLKMODE = "read_write";
defparam gabosram.DATA_WIDTH_A = 8;
defparam gabosram.ADDR_WIDTH_A = 9;
defparam gabosram.BYTE_WIDTH_A = 0;
defparam gabosram.PORTA_WRITEMODE = "normal";
defparam gabosram.PORTA_OUTREG = "no";
defparam gabosram.INIT_PORT = "a";
defparam gabosram.INIT_VAL = 4096'h349ACD277100B8E4653B9BCD866100A4E67334994EE6C32990CA281018CCA76349B8E665361404065341D040683A1A4EE20339B8DE6C30880587232998C27710194D07410198DE2039194E070309C8EE20329A0E820371BC4064329D0C66938194C82039194EE6F36198402937184D27437194CE281018CCA72379A04065341D040723799840653B1A4E875371A4DA6932080C220399A44079359B0C272379A04064391BCEE20329A0A820170A4E67437194DA6F26880DE7410190CA6737184D063101CCC277140807030180C840653A184D820371A4407939184CE6E3A920406E34880C86E308805829349ACD861391BC8E20161B8DE69399C8CA76101A0E669361BCA020329C8DE6D10184406F3A080C865339B8C26831880E6613B880CA6D309B850201B8C06032101B8D220321B8C26C37940406E34880C865319D4C86F391D0DC69101CCC277101E4D66C309C8DE48100B86A361C8C44065319B8D27310184D26B309D8DE6C29880C86E30880C66936188EA7032948406831994F44310194D074101B8D220329B4D274101B0D86110198DE2039194CC613B880CE6E349B0D865399D0E66531080CA683A080E669101D0922017190DC613918840613A1A4C86529880CA683A080E465321B8EA20171CC5C61100B0CE6E34990D86F240805C43171105C49101E4C42032990C26D1019CDC693A184DE6310184DE633798C4064371844067371A4D86C3499840743A9B8C26538080D074349DC4074349D4C67334988407232998C277101ACC276379B0A62030880E669101E4D66C309C8DE48;


always @(posedge clk or posedge rst)
begin
  if (rst == 1'b1)
  begin
    address <= 10'b0000000000;
  end
  else if (clk == 1'b1)
  begin
    regFibOut <= dataOut;
    address <= address + 1;
  end
end

endmodule
