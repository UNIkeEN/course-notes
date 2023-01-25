`timescale 1ns/1ns

module tb_seg_static_pro();

reg     sys_clk;
reg     sys_rst_n;

wire    [3:0]   sel;
wire    [7:0]   seg;

initial
    begin
        sys_clk = 1'b1;
        sys_rst_n <= 1'b0;
        #20
        sys_rst_n <= 1'b1;
    end

always #10 sys_clk = ~sys_clk;

seg_static_pro
#(
    .CNT_MAX(25'd24)
)
seg_static_pro_inst
(
    .sys_clk     (sys_clk    ),
    .sys_rst_n   (sys_rst_n  ),
   
    .sel         (sel        ),
    .seg         (seg        )
);

endmodule