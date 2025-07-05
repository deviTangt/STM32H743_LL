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
    input  wire     [1 : 0]         wave_in         ,   //  ���������ź�

    input  wire     [15 : 0]        N
);

//**************************************// ������д         //**********************************//

//**************************************// �ź�����         //**********************************//
//? ����ʱ������Զ��ʱ
reg     [31 : 0]    cnt_main; 

reg     [31 : 0]    cnt_start;
reg     [31 : 0]    cnt_end;
reg     [31 : 0]    period_total;
reg     [31 : 0]    cnt_cycle;

//**************************************// �������         //**********************************//
//? ����ʱ������ ÿ5ns�Զ�+1 ��32λ�ɼ���21.47�룩
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_main <= 32'b0;
    end else begin
        cnt_main <= cnt_main + 32'b1;
    end
end

//? ���������߼�
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_start       <= 32'b0;
        cnt_end         <= 32'b0;
        period_total    <= 32'b0;
        cnt_cycle       <= 32'b0;
    end else begin  // ��⵽����������
        if (cnt_cycle == 0) 
            cnt_start <= cnt_main;  // ��¼��ʼʱ���
        if (cnt_cycle == N) begin   // �ﵽԤ��������
            cnt_end <= cnt_main;    // ��¼����ʱ���
            period_total = (cnt_main >= start_cnt) ? 
                     (cnt_end - start_cnt) : 
                     (32'hFFFF_FFFF - start_cnt + cnt_end + 1);  // ������ʱ���
        end
    end
end

//**************************************// ��ģ�����       //**********************************//

//**************************************// ģ�����         //**********************************//
endmodule
