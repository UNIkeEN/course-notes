module seg_static_pro
#(
    parameter   CNT_MAX = 25'd24_999_999
)
(
    input   wire            sys_clk     ,
    input   wire            sys_rst_n   ,
    
    output   reg    [3:0]   sel         ,
    output   reg    [7:0]   seg
);

reg     [24:0]  cnt;
reg             cnt_flag;
reg     [3:0]   data;

//cnt
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        cnt <= 25'd0;
    else if(cnt == CNT_MAX)
        cnt <= 25'd0;
    else
        cnt <= cnt + 25'd1;

//cnt_flag
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        cnt_flag <= 1'b0;
    else if(cnt == CNT_MAX - 25'd1)
        cnt_flag <= 1'b1;
    else
        cnt_flag <= 1'b0;

//data
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        data <= 4'd0;
    else if((cnt_flag == 1'b1) && (data == 4'b1111))
        data <= 4'd0;
    else if(cnt_flag == 1'b1)
        data <= data + 4'd1;
    else
        data <= data;

//sel
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        sel <= 4'b0000;
    else
        sel <= 4'b1111;

//seg
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        seg <= 8'b0000_0000;
    else 
        case(data)
            4'd0:       seg <= 8'b1011_1111;
            4'd1:       seg <= 8'b0000_0110;
            4'd2:       seg <= 8'b1101_1011;
            4'd3:       seg <= 8'b0100_1111;
            4'd4:       seg <= 8'b1110_0110;
            4'd5:       seg <= 8'b0110_1101;
            4'd6:       seg <= 8'b1111_1101;
            4'd7:       seg <= 8'b0000_0111;
            4'd8:       seg <= 8'b1111_1111;
            4'd9:       seg <= 8'b0110_1111;
            4'd10:      seg <= 8'b1111_0111;
            4'd11:      seg <= 8'b0111_1100;
            4'd12:      seg <= 8'b1011_1001;
            4'd13:      seg <= 8'b0101_1110;
            4'd14:      seg <= 8'b1111_1001;
            4'd15:      seg <= 8'b0111_0001;
            default:    seg <= 8'b1000_0000;
        endcase

endmodule

