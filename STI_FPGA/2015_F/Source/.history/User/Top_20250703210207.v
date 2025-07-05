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

	// spi
	input 	wire                		cs							,
	input 	wire                		sclk						,
	input 	wire                		mosi						,
	output 	wire                		miso						,

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

//?	spi
wire                re_ack;

wire    [7 : 0]     slave_rx;

//**************************************// 主程序段         //**********************************//
//? 数码管值更新
always@(posedge sys_clk or negedge sys_rst_n) begin
	if (!sys_rst_n) begin
		dsp_data <= {4'd15,4'd10,4'd10,4'd10,4'd10,4'd10,4'd10,4'd10};
	end else begin
		if(rd_data < 10) begin
			dsp_data <= {4'd15,4'd10,4'd10,4'd10,4'd10,4'd10,4'd10, rd_data[3:0]};
		end else if(rd_data < 100 && rd_data >= 10) begin
			bcd[7:4] <= rd_data / 10;
			bcd[3:0] <= rd_data % 10;
			dsp_data <= {4'd15,4'd10,4'd10,4'd10,4'd10,4'd10,bcd[7:4], bcd[3:0]};
		end else begin
			bcd[11:8] <= rd_data / 100;
			bcd[7:4]  <= rd_data % 100 / 10;
			bcd[3:0]  <= rd_data % 10;
			dsp_data <= {4'd15, 4'd10, 4'd11, {2'b0, cs, sclk}, {2'b0, mosi, miso}, 4'd10, slave_rx[7 : 4], slave_rx[3 : 0]};
		end
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

//? spi从设备
spi_slave#(
	.CPOL           (1'b1)					    ,			// 时钟极性控制
	.CPHA           (1'b1)								    // 时钟相位控制
)
ins_spi_slave
(	
	.sys_clk		(sys_clk)                   ,     	    // 时钟信号
	.sys_rst_n		(sys_rst_n)                 ,   	    // 复位信号
                           
	.cs				(cs)                        ,           // 片选信号
	.sclk			(sclk)                      ,           // 时钟信号输出
	.mosi			(mosi)                      ,           // 主设备输出从设备输入数据线
	.miso			(miso)                      ,           // 主设备输入从设备输出数据线
                            
	.re_ack			(re_ack)                    ,      	    // 读应答信号
                             
	.data_tx		(slave_rx + 8'h10)           ,           // 输入数据
	.data_rx 		(slave_rx)                              // 输出数据
);  

//**************************************// 模块结束         //**********************************//
endmodule
