//********************************//
//! 模块: uart字符串收发管理模块
//  功能: 可以指定发送字符串，也可以接收字符串
//        收发不定长字符串格式: &&data-string&&
//********************************//
module uart_string_handle#(
	parameter 		BAUD_RATE 	= 115_200
)
(
	input 	wire						sys_clk,
	input 	wire 						sys_rst_n,

    input   wire        [1023 : 0]      tx_string,
    input   wire        [7 : 0]         tx_length,
    input   wire                        tx_req,
    output  wire                        tx_busy,
    output  wire                        tx_done,

    output  reg         [1023 : 0]      rx_string,
    output  reg         [7 : 0]         rx_length,
    output  wire                        rx_busy,
    output  wire                        rx_done,

	input 	wire						uart_rx_port,
	output 	wire 						uart_tx_port
);

//**************************************// 参数列写         //**********************************//
//? 发送序列状态机
localparam 		ST0_IDLE			= 7'b000_0001,
				ST1_SIGN1 			= 7'b000_0010,  // &
				ST2_SIGN2 			= 7'b000_0100,  // &
				ST3_CONTENT 		= 7'b000_1000,  // 发送主体段
				ST4_SIGN3 			= 7'b001_0000,  // &
                ST5_SIGN4 			= 7'b010_0000,  // &
                ST6_FINISH 			= 7'b100_0000;

//? 接收序列状态机
localparam 		SR0_IDLE			= 7'b000_0001,
				SR1_SIGN1 			= 7'b000_0010,  // &
				SR2_SIGN2 			= 7'b000_0100,  // &
				SR3_CONTENT 		= 7'b000_1000,  // 接收主体段
				SR4_SIGN3 			= 7'b001_0000,  // &
                SR5_SIGN4 			= 7'b010_0000,  // &
                SR6_FINISH 			= 7'b100_0000;

//**************************************// 信号声明         //**********************************//
reg 	[6 : 0]  			state_tx; 			// 状态机
reg 	[6 : 0]  			state_rx; 			// 状态机

//? uart发送模块信号
reg    [1023 : 0]           tx_string_buf;
reg     [7 : 0]             tx_length_buf;
wire 						flag_tx_busy;
reg		[7 : 0]        		tx_byte_cnt;
reg							uart_tx_req;
reg 	[7 : 0]				uart_tx_data;
wire 						uart_tx_done;

//? uart接收模块信号
wire    [7 : 0]             uart_rx_data;
wire						uart_rx_vld;
wire						uart_rx_done;

assign                      tx_busy         = (state_tx != ST0_IDLE);
assign                      tx_done         = (state_tx == ST6_FINISH);

assign                      rx_busy         = (state_rx != SR0_IDLE);
assign                      rx_done         = (state_rx == SR6_FINISH);

//**************************************// 主程序段 发送部分      //**********************************//
//? 状态机
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		state_tx <= ST0_IDLE;
	else begin
		case(state_tx) 
			ST0_IDLE: 
				state_tx <= (tx_req && tx_length) 		    ? ST1_SIGN1 	    : ST0_IDLE;
			ST1_SIGN1: 
				state_tx <= (uart_tx_done) 	                ? ST2_SIGN2 	    : ST1_SIGN1;
			ST2_SIGN2: 
				state_tx <= (uart_tx_done) 			        ? ST3_CONTENT       : ST2_SIGN2;
			ST3_CONTENT: 
				state_tx <= (tx_byte_cnt == tx_length_buf)  ? ST4_SIGN3 	    : ST3_CONTENT;
			ST4_SIGN3: 
				state_tx <= (uart_tx_done) 			        ? ST5_SIGN4 	    : ST4_SIGN3;
            ST5_SIGN4:
                state_tx <= (uart_tx_done) 			        ? ST6_FINISH 	    : ST5_SIGN4;
            ST6_FINISH:
                state_tx <= ST0_IDLE;
		endcase
	end
end

//? 发送字符串管理
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
        uart_tx_data    <= 8'b0;
		uart_tx_req     <= 1'b0;
		tx_byte_cnt     <= 8'd0;
        tx_string_buf   <= 1024'b0;
        tx_length_buf   <= 8'd0;
	end else if (state_tx == ST0_IDLE) begin
        if (tx_req && tx_length) begin
            uart_tx_data    <= "&";
		    uart_tx_req     <= 1'b1;
            tx_string_buf   <= tx_string;
            tx_length_buf   <= tx_length;
        end else begin
            uart_tx_req     <= 1'b0;
            tx_byte_cnt     <= 8'd0;
        end
	end else if (state_tx == ST1_SIGN1 && uart_tx_done) begin
		uart_tx_data    <= "&";
        uart_tx_req     <= 1'b1;
	end else if (state_tx == ST2_SIGN2 && uart_tx_done) begin
        uart_tx_data    <= tx_string_buf[3 : 0];
		uart_tx_req     <= 1'b1;
		tx_byte_cnt     <= 8'd1;
	end else if (state_tx == ST3_CONTENT && uart_tx_done) begin
        if (tx_byte_cnt != tx_length_buf) begin
            uart_tx_data <= {tx_string_buf[(tx_byte_cnt << 2) + 8'd3], tx_string_buf[(tx_byte_cnt << 2) + 8'd2], tx_string_buf[(tx_byte_cnt << 2) + 8'd1], tx_string_buf[(tx_byte_cnt << 2) + 8'd0]};
            uart_tx_req <= 1'b1;
            tx_byte_cnt <= tx_byte_cnt + 8'd1;
        end else begin
            uart_tx_data    <= "&";
            uart_tx_req     <= 1'b1;
        end
    end else if (state_tx == ST4_SIGN3 && uart_tx_done) begin
		uart_tx_data    <= "&";
        uart_tx_req     <= 1'b1;
	end else
		uart_tx_req <= 1'b0;
