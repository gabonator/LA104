// https://www.fpga4fun.com/SPI2.html

module app(clk, SCK, MOSI, MISO, SSEL);

input clk;
input SCK, SSEL, MOSI;
output MISO;
//output LED;

// sync SCK to the FPGA clock using a 3-bits shift register
reg [2:0] SCKr;  always @(posedge clk) SCKr <= {SCKr[1:0], SCK};
wire SCK_risingedge = (SCKr[2:1]==2'b01);  // now we can detect SCK rising edges
wire SCK_fallingedge = (SCKr[2:1]==2'b10);  // and falling edges

// same thing for SSEL
reg [2:0] SSELr;  always @(posedge clk) SSELr <= {SSELr[1:0], SSEL};
wire SSEL_active = ~SSELr[1];  // SSEL is active low
wire SSEL_startmessage = (SSELr[2:1]==2'b10);  // message starts at falling edge
wire SSEL_endmessage = (SSELr[2:1]==2'b01);  // message stops at rising edge

// and for MOSI
reg [1:0] MOSIr;  always @(posedge clk) MOSIr <= {MOSIr[0], MOSI};
wire MOSI_data = MOSIr[1];

// receive

// we handle SPI in 8-bits format, so we need a 3 bits counter to count the bits as they come in
reg [2:0] bitcnt;

reg byte_received;  // high when a byte has been received
reg [7:0] byte_data_received;

always @(posedge clk)
begin
  if(~SSEL_active)
  begin
    bitcnt <= 3'b000;
  end
  else
  if(SCK_risingedge)
  begin
    bitcnt <= bitcnt + 3'b001;

    // implement a shift-left register (since we receive the data MSB first)
    byte_data_received <= {byte_data_received[6:0], MOSI_data};
  end
  byte_received <= bitcnt == 3'b111 && SCK_risingedge && SSEL_active;
end

always @(posedge clk) 
  if (byte_received)
  begin
    $write("<reveived: 0x%02x>", byte_data_received);
  end

// transmit

reg [7:0] byte_data_sent;

always @(posedge clk)
if(SSEL_active)
begin
  if(SSEL_startmessage)
  begin
    byte_data_sent <= fibnum;  // first byte sent in a message is the message count
    $write("(start msg)\n");
  end
  else
  if(SCK_fallingedge)
  begin
    if(bitcnt==3'b100)
      fibclk <= 1'b1;
    if(bitcnt==3'b101)
      fibclk <= 1'b0;

    if(bitcnt==3'b000)
    begin
      $write("(set new byte: 0x%02x)\n", fibnum);
      byte_data_sent <= fibnum;
    end
    else
    begin
      byte_data_sent <= {byte_data_sent[6:0], 1'b0};
    end
  end
end

assign MISO = byte_data_sent[7];  // send MSB first
// we assume that there is only one slave on the SPI bus
// so we don't bother with a tri-state buffer for MISO
// otherwise we would need to tri-state MISO when SSEL is inactive

// sequence generator
reg fibclk;
wire fibrst = SSEL_startmessage && SSEL_active;
reg [7:0] fibnum = 8'h00;
memory fib(.clk(fibclk), .rst(fibrst), .fibout(fibnum));

endmodule
