//********************************//
//! 模块: uart_tx
//  功能: uart发送数据模块
//********************************//
module uart_tx
#(
	parameter 		BAUD_RATE 			= 115_200,
					S2_TX_MAX_BIT		= 4'd8, 		// 发送位数
					S3_END_MAX_BIT		= 4'd2  		// 停止位数
)
(
	input wire 					sys_clk			,
	input wire					sys_rst_n		,
	input wire 	[7 : 0]			tx_data			,
	input wire					tx_req			,
	
	output reg					tx				,
	output wire					tx_done
);

//**************************************// 参数列写         //**********************************//
localparam 	MAX_CNT_SYS_CLK 	= 50_000_000 / BAUD_RATE - 1;

localparam 	S0_IDLE				= 4'd0,
			S1_START 			= 4'd1, 		// 1位起始位
			S2_TX 				= 4'd2, 		// 8位数据位
			S3_END 				= 4'd3, 		// 停止位
			S4_FINISH 			= 4'd4;

//**************************************// 信号声明         //**********************************//
reg 	[31 : 0] cnt_sys_clk; 	// 计数
reg 	[3 : 0]  state; 		// 状态机
reg 	[3 : 0]  cnt_bit; 		// 比特数

reg 	[7 : 0]  tx_buf; 		// 发送缓冲

assign 	tx_done 				= (state == S4_FINISH);

//**************************************// 主程序段         //**********************************//
// 状态机
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		state <= 4'd0;
	else begin
		case(state) 
			S0_IDLE: 
				state <= (tx_req == 1'd1) 	? S1_START 	: S0_IDLE;
			S1_START: 
				state <= (cnt_bit == 4'd1) ? S2_TX 		: S1_START;
			S2_TX: 
				state <= (cnt_bit == S2_TX_MAX_BIT) ? S3_END 	: S2_TX;
			S3_END: 
				state <= (cnt_bit == S3_END_MAX_BIT) ? S4_FINISH : S3_END;
			S4_FINISH: 
				state <= S0_IDLE;
		endcase
	end
end

// 发送缓冲
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		tx_buf <= 8'd0;
	else if (tx_req)
		tx_buf <= tx_data;
	else
		tx_buf <= tx_buf;
end

// 时钟计数
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		cnt_sys_clk <= 32'b0;
	end else begin
		if (state == S0_IDLE) 
			cnt_sys_clk <= 32'b0;
		else if (cnt_sys_clk == MAX_CNT_SYS_CLK) 
			cnt_sys_clk <= 32'b0;
		else
			cnt_sys_clk <= cnt_sys_clk + 32'b1;
	end
end

// 比特计数
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		cnt_bit <= 4'b0;
	end else begin
		case(state)
			S0_IDLE: 
				cnt_bit <= 4'b0;
			S1_START: 
				if (cnt_bit == 4'd1)
					cnt_bit <= 4'b0;
				else if (cnt_sys_clk == MAX_CNT_SYS_CLK)
					cnt_bit <= cnt_bit + 4'b1;
			S2_TX: 
				if (cnt_bit == S2_TX_MAX_BIT)
					cnt_bit <= 4'b0;
				else if (cnt_sys_clk == MAX_CNT_SYS_CLK)
					cnt_bit <= cnt_bit + 4'b1;
			S3_END: 
				if (cnt_bit == S3_END_MAX_BIT)
					cnt_bit <= 4'b0;
				else if (cnt_sys_clk == MAX_CNT_SYS_CLK)
					cnt_bit <= cnt_bit + 4'b1;
			S4_FINISH: 
				cnt_bit <= 4'b0;
		endcase
	end
end

// 发送数据位
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		tx <= 1'b1;
	end else begin
		case(state) 
			S0_IDLE: 
				tx <= 1'b1;
			S1_START: 
				tx <= 1'b0;
			S2_TX: 
				tx <= tx_buf[cnt_bit];
			S3_END: 
				tx <= 1'b1;
			S4_FINISH: 
				tx <= 1'b1;
		endcase
	end
end

//**************************************// 子模块调用       //**********************************//
//**************************************// 模块结束         //**********************************//
endmodule
