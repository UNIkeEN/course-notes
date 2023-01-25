module seg_dynamic
(
    input   wire            sys_clk     ,
    input   wire            sys_rst_n   ,
    
    output  wire    [7:0]   seg         ,
    output  wire    [7:0]   sel     
);

wire    [15:0]  data;

data_gen
#(
    .CNT_MAX  (23'd4_999_999),
    .DATA_MAX (16'hFFFF)
)
data_gen_inst
(
    .sys_clk    (sys_clk  ),
    .sys_rst_n  (sys_rst_n),
    
    .data       (data     )
);

data_show data_show_inst
(
    .sys_clk     (sys_clk    ),
    .sys_rst_n   (sys_rst_n  ),
    .data        (data       ),
   
    .seg         (seg        ),
    .sel         (sel        )
);
    
endmodule