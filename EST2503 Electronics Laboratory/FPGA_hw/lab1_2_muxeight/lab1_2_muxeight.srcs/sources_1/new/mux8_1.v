`timescale 1ns / 1ps

module mux8_1(
    input wire in0,in1,in2,in3,in4,in5,in6,in7,
    input wire [2:0] sel,
    
    output reg out 
    );
   
always@(*)
    if(sel == 3'b000)
        out = in0;
    else if(sel == 3'b001)
        out = in1;
    else if(sel == 3'b010)
        out = in2;
    else if(sel == 3'b011)
        out = in3;
    else if(sel == 3'b100)
        out = in4;
    else if(sel == 3'b101)
        out = in5;
    else if(sel == 3'b110)
        out = in6;
    else
        out = in7;
        
endmodule
