//********************************//
//! 模块: square_wave_freq
//  功能: 测量方波频率、占空比模块
//********************************//
module square_wave_freq#(
    parameter       SYS_CLK_FREQ    = 50_000_000    ,
                    PLL_FREQ        = 200_000_000   
)    
(
    input  wire                     sys_clk         ,   //? 系统时钟    50MHz
    input  wire                     sys_rst_n       ,   //? 系统复位，低有效

    input  wire                     pll_clk         ,   //? PLL倍频信号 200MHz
    input  wire                     wave_in         ,   //  波形输入信号

    input  wire     [15 : 0]        N
);

//**************************************// 参数列写         //**********************************//

//**************************************// 信号声明         //**********************************//
//? 主计时器，永远计时
reg     [31 : 0]    cnt_main;       // 主计时器

reg     [31 : 0]    cnt_start;      // 记录起始时间戳
reg     [31 : 0]    period_total;   // 记录总时间差
reg     [15 : 0]    cnt_cycle;      // 记录时间差内待测波上升沿个数

//? 两级D触发器消除亚稳态
reg                 wave_sync;
reg                 wave_dly;
wire                wave_in_pos;
always @(posedge pll_clk or posedge sys_rst_n) begin
    if (!sys_rst_n) 
        {wave_sync, wave_dly} <= 2'b00;
    else 
        {wave_sync, wave_dly} <= {wave_in, wave_sync};
end
assign wave_in_pos = ~wave_dly & wave_sync;  // 上升沿检测

//**************************************// 主程序段         //**********************************//


//? 主计时器计数 每5ns自动+1 （32位可计数21.47秒）
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_main <= 32'b0;
    end else begin
        cnt_main <= cnt_main + 32'b1;
    end
end

//? 输入信号上升沿计数 
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_cycle   <= 16'd0;
    end else begin
        
    end
end

//? 测量控制逻辑
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_start       <= 32'b0;
        period_total    <= 32'b0;
    end else begin  // 检测到方波上升沿
        if (cnt_cycle == 0) 
            cnt_start <= cnt_main;  // 记录起始时间戳
        if (cnt_cycle == N) begin   // 达到预设周期数
            period_total =  (cnt_main >= cnt_start) ? 
                            (cnt_main - cnt_start) : 
                            (32'hFFFF_FFFF - cnt_start + cnt_main + 32'b1);  // 计算总时间差
        end
    end
end

//**************************************// 子模块调用       //**********************************//

//**************************************// 模块结束         //**********************************//
endmodule
