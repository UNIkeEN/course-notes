`timescale 1ns / 1ps

module muxtwo(
    input wire in0,
    input wire in1,
    input wire sel,
    
    output reg out
    );
    
always@(*)
    out = ((sel == 1'b0) & in0 )|((sel == 1'b1) & in1 );
endmodule
