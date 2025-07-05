//********************************//
//! 模块: Top
//  功能: 顶层文件
//********************************//
module Top(
    input 	wire                     	sys_clk         			,
    input 	wire                      	sys_rst_n       			,

    input 	wire    [3 : 0]	        	key_in          			,

	output 	wire				        bepeer          			,
	output 	wire	[7 : 0]	        	led             			,

	// 数码管
	output 	wire    [7 : 0] 	    	seg_number	    			,
	output 	wire 	[7 : 0]		    	seg_choice	    			,

	// uart - mcu
	input 	wire						uart_rx_port				,
	output 	wire 						uart_tx_port				,
	output 	wire 						uart_3v3_port				,
	output 	wire 						uart_gnd_port				,

	// iic
   	output 	wire        		    	scl     	    			,
   	inout	wire      			    	sda     					
);

//**************************************// 参数列写         //**********************************//

//**************************************// 信号声明         //**********************************//
//? 数码管驱动模块
reg     [31 : 0]                seg_data;

//? 按键模块
wire    [3 : 0]                 sta_key;   
wire    [3 : 0]                 sta_key_pos;   

//? signal define
wire    [7:0]   				rd_data		;
reg		[11:0]					bcd			;
reg		[31:0]					dsp_data	;

//? uart - mcu
wire	[7 : 0]					uart_rx_data;
wire 	[7 : 0]					uart_test_data;

assign 							uart_3v3_port 	= 1'b1;
assign 							uart_gnd_port 	= 1'b0;

//**************************************// 主程序段         //**********************************//
//? 数码管值更新
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		dsp_data <= {4'd15,4'd10,4'd10,4'd10,4'd10,4'd10,4'd10,4'd10};
	end else begin
		dsp_data <= {	
						4'd15, 
						4'd10, 
						4'd11, 
						uart_test_data[7 : 4], 
						uart_test_data[3 : 0], 
						4'd10, 
						uart_rx_data[7 : 4], 
						uart_rx_data[3 : 0]
					};
	end 
end

//**************************************// 子模块调用       //**********************************//

//? adc驱动模块
my_adc adc_inst(
	.sys_clk		 				(sys_clk)					,
	.sys_rst_n	     				(sys_rst_n)					,

	.rd_req      					(1'b1)						,
	.device_id   					(7'b101_0100)				,
	.reg_addr    					(8'h00)						,
	.reg_addr_vld					(1'b1)						,

	.rd_data     					(rd_data)					,
	.rd_data_vld 					()							,

	.scl         					(scl)						,
	.sda         					(sda)						
);

//? 数码管驱动模块
Seg_Display ins_Seg_Display(
	.sys_clk						(sys_clk)                   ,
	.sys_rst_n				    	(sys_rst_n)                 ,
	.seg_val						(dsp_data)                  ,
	.seg_sel						(8'b1111_1011)              ,

	.seg_val_out			    	(seg_number)                ,
	.seg_sel_out			    	(seg_choice)
);

//? 按键模块
Key ins_Key(
	.sys_clk                    	(sys_clk)                   ,
	.sys_rst_n                  	(sys_rst_n)                 ,
	.key_in                     	(key_in)                    ,

	.sta_key                    	(sta_key)                   ,
	.sta_key_pos                	(sta_key_pos)               ,

	.led                        	(led)
);

//? 蜂鸣器模块
Buzzer ins_Buzzer(
	.sys_clk                    	(sys_clk)                   ,
	.sys_rst_n                  	(sys_rst_n)                 ,

	.sta_key                    	(0 & sta_key)               ,
	.sta_key_pos                	(0 & sta_key_pos)           ,

	.bepeer                     	(bepeer)
);

//? uart发送接收管理模块
uart_try#(
	.BAUD_RATE 	(115_200)
)
ins_uart_try
(
	.sys_clk			,
	.sys_rst_n			,

	.uart_rx_data		,
	.uart_rx_port		,
	.uart_tx_port		
);



//**************************************// 模块结束         //**********************************//
endmodule
