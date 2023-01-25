`timescale 1ns / 1ps

module mux8_3(
    input wire [7:0] in,
    input wire [2:0] sel,
    
    output wire out
    );
   
assign out = in[sel];
        
endmodule