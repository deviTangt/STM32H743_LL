module uart_rx
#(
	parameter 	BAUD_RATE 			= 115_200,
					S2_RX_MAX_BIT		= 4'd8, // 发送位数
					S3_END_MAX_BIT		= 4'd2  // 停止位数
)
(
	input wire 					sys_clk			,
	input wire					sys_rst_n		,
	input wire 					rx					,

	output reg 	[7 : 0]		rx_data			,
	output wire					rx_vld			,
	output wire					rx_done
);

localparam 	MAX_CNT_SYS_CLK 	= 50_000_000 / BAUD_RATE - 1;

localparam 	S0_IDLE				= 4'd0,
				S1_START 			= 4'd1, // 1位起始位
				S2_RX 				= 4'd2, // 8位数据位
				S3_END 				= 4'd3, // 停止位
				S4_FINISH 			= 4'd4;

reg 	[31 : 0] cnt_sys_clk; // 计数
reg 	[3 : 0]  state; // 状态机
reg 	[3 : 0]  cnt_bit; // 比特数

reg 	[7 : 0]  rx_buf; // 发送缓冲

// 状态机
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		state <= 4'd0;
	else begin
		case(state) 
			S0_IDLE: 
				state <= (rx == 1'b0) 						? S1_START 	: S0_IDLE;
			S1_START: 
				state <= (cnt_bit == 4'd1) 				? S2_RX 		: S1_START;
			S2_RX: 
				state <= (cnt_bit == S2_RX_MAX_BIT) 	? S3_END 	: S2_RX;
			S3_END: 
				state <= (cnt_bit == S3_END_MAX_BIT) 	? S4_FINISH : S3_END;
			S4_FINISH: 
				state <= S0_IDLE;
		endcase
	end
end

// 接收缓冲
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		rx_data <= 8'd0;
	else if (state == S3_END && cnt_sys_clk == MAX_CNT_SYS_CLK / 4)
		rx_data <= rx_buf;
	else
		rx_data <= rx_data;
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
			S2_RX: 
				if (cnt_bit == S2_RX_MAX_BIT)
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

// 接收数据位
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		rx_buf <= 8'b1;
	end else begin
		case(state) 
			S0_IDLE: 
				rx_buf <= 8'b0;
			S1_START: 
				rx_buf <= 8'b0;
			S2_RX: 
				if (cnt_sys_clk == MAX_CNT_SYS_CLK / 2)
					rx_buf[cnt_bit] <= rx;
				else
					rx_buf <= rx_buf;
			S3_END: 
				rx_buf <= rx_buf;
			S4_FINISH: 
				rx_buf <= rx_buf;
		endcase
	end
end

assign  	rx_vld 				= (state == S4_FINISH);
assign 	rx_done 				= (state == S4_FINISH);

endmodule
