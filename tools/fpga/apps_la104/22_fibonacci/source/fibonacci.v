// https://thesharanmohanblog.wordpress.com/2018/09/12/intro-to-fpgas-fibonacci-sequence/

module fibonacci(clk, rst, fibout);
input clk;
input rst;
output [7:0] fibout;
reg [7:0] prev_val;
reg [7:0] curr_val;

always @(posedge clk or posedge rst)
begin
  if (rst == 1'b1)
  begin
    $write("(fibonacci: reset)");
    prev_val <= 8'h00;
    curr_val <= 8'h01;
  end
  else if (clk == 1'b1)
  begin
    $write("(fibonacci: 0x%02x)", fibout);
    prev_val <= curr_val;
    curr_val <= curr_val + prev_val;
  end
end

assign fibout = prev_val;
endmodule
