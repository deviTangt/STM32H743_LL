//********************************//
//! 模块: Seg_Display
//  功能: 数码管驱动
//********************************//
module Seg_Display(
	input 	wire 							sys_clk				,
	input 	wire 							sys_rst_n			,
	input 	wire 	[31 : 0]				seg_val				,
	input 	wire  	[7 : 0]					seg_sel				,

	output 	wire	[7 : 0]					seg_val_out			,
	output 	wire	[7 : 0]					seg_sel_out			
);
//**************************************// 参数列写         //**********************************//

parameter               device_id       = 7'b1010_100,
						reg_addr        = 8'b0000_0000;

//**************************************// 信号声明         //**********************************//

// 数码管刷新频率
wire 	clk_1000;
divider #(50_000_000, 1000) 				d1(sys_clk, sys_rst_n, clk_1000);

// 数码管字符映射表
reg [7 : 0] val_decoder [15 : 0];
initial begin	
	val_decoder[0]  <= 8'hC0;
	val_decoder[1]  <= 8'hF9;
	val_decoder[2]  <= 8'hA4;
	val_decoder[3]  <= 8'hB0;
	val_decoder[4]  <= 8'h99;
	val_decoder[5]  <= 8'h92;
	val_decoder[6]  <= 8'h82;
	val_decoder[7]  <= 8'hF8;
	val_decoder[8]  <= 8'h80;
	val_decoder[9]  <= 8'h90;
	val_decoder[10] <= 8'h88;
	val_decoder[11] <= 8'h83;
	val_decoder[12] <= 8'ha7;
	val_decoder[13] <= 8'ha1;
	val_decoder[14] <= 8'h84;
	val_decoder[15] <= 8'h8e;
end

//**************************************// 主程序段         //**********************************//
//? 数码管片选移位计数
reg [2 : 0] sel;
always@(negedge sys_rst_n or posedge clk_1000) begin
	if (~sys_rst_n)
		sel	<= 3'b0;
	else 
		sel	<= sel + 3'b1;
end

//? 数码管显示值选择
genvar i;
generate
	for (i = 0;i < 8;i = i + 1) begin:for_loop_1
		assign seg_sel_out[i] 	=  ~(sel == i && seg_sel[4'D7 - i]);
	end
endgenerate

//? GPIO引脚赋值
assign seg_val_out	=  val_decoder[{seg_val[4 * (7 - sel) + 3], seg_val[4 * (7 - sel) + 2], seg_val[4 * (7 - sel) + 1], seg_val[4 * (7 - sel) + 0]}];	

//**************************************// 子模块调用       //**********************************//
//**************************************// 模块结束         //**********************************//
endmodule
