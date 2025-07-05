//********************************//
//! 模块: my_adc
//  功能: adc驱动模块
//********************************//
module my_adc(
	input 	wire						sys_clk		   		,
	input 	wire						sys_rst_n	   		,
	
	input   wire						wr_req      		, //#
	input   wire						rd_req      		,

	input   wire	[6 : 0]				device_id   		,
	input   wire    [7 : 0]				reg_addr    		,
	input   wire						reg_addr_vld		,

	input   wire    [7 : 0]				wr_data     		, //#
	input   wire						wr_data_vld 		, //#

	output  wire    [7 : 0]				rd_data     		,
	output  wire						rd_data_vld 		, //#
	output  wire						ready       		, //#
	
	output                        		scl         		,
	inout                         		sda 						
);

//**************************************// 参数列写         //**********************************//
//cmd para define
`define     	START_BIT   5'b00001
`define     	WRITE_BIT   5'b00010
`define     	READ_BIT    5'b00100
`define     	STOP_BIT    5'b01000
`define     	ACK_BIT     5'b10000

reg		[3 : 0]				state;
localparam 					S0_IDLE		= 4'd0,
							S1_WRITE		= 4'd1,
							S2_READ		= 4'd2,
							S3_FINISH	= 4'd3;

//**************************************// 信号声明         //**********************************//
// i2c
reg		[7 : 0]				op_wr_data;
reg		[4 : 0]				cmd;
reg							cmd_vld;
wire	[7 : 0]				op_rd_data;
wire 						done;

//
reg 	[7 : 0]				adc_data_h, adc_data_l;
assign						rd_data 		= {adc_data_h[3 : 0], adc_data_l[7 : 4]};
reg		[3 : 0]				cnt_byte;

reg		[6 : 0]				device_id_buf;
reg		[7 : 0]				reg_addr_buf;
reg		[7 : 0]				wr_data_buf

//**************************************// 主程序段         //**********************************//
// 状态机
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		state <= 4'd0;
	else begin
		case(state)
			S0_IDLE:
				if 		(wr_req)
					state <= S1_WRITE;
				else if 	(rd_req)
					state <= S2_READ;
			S1_WRITE:
				if (cnt_byte == 4'd3)
					state <= S3_FINISH;
			S2_READ:
				if (cnt_byte == 4'd5)
					state <= S3_FINISH;
			S3_FINISH:
				state <= S0_IDLE;
		endcase
	end
end

// 字节计数
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) 
		cnt_byte <= 4'd0;
	else begin
		case(state)
			S0_IDLE:
				cnt_byte <= 4'd0;
			S1_WRITE:
				if (cnt_byte == 4'd2)
					cnt_byte <= 4'd0;
				else if (done)
					cnt_byte <= cnt_byte + 4'd1;
			S2_READ:
				if (cnt_byte == 4'd4)
					cnt_byte <= 4'd0;
				else if (done)
					cnt_byte <= cnt_byte + 4'd1;
			S3_FINISH:
				cnt_byte <= 4'd0;
		endcase
	end
end

// 设备id和设备写寄存器地址缓冲
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		device_id_buf 	<= 7'b0;
		reg_addr_buf	<= 8'b0;
	end else if (reg_addr_vld) begin
		device_id_buf 	<= device_id;
		reg_addr_buf	<= reg_addr;
	end
end

// 写数据地址缓冲
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		wr_data_buf		<= 8'b0;
	end else if (reg_addr_vld) begin
		wr_data_buf		<= wr_data;
	end
end

// 指令请求与写数据
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		cmd 			<= 5'b0;
		cmd_vld 		<= 1'b0;
		op_wr_data 	<= 8'b0;
	end else begin
		case(state)
			S0_IDLE: begin
				if 		(wr_req) begin
					cmd_vld <= 1'b1; cmd <= (`START_BIT | `WRITE_BIT 	); op_wr_data <= {device_id, 1'b0};  	 
				end else if  (rd_req) begin
					cmd_vld <= 1'b1; cmd <= (`START_BIT | `WRITE_BIT 	); op_wr_data <= {device_id, 1'b0};
				end else begin
					cmd <= 5'b0;
					cmd_vld <= 1'b0;
					op_wr_data <= 8'b0;
				end
			end
			
			S1_WRITE: begin
				if (done)
					case(cnt_byte)
						9: begin cmd_vld <= 1'b1; cmd <= (`START_BIT | `WRITE_BIT 	); op_wr_data <= {device_id, 1'b0};  		end
						0: begin cmd_vld <= 1'b1; cmd <= (`WRITE_BIT 					); op_wr_data <= {reg_addr_buf}; 			end
						1: begin cmd_vld <= 1'b1; cmd <= (`WRITE_BIT | `STOP_BIT		); op_wr_data <= {wr_data_buf}; 				end
					endcase
				else
					cmd <= 1'b0;
			end
			
			S2_READ: begin
				if (done)
					case(cnt_byte)
						9: begin cmd_vld <= 1'b1; cmd <= (`START_BIT | `WRITE_BIT 	); op_wr_data <= {device_id, 1'b0};  		end
						0: begin cmd_vld <= 1'b1; cmd <= (`WRITE_BIT 					); op_wr_data <= {reg_addr_buf}; 			end
						1: begin cmd_vld <= 1'b1; cmd <= (`START_BIT | `WRITE_BIT 	); op_wr_data <= {device_id_buf, 1'b1}; 	end
						2: begin cmd_vld <= 1'b1; cmd <= (`READ_BIT 						); adc_data_l <= {op_rd_data}; 				end
						3: begin cmd_vld <= 1'b1; cmd <= (`READ_BIT  | `STOP_BIT	   ); adc_data_h <= {op_rd_data}; 				end
					endcase
				else
					cmd_vld <= 1'b0;
			end
			
			S3_FINISH: begin
				cmd_vld  	<= 1'b0;
				cmd 			<= 5'b0;
				op_wr_data 	<= 8'b0;
			end
		endcase
	end
end

//**************************************// 子模块调用       //**********************************//
//? i2c驱动模块
i2c inst_i2c(
	.sys_clk		 			(sys_clk)				,
	.sys_rst_n	    			(sys_rst_n)				,
	.wr_data     				(op_wr_data)			,
	.cmd         				(cmd)					,
	.cmd_vld     				(cmd_vld)				,
	.rd_data     				(op_rd_data)			,
	.rd_data_vld 				(rd_data_vld)			,
	.done        				(done)					,
	.scl         				(scl)					,
	.sda         				(sda)					
);

//**************************************// 模块结束         //**********************************//
endmodule
