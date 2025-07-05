//********************************//
//! 模块: 
//  功能: 
//********************************//
module adc(
	input  wire                     sys_clk         ,   //? 系统时钟
	input  wire                     sys_rst_n           //? 系统复位，低有效
);

//**************************************// 参数列写         //**********************************//

parameter               device_id       = 7'b1010_100,
						reg_addr        = 8'b0000_0000;

//**************************************// 信号声明         //**********************************//

reg     [7 : 0]         adc_data_temp;

//**************************************// 主程序段         //**********************************//
// 功能： 串口发送什么，返回什么+\r\n,接收最大连续间隔可以设置
module uart_control(
	input 	wire					sys_clk,
	input 	wire 					sys_rst_n,
	
	output 	reg 		[7 : 0]     test_data,
	
	output 	wire		[7 : 0]		uart_rx_data,
	input 	wire					uart_rx_port,
	output 	wire 					uart_tx_port
);

wire 						uart_tx_done;

wire						uart_rx_vld;
wire						uart_rx_done;

reg 	[7 : 0]			rx_byte_cnt;
reg 	[7 : 0]  		rx_string 	[63 : 0];
reg 	[31 : 0] 		rx_time_cnt;

wire 						flag_tx_busy;
reg	[7 : 0]        tx_byte_cnt;
reg						uart_tx_req;
reg 	[7 : 0]			uart_tx_data;

reg 	[3 : 0]  		state; // 状态机
localparam 	S0_IDLE				= 4'd0,
				S1_WAIT 				= 4'd1, 
				S2_TX 				= 4'd2, // 发送字符串
				S3_ACK 				= 4'd3, 
				S4_FINISH 			= 4'd4;

localparam 	MAX_WAIT_DELAY_CLK 	= 50_000_000 / 1000 - 1;  // 最大连续接收时延 1ms

always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		test_data[3 : 0] <= 4'd0;
	else if (state == S2_TX)
		test_data[3 : 0] <= rx_byte_cnt;
end

// 状态机
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		state <= 4'd0;
	else begin
		case(state) 
			S0_IDLE: 
				state <= (uart_rx_vld) 								? S1_WAIT 	: S0_IDLE;
			S1_WAIT: 
				state <= (rx_time_cnt == MAX_WAIT_DELAY_CLK) ? S2_TX 		: S1_WAIT;
			S2_TX: 
				state <= (tx_byte_cnt == rx_byte_cnt) 			? S3_ACK 	: S2_TX;
			S3_ACK: 
				state <= S4_FINISH;
			S4_FINISH: 
				state <= S0_IDLE;
		endcase
	end
end

integer i;
// 接收统计
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		rx_byte_cnt <= 8'd0;
		for (i = 0;i < 64;i = i + 1) begin: for_loop_0
			rx_string[i] <= 8'b0;
		end
	end else begin
		if ((state == S0_IDLE || state == S1_WAIT) && uart_rx_vld) begin
			rx_string[rx_byte_cnt] 	<= uart_rx_data;
			rx_byte_cnt 				<= rx_byte_cnt + 8'd1; 
		end else if (state == S1_WAIT && rx_time_cnt == MAX_WAIT_DELAY_CLK) begin // +\r\n
			rx_string[rx_byte_cnt] 			<= "\r";
			rx_string[rx_byte_cnt + 1] 	<= "\n";
			rx_byte_cnt 				<= rx_byte_cnt + 8'd2;
		end else if (state == S2_TX && tx_byte_cnt == rx_byte_cnt) begin
			rx_byte_cnt 				<= 8'd0;
		end
	end
end

// 等待时钟计数(超过1ms未接收到数据后发送之前接收到的数据)
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		rx_time_cnt <= 32'b0;
	end else begin
		if (state != S1_WAIT) 
			rx_time_cnt <= 32'b0;
		else begin 
			if (rx_time_cnt == MAX_WAIT_DELAY_CLK) 
				rx_time_cnt <= 32'b0;
			else if (uart_rx_vld)
				rx_time_cnt <= 32'b0;
			else
				rx_time_cnt <= rx_time_cnt + 32'b1;
		end
	end
end

// 发送字符串管理
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		uart_tx_req <= 1'b0;
		tx_byte_cnt <= 8'd0;
	end else if (state == S0_IDLE) begin
		uart_tx_req <= 1'b0;
		tx_byte_cnt <= 8'd0;
	end else if (state == S1_WAIT && rx_time_cnt == MAX_WAIT_DELAY_CLK) begin
		uart_tx_data <= rx_string[0];
		uart_tx_req <= 1'b1;
		tx_byte_cnt <= tx_byte_cnt + 8'd1;
	end else if (state == S2_TX && tx_byte_cnt == rx_byte_cnt) begin
		uart_tx_req <= 1'b0;
		tx_byte_cnt <= 8'd0;
	end else if (state == S2_TX && uart_tx_done) begin
		uart_tx_data <= rx_string[tx_byte_cnt];
		uart_tx_req <= 1'b1;
		tx_byte_cnt <= tx_byte_cnt + 8'd1;
	end else
		uart_tx_req <= 1'b0;
end
assign flag_tx_busy			= (state == S2_TX);



//**************************************// 子模块调用       //**********************************//
//? uart发送数据模块
uart_tx 
#(
	.BAUD_RATE			(115_200),
	.S2_TX_MAX_BIT		(4'd8),
	.S3_END_MAX_BIT		(4'd2)
) 
ins_uart_tx
(
	.sys_clk			(sys_clk),
	.sys_rst_n			(sys_rst_n),
	.tx_data			(uart_tx_data),//("a"), uart_rx_data
	.tx_req				(uart_tx_req), // uart_tx_req
	
	.tx					(uart_tx_port),
	.tx_done			(uart_tx_done)
);

//? uart接收数据模块
uart_rx 
#(
	.BAUD_RATE			(115_200),
	.S2_RX_MAX_BIT		(4'd8),
	.S3_END_MAX_BIT		(4'd1)
) 
ins_uart_rx
(
	.sys_clk			(sys_clk),
	.sys_rst_n			(sys_rst_n),
	.rx					(uart_rx_port),

	.rx_data			(uart_rx_data),
	.rx_vld				(uart_rx_vld),
	.rx_done       		(uart_rx_done)
);

//**************************************// 模块结束         //**********************************//
endmodule
