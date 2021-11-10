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
reg [7:0] byte_data_received;
reg [7:0] command;

reg [7:0] ram[63:0];  // can't fit more!
reg [5:0] ram_offset;

always @(posedge clk)
begin
  if(~SSEL_active)
  begin
    bitcnt <= 3'b000;
    command <= 8'hff;
    data_send <= 8'h00;
  end
  else
  if(SCK_risingedge)
  begin
    bitcnt <= bitcnt + 3'b001;

    // implement a shift-left register (since we receive the data MSB first)
    byte_data_received <= {byte_data_received[6:0], MOSI_data};

    if (bitcnt == 3'b111)
    begin
      // assigning final bit
      case (command)
        8'hff: 
          begin
            command <= {byte_data_received[6:0], MOSI_data};
            ram_offset <= 0;

            if ({byte_data_received[6:0], MOSI_data} == 8'h02)
              data_send <= ram[0];
            else
              data_send <= 8'h77;
          end
        8'h01:  // write
          begin 
            ram[ram_offset] <= {byte_data_received[6:0], MOSI_data};
            ram_offset <= ram_offset + 1;
            data_send <= 8'h00; // write
          end
        8'h02: // read
          begin
            data_send <= ram[ram_offset+1];
            ram_offset <= ram_offset + 1; 
          end
        default: 
          data_send <= 8'hff;
      endcase
    end
  end
end

// transmit

reg [7:0] byte_data_sent;
reg [7:0] data_send;

always @(posedge clk)
if(SSEL_active)
begin
  if(SSEL_startmessage)
  begin
    $write("(start msg)\n");
    byte_data_sent <= 8'h00; // invalid
  end
  else
  if(SCK_fallingedge)
  begin
    if(bitcnt==3'b000)
    begin
      $write("(set new byte: 0x%02x)\n", data_send);
      byte_data_sent <= data_send;
    end
    else
    begin
      byte_data_sent <= {byte_data_sent[6:0], 1'b0};
    end
  end
end

assign MISO = byte_data_sent[7];  // send MSB first
endmodule
