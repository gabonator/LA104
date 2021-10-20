module app ( 
    output wire led,
    input wire in1
); 
 
assign led = ~in1; 

endmodule 
