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
wire                cs;
wire                sclk;
wire                mosi;
wire                miso;

wire                wr_ack;
wire                re_ack;

wire    [7 : 0]     master_rx;
wire    [7 : 0]     slave_rx;

//**************************************// 主程序段         //**********************************//



//**************************************// 子模块调用       //**********************************//
spi_master#(
	.CPOL           (1'b1)						,			// 时钟极性控制
	.CPHA           (1'b1)									// 时钟相位控制
)
ins_spi_master
(	
	.sys_clk		(sys_clk)                          ,     		// 时钟信号
	.sys_rst_n		(sys_rst_n)                          ,   		// 复位信号
                    
	.cs				(cs)                          ,          	// 片选信号
	.sclk			(sclk)                          ,        	// 时钟信号输出
	.mosi			(mosi)                          ,        	// 主设备输出从设备输入数据线
	.miso			(miso)                          ,        	// 主设备输入从设备输出数据线
                         
	.cs_ctrl		()                          ,     		// 片选控制信号 cs = cs_ctrl
	.clk_div_val	()                          ,          	// 时钟分频值
                     
	.wr_req			(wr_ack)                          ,      		// 写请求信号
	.wr_ack			()                          ,      		// 写应答信号
                      
	.data_tx		(master_rx + 8'b1)                          ,           // 输入数据
	.data_rx 		(master_rx)                                     	// 输出数据
);

spi_slave#(
	.CPOL               (1'b1)					,			// 时钟极性控制
	.CPHA               (1'b1)								// 时钟相位控制
)
ins_spi_slave
(	
	.sys_clk		(sys_clk)                          ,     	    // 时钟信号
	.sys_rst_n		(sys_rst_n)                          ,   	    // 复位信号
                           
	.cs				(cs)                          ,           // 片选信号
	.sclk			(sclk)                          ,           // 时钟信号输出
	.mosi			(mosi)                          ,           // 主设备输出从设备输入数据线
	.miso			(miso)                          ,           // 主设备输入从设备输出数据线
                    
	.clk_div_val	(50 - 1)                         ,           // 时钟分频值 1M
                            
	.re_ack			(re_ack)                          ,      	    // 读应答信号
                             
	.data_tx		(slave_rx + 8'b2)                          ,           // 输入数据
	.data_rx 		(slave_rx)                                      // 输出数据
);  
//**************************************// 模块结束         //**********************************//
endmodule
