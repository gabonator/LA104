module app ( 
    output wire led
); 
 
reg data;
assign led = data;  
always @(*) begin 
  data <= 1'b1; 
end 
 
endmodule 
