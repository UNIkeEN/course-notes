module data_gen
#(
    parameter   CNT_MAX  = 23'd4_999_999,
    parameter   DATA_MAX = 16'hFFFF
)
(
    input   wire            sys_clk      ,
    input   wire            sys_rst_n    ,
    
    output   reg    [15:0]  data         
);

reg     [22:0]  cnt_50ms;
reg             cnt_flag;

//cnt_50ms
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        cnt_50ms <= 23'd0;
    else if(cnt_50ms == CNT_MAX)
        cnt_50ms <= 23'd0;
    else
        cnt_50ms <= cnt_50ms + 23'd1;

//cnt_flag
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        cnt_flag <= 1'b0;
    else if(cnt_50ms == CNT_MAX - 23'd1)
        cnt_flag <= 1'b1;
    else
        cnt_flag <= 1'b0;

//data
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        data <= 16'h0;
    else if((data == DATA_MAX)&&(cnt_flag == 1'b1))
        data <= 16'h0;
    else if(cnt_flag == 1'b1)
        data <= data + 16'd1;
    else
        data <= data;

endmodule