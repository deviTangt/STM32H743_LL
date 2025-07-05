//********************************//
//! ģ��: square_wave_freq
//  ����: ��������Ƶ�ʡ�ռ�ձ�ģ��
//********************************//
module square_wave_freq#(
    parameter       SYS_CLK_FREQ    = 50_000_000    ,
                    PLL_FREQ        = 200_000_000   
)    
(
    input  wire                     sys_clk         ,   //? ϵͳʱ��    50MHz
    input  wire                     sys_rst_n       ,   //? ϵͳ��λ������Ч

    input  wire                     pll_clk         ,   //? PLL��Ƶ�ź� 200MHz
    input  wire                     wave_in         ,   //  ���������ź�

    input  wire     [15 : 0]        N
);

//**************************************// ������д         //**********************************//

//**************************************// �ź�����         //**********************************//
//? ����ʱ������Զ��ʱ
reg     [31 : 0]    cnt_main;       // ����ʱ��

reg     [31 : 0]    cnt_start;      // ��¼��ʼʱ���
reg     [31 : 0]    period_total;   // ��¼��ʱ���
reg     [15 : 0]    cnt_cycle;      // ��¼ʱ����ڴ��Ⲩ�����ظ���

//? ����D��������������̬
reg                 wave_sync;
reg                 wave_dly;
wire                wave_in_pos;
always @(posedge pll_clk or posedge sys_rst_n) begin
    if (!sys_rst_n) 
        {wave_sync, wave_dly} <= 2'b00;
    else 
        {wave_sync, wave_dly} <= {wave_in, wave_sync};
end
assign wave_in_pos = ~wave_dly & wave_sync;  // �����ؼ��

//**************************************// �������         //**********************************//


//? ����ʱ������ ÿ5ns�Զ�+1 ��32λ�ɼ���21.47�룩
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_main <= 32'b0;
    end else begin
        cnt_main <= cnt_main + 32'b1;
    end
end

//? �����ź������ؼ��� 
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_cycle   <= 16'd0;
    end else begin
        
    end
end

//? ���������߼�
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_start       <= 32'b0;
        period_total    <= 32'b0;
    end else begin  // ��⵽����������
        if (cnt_cycle == 0) 
            cnt_start <= cnt_main;  // ��¼��ʼʱ���
        if (cnt_cycle == N) begin   // �ﵽԤ��������
            period_total =  (cnt_main >= cnt_start) ? 
                            (cnt_main - cnt_start) : 
                            (32'hFFFF_FFFF - cnt_start + cnt_main + 32'b1);  // ������ʱ���
        end
    end
end

//**************************************// ��ģ�����       //**********************************//

//**************************************// ģ�����         //**********************************//
endmodule