end

//**************************************// 主程序段 接收部分      //**********************************//
//? 状态机
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		state_rx <= ST0_IDLE;
	else begin
		case(state_rx) 
			SR0_IDLE: 
				state_rx <= (uart_rx_vld && uart_rx_data == "&")    ? SR1_SIGN1 	    : SR0_IDLE;
			SR1_SIGN1: 
				state_rx <= (uart_rx_vld && uart_rx_data == "&")    ? SR2_SIGN2 	    : SR1_SIGN1;
			SR2_SIGN2: 
				state_rx <= SR3_CONTENT;
			SR3_CONTENT: 
                if (uart_rx_vld && uart_rx_data == "&")
                    state_rx <= SR4_SIGN3;
                else
                    state_rx <= SR3_CONTENT;
			SR4_SIGN3: 
                if (uart_rx_vld && uart_rx_data == "&")
                    state_rx <= SR5_SIGN4;
                else if (uart_rx_vld)
                    state_rx <= SR3_CONTENT;
                else 
                    state_rx <= SR4_SIGN3;
            SR5_SIGN4:
                state_rx <= SR6_FINISH;
            SR6_FINISH:
                state_rx <= SR0_IDLE;
		endcase
	end
end

integer i;
//? 接收字符串管理
always @(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		rx_length <= 8'd0;
		for (i = 0;i < 1024;i = i + 1) begin: for_loop_0
			rx_string[i] <= 1'b0;
		end
	end else begin if (state_rx == SR2_SIGN2) begin
        rx_length 	<= 8'd0; 
    end else if (state_tx == SR3_CONTENT && uart_rx_vld && uart_rx_data != "&") begin
        {
            rx_string[(rx_length << 2) + 8'd7], rx_string[(rx_length << 2) + 8'd6], 
            rx_string[(rx_length << 2) + 8'd5], rx_string[(rx_length << 2) + 8'd4], 
            rx_string[(rx_length << 2) + 8'd3], rx_string[(rx_length << 2) + 8'd2], 
            rx_string[(rx_length << 2) + 8'd1], rx_string[(rx_length << 2) + 8'd0]
         }  
                    <=  uart_rx_data;
		rx_length 	<= rx_length + 8'd1; 
	end else if (state_tx == SR4_SIGN3 && uart_rx_vld && uart_rx_data != "&") begin // +\r\n
		{
            rx_string[(rx_length << 2) + 8'd7], rx_string[(rx_length << 2) + 8'd6], 
            rx_string[(rx_length << 2) + 8'd5], rx_string[(rx_length << 2) + 8'd4], 
            rx_string[(rx_length << 2) + 8'd3], rx_string[(rx_length << 2) + 8'd2], 
            rx_string[(rx_length << 2) + 8'd1], rx_string[(rx_length << 2) + 8'd0]
         }  
                    <=  8'h26;//"&";
        {
            rx_string[(rx_length << 2) + 8'd15], rx_string[(rx_length << 2) + 8'd14], 
            rx_string[(rx_length << 2) + 8'd13], rx_string[(rx_length << 2) + 8'd12], 
            rx_string[(rx_length << 2) + 8'd11], rx_string[(rx_length << 2) + 8'd10], 
            rx_string[(rx_length << 2) + 8'd9 ], rx_string[(rx_length << 2) + 8'd8 ] 
         }  
                    <=  uart_rx_data;
		rx_length 	<= rx_length + 8'd2; 
	end
end

//**************************************// 子模块调用       //**********************************//
//? uart发送数据模块
uart_tx 
#(
	.BAUD_RATE			(BAUD_RATE),
	.S2_TX_MAX_BIT		(4'd8),
	.S3_END_MAX_BIT	    (4'd2)
) 
ins_uart_tx
(
	.sys_clk			(sys_clk),
	.sys_rst_n			(sys_rst_n),
	.tx_data			(uart_tx_data),
	.tx_req				(uart_tx_req),
	
	.tx					(uart_tx_port),
	.tx_done			(uart_tx_done)
);

//? uart接收数据模块
uart_rx 
#(
	.BAUD_RATE			(BAUD_RATE),
	.S2_RX_MAX_BIT		(4'd8),
	.S3_END_MAX_BIT	    (4'd1)
) 
ins_uart_rx
(
	.sys_clk			(sys_clk),
	.sys_rst_n			(sys_rst_n),

	.rx_data			(uart_rx_data),
	.rx_vld				(uart_rx_vld),

    .rx					(uart_rx_port),
	.rx_done       		(uart_rx_done)
);

//**************************************// 模块结束         //**********************************//
endmodule
