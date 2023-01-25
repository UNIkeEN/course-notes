`timescale 1ns / 1ps

module tb_seg_dynamic();

reg             sys_clk     ;
reg             sys_rst_n   ;

wire    [7:0]   seg         ;
wire    [7:0]   sel         ;    

initial
    begin
        sys_clk = 1'b1;
        sys_rst_n <= 1'b0;
        #30
        sys_rst_n <= 1'b1;
    end

always #10 sys_clk = ~sys_clk;

defparam    seg_dynamic_inst.data_show_inst.CNT_MAX = 17'd19;
defparam    seg_dynamic_inst.data_gen_inst.CNT_MAX = 23'd49;

seg_dynamic seg_dynamic_inst
(
    .sys_clk     (sys_clk   ),
    .sys_rst_n   (sys_rst_n ),
   
    .seg         (seg       ),
    .sel         (sel       )
);

endmodule
