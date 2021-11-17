module fifo(
  input wire clk, 
  input wire read, 
  output reg dataReadReady,
  input wire write, 
  input reg[7:0] dataWrite,
  output reg[7:0] dataRead);

reg [7:0] memory[15:0];
reg [3:0] ptrWrite = 0;
reg [3:0] ptrRead = 0;
reg [3:0] ptrCount = 0;

/*assign*/wire fifo_empty   = ( ptrWrite == ptrRead ) ? 1'b1 : 1'b0;
/*assign*/wire fifo_full    = ( ptrWrite == 14 ) ? 1'b1 : 1'b0;
/*assign*/wire fifo_not_empty = ~fifo_empty;
/*assign*/wire fifo_not_full = ~fifo_full;

always @ (posedge clk) 
begin
  if (dataReadReady)
    dataReadReady <= 0;

  if ( write )
  begin
    $write("<fifo: push to %d = %02x>", ptrWrite, dataWrite);
    memory[ptrWrite] <= dataWrite;
  end

  if ( read )
  begin
    dataRead <= memory[ptrRead];
    dataReadReady <= 1;
    $write("<fifo: pull at %d = %02x>", ptrRead, memory[ptrRead]);
  end

  if ( write )
      ptrWrite <= ptrWrite + 1;

  if ( read && fifo_not_empty )
      ptrRead <= ptrRead + 1;
end
endmodule

