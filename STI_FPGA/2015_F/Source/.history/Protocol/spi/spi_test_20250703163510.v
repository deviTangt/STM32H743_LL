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



//**************************************// 子模块调用       //**********************************//

//**************************************// 模块结束         //**********************************//
endmodule
