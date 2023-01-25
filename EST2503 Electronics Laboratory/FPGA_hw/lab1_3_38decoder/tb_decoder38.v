`timescale 1ns / 1ps

module tb_decoder38();
reg [2:0]in;
wire [7:0]out;

initial
    begin
    in <= 3'b000;
    end
    
always #10 in <= {$random} % 8;

decoder38 decoder38_inst
(
    .in(in),
    .out(out)
);
endmodule
