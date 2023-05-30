`timescale 1ns / 1ps

module tb_mux8();

reg in0,in1,in2,in3,in4,in5,in6,in7;
reg [2:0]sel;
wire out;

initial
    begin
    in0 = 0;
    in1 = 0;
    in2 = 0;
    in3 = 0;
    in4 = 0;
    in5 = 0;
    in6 = 0;
    in7 = 0;
    sel <= 3'b000;
    end

always #10 in0 <= {$random} % 2;
always #10 in1 <= {$random} % 2;
always #10 in2 <= {$random} % 2;
always #10 in3 <= {$random} % 2;
always #10 in4 <= {$random} % 2;
always #10 in5 <= {$random} % 2;
always #10 in6 <= {$random} % 2;
always #10 in7 <= {$random} % 2;
always #10 sel <= {$random} % 8;
   
//mux8_1 mux_8_1_inst(
//    .in0(in0), 
//    .in1(in1), 
//    .in2(in2),
//    .in3(in3),
//    .in4(in4),
//    .in5(in5),
//    .in6(in6),
//    .in7(in7),
//    .sel(sel),
//    .out(out)
//);
mux8_2 mux_8_2_inst(
    .in0(in0), 
    .in1(in1), 
    .in2(in2),
    .in3(in3),
    .in4(in4),
    .in5(in5),
    .in6(in6),
    .in7(in7),
    .sel(sel),
    .out(out)
);
//mux8_3 mux_8_3_inst
//(
//    .in({in0, in1, in2, in3, in4, in5, in6, in7}),
//    .sel(se),
//    .out(out)
//);
endmodule
