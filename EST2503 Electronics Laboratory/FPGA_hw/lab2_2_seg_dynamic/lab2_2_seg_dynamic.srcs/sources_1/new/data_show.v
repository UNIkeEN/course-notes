module data_show
(
    input   wire            sys_clk     ,
    input   wire            sys_rst_n   ,
    input   wire    [15:0]  data        ,
    
    output   reg    [7:0]   seg         ,
    output   reg    [7:0]   sel
);

parameter   CNT_MAX = 17'd99_999;

reg     [16:0]  cnt_1ms;
reg             flag_1ms;
reg     [1:0]   cnt_sel;
reg     [7:0]   sel_reg;
reg     [3:0]   data_disp;

//cnt_1ms
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        cnt_1ms <= 17'd0;
    else if(cnt_1ms == CNT_MAX)
        cnt_1ms <= 17'd0;
    else
        cnt_1ms <= cnt_1ms + 17'd1;

//flag_1ms
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        flag_1ms <= 1'b0;
    else if(cnt_1ms == CNT_MAX - 17'd1)
        flag_1ms <= 1'b1;
    else
        flag_1ms <= 1'b0;

//cnt_sel
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        cnt_sel <= 2'd0;
    else if((cnt_sel == 2'd3) && (flag_1ms == 1'b1))
        cnt_sel <= 2'd0;
    else if(flag_1ms == 1'b1)
        cnt_sel <= cnt_sel + 1'd1;
    else
        cnt_sel <= cnt_sel;

//sel_reg
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        sel_reg <= 8'b0000_0000;
    else if((cnt_sel == 2'd0)&&(flag_1ms == 1'b1))
        sel_reg <= 8'b0001_0001;
    else if(flag_1ms == 1'b1)
        sel_reg <= sel_reg << 1;
    else
        sel_reg <= sel_reg;

//data_disp
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        data_disp <= 4'd0;
    else if(flag_1ms == 1'b1)
        case(cnt_sel)
            2'd0:       data_disp <= data[3:0];
            2'd1:       data_disp <= data[7:4];
            2'd2:       data_disp <= data[11:8];
            2'd3:       data_disp <= data[15:12];
            default:    data_disp <= 4'd0;
        endcase
    else
        data_disp <= data_disp;


//seg
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        seg <= 8'b0000_0000;
    else 
        case(data_disp)
            4'd0:       seg <= 8'b0011_1111;
            4'd1:       seg <= 8'b0000_0110;
            4'd2:       seg <= 8'b0101_1011;
            4'd3:       seg <= 8'b0100_1111;
            4'd4:       seg <= 8'b0110_0110;
            4'd5:       seg <= 8'b0110_1101;
            4'd6:       seg <= 8'b0111_1101;
            4'd7:       seg <= 8'b0000_0111;
            4'd8:       seg <= 8'b0111_1111;
            4'd9:       seg <= 8'b0110_1111;
            4'd10:      seg <= 8'b0111_0111;
            4'd11:      seg <= 8'b0111_1100;
            4'd12:      seg <= 8'b0011_1001;
            4'd13:      seg <= 8'b0101_1110;
            4'd14:      seg <= 8'b0111_1001;
            4'd15:      seg <= 8'b0111_0001;
            default:    seg <= 8'b0000_0000;
        endcase

//sel
always@(posedge sys_clk or negedge sys_rst_n)
    if(sys_rst_n == 1'b0)
        sel <= 8'b0000_0000;
    else
        sel <= sel_reg;

endmodule