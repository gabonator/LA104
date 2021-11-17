//`define VERBOSE

module spi(clk, SCK, MOSI, MISO, SSEL, dataTx, dataRx, dataRxReady, 
  dataTxReady, dataTxRequest, reset);

input clk, SCK, SSEL, MOSI;
output MISO;
input [7:0] dataTx;
output [7:0] dataRx;
output dataRxReady;
output reset;
input dataTxReady;
output dataTxRequest;

reg [7:0] byte_data_receiving = 8'h00;
reg [7:0] dataOutReg = 8'h00;
assign dataRx = dataOutReg;
reg data_received_valid = 1'b0;
assign dataRxReady = data_received_valid;
assign reset = ~SSEL_active;
reg dataTxRequestReg = 1'b0;
assign dataTxRequest = dataTxRequestReg;

/*
   ______                                                                              ____
SS       \____________________________________________________________________________/

          __0    ___1    ___2    ___3    ___4    ___5    ___6    ___7    ___10   ___
SCK-CPOL1   \___/   \___/   \___/   \___/   \___/   \___/   \___/   \___/   \___/   \


SPI Slave: (L-load, S-sample, P-preload, F-flush)
MISO     P  L0      L1     L2       L3      L4      L5      L6      L7   +P L10     L11
MOSI            S0      S1      S2      S3      S4      S5      S6      S7+F    S10
                                                                    
SPI Master:
MOSI        L        L       L       L       L       L       L       L       L       L
MISO            S        S       S       S       S       S       S       S       S

*/
// SCK
reg [2:0] SCKr = 3'b000;  
always @(posedge clk) 
  SCKr <= {SCKr[1:0], SCK};

wire SCK_risingedge = (SCKr[2:1]==2'b01);
wire SCK_fallingedge = (SCKr[2:1]==2'b10);

// SSEL
reg [2:0] SSELr = 3'b111;  
always @(posedge clk) 
  SSELr <= {SSELr[1:0], SSEL};

wire SSEL_active = ~SSELr[1];
wire SSEL_startmessage = (SSELr[2:1]==2'b10);
wire SSEL_endmessage = (SSELr[2:1]==2'b01);

// MOSI
reg [1:0] MOSIr = 2'b00;  
always @(posedge clk) 
  MOSIr <= {MOSIr[0], MOSI};
wire MOSI_data = MOSIr[1];

// comm
reg [2:0] bitcnt = 3'b000;
reg [7:0] byte_data_sent = 8'h00;
reg [1:0] first;
reg dataTxRequest = 1'b0;
reg resetRequested = 0;
reg loadedAssert = 0;
assign MISO = byte_data_sent[7];

always @(posedge clk)
begin
  if (data_received_valid)
    data_received_valid <= 0;

  if (dataTxRequestReg)
    dataTxRequestReg <= 0;

  if (dataTxReady)
  begin
`ifdef VERBOSE
    $write(" Preload (0x%02x)", dataTx);
`endif
    byte_data_sent <= dataTx;
    loadedAssert <= 1;
  end

  if(~SSEL_active)
  begin
`ifdef VERBOSE
    $write(" Reset");
`endif
    bitcnt <= 3'b000;
    data_received_valid <= 1'b0;
    if (resetRequested == 0)
    begin
`ifdef VERBOSE
      $write(" Request init");
`endif
      $write(" Request init");
      dataTxRequestReg <= 1;
      resetRequested <= 1;
    end
  end
  // sample MOSI on rising edge, host has set the level on previous falling edge
  else if(SCK_risingedge)
  begin
    resetRequested <= 0;
`ifdef VERBOSE
    $write(" RX: %d/8, add bit %d, new reg 0x%02x", bitcnt, MOSI_data, {byte_data_receiving[6:0], MOSI_data});
`endif
    bitcnt <= bitcnt + 3'b001;
    if (bitcnt == 3'b111)
    begin
`ifdef VERBOSE
      $write(" RX: received 0x%02x", {byte_data_receiving[6:0], MOSI_data});
`endif
      dataOutReg <= {byte_data_receiving[6:0], MOSI_data};
      data_received_valid <= 1;
      loadedAssert <= 0;
      dataTxRequestReg <= 1;
    end else begin
      byte_data_receiving <= {byte_data_receiving[6:0], MOSI_data};
    end
  end
  // shift MISO on falling edge
  else if(SCK_fallingedge)
  begin
`ifdef VERBOSE
    $write(" TX: %d", byte_data_sent[7]);
`endif
    if (bitcnt != 0)
    begin
      byte_data_sent <= {byte_data_sent[6:0], 1'b0};
      if (loadedAssert == 0)
        $write(" TX data not ready!");
    end
  end
end

endmodule
