`timescale 1ns / 1ps


module tb_muxtwo();

reg in0;
reg in1;
reg sel;

wire y;

initial
begin
    sel=1'b0;
    in0=0;
    in1=1;
    #100
    in0=1;
    #100
    sel=1'b1;
    #100
    in1=0;
end

muxtwo tb_muxtwo
(
    .in1(in1),
    .in0(in0),
    .sel(sel),
    
    .out(out)
    );

endmodule
