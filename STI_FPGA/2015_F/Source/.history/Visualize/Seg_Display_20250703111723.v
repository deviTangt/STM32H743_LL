//********************************//
//! 模块: adc
//  功能: 采集adc数据
//********************************//
module adc(
	input wire                  sys_clk         ,   //? 系统时钟
	input wire                  sys_rst_n       ,   //? 系统复位，低有效

	input wire                  en_read         ,   // ad采样读使能

	output wire [7 : 0]         adc_data        ,   // adc采样数据
	output wire                 adc_data_vld    ,   // adc采样数据有效

	output wire                 sda             ,   // iic数据线
	inout wire                  scl                 // iic时钟线
);
//**************************************// 参数列写         //**********************************//

parameter               device_id       = 7'b1010_100,
						reg_addr        = 8'b0000_0000;

//**************************************// 信号声明         //**********************************//

reg     [7 : 0]         adc_data_temp;

//**************************************// 主程序段         //**********************************//



//**************************************// 子模块调用       //**********************************//

//? IIC模块
i2c inst_i2c(
   .clk            (clk)           ,
   .rst            (rst)           ,
   .wr_data        (op_wr_data)    ,
   .cmd            (cmd)           ,
   .cmd_vld        (cmd_vld)       ,
   .rd_data        (op_rd_data)    ,
   .rd_data_vld    (rd_data_vld)   ,
   .done           (done)          ,
   .scl            (scl)           ,
   .sda            (sda)           
);

//**************************************// 模块结束         //**********************************//
endmodule

module Seg_Display(
	input wire 				sys_clk,
	input wire 				sys_rst_n,
	input wire 	[31 : 0]	seg_val,
	input wire  [7 : 0]	seg_sel,
	
	output wire	[7 : 0]	seg_val_out,
	output wire	[7 : 0]	seg_sel_out
);

wire 	clk_1000;
divider #(50_000_000, 1000) 				d1(sys_clk, sys_rst_n, clk_1000);

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

reg [2 : 0] sel;
always@(negedge sys_rst_n or posedge clk_1000) begin
	if (~sys_rst_n)
		sel	<= 3'b0;
	else 
		sel	<= sel + 3'b1;
end


genvar i;
generate
	for (i = 0;i < 8;i = i + 1) begin:for_loop_1
		assign seg_sel_out[i] 	=  ~(sel == i && seg_sel[i]);
	end
endgenerate

assign seg_val_out	=  val_decoder[{seg_val[4 * (7 - sel) + 3], seg_val[4 * (7 - sel) + 2], seg_val[4 * (7 - sel) + 1], seg_val[4 * (7 - sel) + 0]}];	


//*********************************************// end	
endmodule
