module app(clk, SCK, MOSI, MISO, SSEL);

input clk;
input SCK, SSEL, MOSI;
output MISO;

spi spiInstance(
  .clk(clk), .SCK(SCK), .MOSI(MOSI), .MISO(MISO), .SSEL(SSEL), 
  .dataTx(dataTx), 
  .dataRx(dataRx), 
  .dataRxReady(dataRxReady), 
  .dataTxReady(dataTxReady),
  .dataTxRequest(dataTxRequest),
  .reset(dataReset));

reg [7:0] dataTx;
reg [7:0] dataRx;
reg dataRxReady;
reg dataTxReady;
reg dataTxRequest;
reg dataReset;
reg wasReset;
reg [7:0] readIndex;

// fifo
fifo fifoInstance(
  .clk(clk),
  .read(fifoRead),
  .write(fifoWrite),
  .dataRead(fifoReadData),
  .dataReadReady(fifoReadReady),
  .dataWrite(fifoWriteData)
);

reg fifoRead = 0;
reg fifoWrite = 0;
reg fifoReadReady = 0;
reg [7:0] fifoReadData;
reg [7:0] fifoWriteData;
reg [7:0] command = 8'hff;
reg [7:0] identify [0:15];
initial $readmemh("identify.txt", identify); 

always @(posedge clk)
begin
  if (fifoWrite)
    fifoWrite <= 0;
  if (fifoRead)
    fifoRead <= 0;

  if (fifoReadReady)
  begin
    dataTx <= fifoReadData;
    dataTxReady <= 1;
  end

  if (dataReset)
    wasReset <= 1;

  if (dataTxReady)
    dataTxReady <= 0;

  if (dataTxRequest)
  begin
    if (wasReset)
    begin
      command <= 8'hff;
      dataTx <= 8'h33;
      dataTxReady <= 1;
      wasReset <= 0;
      $write("<rqrst>");
      readIndex <= 0;
    end else
    begin
      $write("<rq? cmd = %02x>", command);
      case ((dataRxReady && command == 8'hff) ? dataRx : command)  
        8'hff: 
          begin
            command <= dataRx;
            dataTx <= 8'h77;
            dataTxReady <= 1;
          end
        8'h00:
          begin
            readIndex <= readIndex + 1;
            dataTx <= identify[readIndex[3:0]];
            dataTxReady <= 1;
          end
        8'h01: 
          begin
            $write("<wrbb>");
            dataTx <= 8'hbb;
            dataTxReady <= 1;
          end
        8'h02: fifoRead <= 1;
        default:
          begin
            $write("<rqee>");
            dataTx <= 8'hee;
            dataTxReady <= 1;
          end
      endcase
    end
  end

  if (dataRxReady)
  begin
    $write("<got %02x -> %02x>\n", dataRx, command);
    case (command)  
      8'hff: command <= dataRx;
      8'h00: ;
      8'h01: 
        begin
          fifoWriteData <= dataRx;
          fifoWrite <= 1;
        end
      8'h02: ;
      default: ;
    endcase
  end
end
endmodule
