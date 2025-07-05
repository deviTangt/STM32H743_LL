//********************************//
//! 模块: spi_test
//  功能: spi_master与spi_slave测试模块
//********************************//
module spi_test(
    input  wire                     sys_clk         ,   //? 系统时钟
    input  wire                     sys_rst_n           //? 系统复位，低有效
);

//**************************************// 参数列写         //**********************************//


//**************************************// 信号声明         //**********************************//


//**************************************// 主程序段         //**********************************//



//**************************************// 子模块调用       //**********************************//
module spi_master#(
	.CPOL = 1'b1						,			// 时钟极性控制
	.CPHA = 1'b1									// 时钟相位控制
)
(	
	input  	wire                    sys_clk			,     		// 时钟信号
	input  	wire                    sys_rst_n		,   		// 复位信号

	output 	wire                    cs				,          	// 片选信号
	output 	reg                    	sclk			,        	// 时钟信号输出
	output 	wire                    mosi			,        	// 主设备输出从设备输入数据线
	input  	wire                    miso			,        	// 主设备输入从设备输出数据线

	input  	wire                    cs_ctrl			,     		// 片选控制信号 cs = cs_ctrl
	input  	wire	[15:0]      	clk_div_val		,          	// 时钟分频值

	input  	wire                    wr_req			,      		// 写请求信号
	output 	wire                    wr_ack			,      		// 写应答信号

	input  	wire	[7:0]			data_tx			,           // 输入数据
	output 	wire	[7:0]       	data_rx 		          	// 输出数据
);

module spi_slave#(
	.CPOL = 1'b1						,			// 时钟极性控制
	.CPHA = 1'b1									// 时钟相位控制
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
//**************************************// 模块结束         //**********************************//
endmodule
