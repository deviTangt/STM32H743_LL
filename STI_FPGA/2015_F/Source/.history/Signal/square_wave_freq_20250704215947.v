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
    input  wire     [1 : 0]         wave_in         ,   //  波形输入信号

    input  wire     [15 : 0]        N
);

//**************************************// 参数列写         //**********************************//

//**************************************// 信号声明         //**********************************//
//? 主计时器，永远计时
reg     [31 : 0]    cnt_main; 

reg     [31 : 0]    cnt_start;
reg     [31 : 0]    cnt_end;
reg     [31 : 0]    period_total;
reg     [31 : 0]    cnt_cycle;

//**************************************// 主程序段         //**********************************//
//? 主计时器计数 每5ns自动+1 （32位可计数21.47秒）
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_main <= 32'b0;
    end else begin
        cnt_main <= cnt_main + 32'b1;
    end
end

//? 测量控制逻辑
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_start       <= 32'b0;
        cnt_end         <= 32'b0;
        period_total    <= 32'b0;
        cnt_cycle       <= 32'b0;
    end else begin  // 检测到方波上升沿
        if (cnt_cycle == 0) 
            cnt_start <= cnt_main;  // 记录起始时间戳
        if (cnt_cycle == N) begin   // 达到预设周期数
            cnt_end <= cnt_main;    // 记录结束时间戳
            period_total = (cnt_main >= start_cnt) ? 
                     (cnt_end - start_cnt) : 
                     (32'hFFFF_FFFF - start_cnt + cnt_end + 1);  // 计算总时间差
        end
    end
end

//**************************************// 子模块调用       //**********************************//

//**************************************// 模块结束         //**********************************//
endmodule
