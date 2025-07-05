//********************************//
//! 模块: spi_slave
//  功能: SPI主机模块，包含输入输出信号
//********************************//
module spi_slave#(
	parameter 		CPOL = 1'b1						,			// 时钟极性控制
					CPHA = 1'b1									// 时钟相位控制
)
(	
	input  	wire                    sys_clk			,     		// 时钟信号
	input  	wire                    sys_rst_n		,   		// 复位信号

	input 	wire                    cs				,          	// 片选信号
	input 	wire                    sclk			,        	// 时钟信号输出
	input 	wire                    mosi			,        	// 主设备输出从设备输入数据线
	output  wire                    miso			,        	// 主设备输入从设备输出数据线

	input  	wire	[15:0]      	clk_div_val		,          	// 时钟分频值

	output 	wire                    re_ack			,      		// 读应答信号

	input  	wire	[7:0]			data_tx			,           // 输入数据
	output 	wire	[7:0]       	data_rx 		          	// 输出数据
);

//**************************************// 参数列写         //**********************************//
//? 状态机
localparam					S0_IDLE			= 4'd0,				// 空闲
							S1_SCLK_IDLE	= 4'd1,				// sclk时钟等待
							S2_SCLK_EDGE	= 4'd2,				// sclk时钟边沿
							S3_LAST_HALF	= 4'd3,				// 最终等待
							S4_ACK			= 4'd4,				// 发送ack
							S5_FINISH		= 4'd5;				// 执行1次spi交换完毕

//**************************************// 信号声明         //**********************************//
reg 	[7 : 0] 			mosi_shift;							// mosi移位寄存器
reg		[7 : 0]				miso_shift;							// miso移位寄存器
reg		[3 : 0]				state;								// 状态寄存器
reg		[15 : 0]			cnt_clk;							// 时钟计数器
reg		[4 : 0]				cnt_sclk_edge;						// sclk边沿计数器

assign 		mosi			= mosi_shift[7];					// 输出数据
assign 		data_rx 		= miso_shift;						// 接收数据
assign		wr_ack			= (state == S4_ACK);				// 交换字节完成ack

//? cs同步
reg 		cs_pre;
always @(negedge sys_rst_n or posedge sys_clk)
	if (!sys_rst_n)	cs_pre = cs;
	else 			cs_pre = cs;
wire   		cs_pos;
wire  		cs_neg;
assign 		cs_pos = ~cs_pre & cs;
assign 		cs_neg = cs_pre & ~cs;	

//? cs同步
reg 		cs_pre;
always @(negedge sys_rst_n or posedge sys_clk)
	if (!sys_rst_n)	cs_pre = cs;
	else 			cs_pre = cs;
wire   		cs_pos;
wire  		cs_neg;
assign 		cs_pos = ~cs_pre & cs;
assign 		cs_neg = cs_pre & ~cs;	
//**************************************// 主程序段         //**********************************//
//? 状态机更迭
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		state	<= S0_IDLE;
	else case(state)
		S0_IDLE:	
			if (wr_req)
				state	<= S1_SCLK_IDLE;
			else 
				state	<= S0_IDLE;
		S1_SCLK_IDLE:	
			// 等待时间
			if (cnt_clk == clk_div_val)
				state	<= S2_SCLK_EDGE;
			else 
				state	<= S1_SCLK_IDLE;
		S2_SCLK_EDGE:	
			if (cnt_sclk_edge == 5'd15)
				state	<= S3_LAST_HALF;
			else 
				state	<= S1_SCLK_IDLE;
		S3_LAST_HALF:	
			if (cnt_clk == clk_div_val)
				state	<= S4_ACK;
			else 
				state	<= S3_LAST_HALF;
		S4_ACK:	
				state	<= S5_FINISH;
		S5_FINISH:	
				state	<= S0_IDLE;
	endcase
end

//? 时钟计数
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		cnt_clk	<= 16'b0;
	else if (state == S1_SCLK_IDLE || state == S3_LAST_HALF)
		cnt_clk	<= cnt_clk + 16'b1;
	else
		cnt_clk	<= 16'b0;
end

//? sclk边沿计数
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		cnt_sclk_edge	<= 5'b0;
	else if (state == S2_SCLK_EDGE)
		cnt_sclk_edge	<= cnt_sclk_edge + 5'b1;
	else if (state == S0_IDLE)
		cnt_sclk_edge	<= 5'b0;
	else
		cnt_sclk_edge	<= cnt_sclk_edge;
end

//? sclk时钟 
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		sclk	<= 1'b0;
	else if (state == S0_IDLE)
		sclk	<= CPOL;
	else if (state == S2_SCLK_EDGE)
		sclk	<= ~sclk;
	else
		sclk	<= sclk;
end

//? mosi移位
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		mosi_shift	<= 8'b0;
	else if (state == S0_IDLE && wr_req)
		mosi_shift	<= data_tx;
	else if (CPHA == 1'b0 && state == S2_SCLK_EDGE && cnt_sclk_edge[0] == 1'b1)
		mosi_shift	<= {mosi_shift[6 : 0], mosi_shift[7]};
	else if (CPHA == 1'b1 && state == S2_SCLK_EDGE && (cnt_sclk_edge != 5'd0 && cnt_sclk_edge[0] == 1'b0))
		mosi_shift	<= {mosi_shift[6 : 0], mosi_shift[7]};
	else
		mosi_shift	<= mosi_shift;
end

//? miso移位
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		miso_shift	<= 8'b0;
	else if (state == S0_IDLE && wr_req)
		miso_shift	<= 8'b0;
	else if (CPHA == 1'b0 && state == S2_SCLK_EDGE && cnt_sclk_edge[0] == 1'b0)
		miso_shift	<= {miso_shift[6 : 0], miso};
	else if (CPHA == 1'b1 && state == S2_SCLK_EDGE && cnt_sclk_edge[0] == 1'b1)
		miso_shift	<= {miso_shift[6 : 0], miso};
	else
		miso_shift	<= miso_shift;
end

//**************************************// 子模块调用       //**********************************//
//**************************************// 模块结束         //**********************************//
endmodule
