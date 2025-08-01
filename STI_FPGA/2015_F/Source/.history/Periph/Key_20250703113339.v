//********************************//
//! 模块: Key
//  功能: 按键驱动
//********************************//
module Key(
	input 	wire 							sys_clk				,
	input 	wire 							sys_rst_n			,
	input 	wire 	[3 : 0]					key_in				,

	output 	reg 	[3 : 0] 				sta_key				,
	output 	wire 	[3 : 0] 				sta_key_pos			,

	output 	reg		[7 : 0]					led					
);

//**************************************// 参数列写         //**********************************//
//**************************************// 信号声明         //**********************************//
//? 按键计数信号
wire 			clk_1s;
wire 			clk_1ms;
reg				clk_1ms_pre;
wire 			syn_pos_clk_1ms = ~clk_1ms_pre & clk_1ms;
wire 			clk_1us;
divider 		#(50_000_000, 1) 				d1(sys_clk, sys_rst_n, clk_1s);
divider 		#(50_000_000, 1_000) 			d2(sys_clk, sys_rst_n, clk_1ms);
divider 		#(50_000_000, 1_000_000) 		d3(sys_clk, sys_rst_n, clk_1us);
// 同步计数信号
always@(negedge sys_rst_n or posedge sys_clk) 
	if (~sys_rst_n) 		clk_1ms_pre	<= clk_1ms;
	else					clk_1ms_pre	<= clk_1ms;

reg [3 : 0] sta_key_p;
reg [3 : 0] sta_key_pre;
assign sta_key_pos = ~sta_key_pre & sta_key_p;
reg [7 : 0] cnt_key_ms;

//**************************************// 主程序段         //**********************************//
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		cnt_key_ms	<= 8'b0;
	else if (key_in == 4'b1111)
		cnt_key_ms	<= 8'b0;
	else if (cnt_key_ms == 8'd20)
		cnt_key_ms	<= cnt_key_ms;
	else if (syn_pos_clk_1ms)
		cnt_key_ms 	<= cnt_key_ms + 8'b1;
end

always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		sta_key		<= 4'b0000;
	else if (cnt_key_ms == 8'd20)
		if 	  (~key_in[0])	sta_key <= 4'b0001;
		else if (~key_in[1])	sta_key <= 4'b0010;
		else if (~key_in[2])	sta_key <= 4'b0100;
		else if (~key_in[3])	sta_key <= 4'b1000;
		else						sta_key <= 4'b0000;
	else 
		sta_key		<= 4'b0000;
end

always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		sta_key_p		<= 4'b0000;
	else if (cnt_key_ms == 8'd19)
		if 	  (~key_in[0])	sta_key_p <= 4'b0001;
		else if (~key_in[1])	sta_key_p <= 4'b0010;
		else if (~key_in[2])	sta_key_p <= 4'b0100;
		else if (~key_in[3])	sta_key_p <= 4'b1000;
		else						sta_key_p <= 4'b0000;
	else 
		sta_key_p		<= 4'b0000;
end

//? 按键状态同步
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		sta_key_pre	<= sta_key_p;
	else 
		sta_key_pre	<= sta_key_p;
end

//? led
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		led[3 : 0]	<= ~4'b0;
	else if (sta_key == 4'b0001)
		led[0]		<= 1'b0;
	else if (sta_key == 4'b0010)
		led[1]		<= 1'b0;
	else if (sta_key == 4'b0100)
		led[2]		<= 1'b0;
	else if (sta_key == 4'b1000)
		led[3]		<= 1'b0;
	else 
		led[3 : 0]	<= ~4'b0;
end	

//? led
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		led[7 : 4]	<= ~4'b0;
	else if (sta_key_pos[0])
		led[4]		<= ~led[4];
	else if (sta_key_pos[1])
		led[5]		<= ~led[5];
	else if (sta_key_pos[2])
		led[6]		<= ~led[6];
	else if (sta_key_pos[3])
		led[7]		<= ~led[7];
end	

//**************************************// 子模块调用       //**********************************//

//**************************************// 模块结束         //**********************************//
endmodule
