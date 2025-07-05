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
    input  wire     [1 : 0]         wave_in             //  ���������ź�
);

//**************************************// ������д         //**********************************//

//**************************************// �ź�����         //**********************************//
//? ����ʱ������Զ��ʱ
reg     [31 : 0]    cnt_main;

//**************************************// �������         //**********************************//
//? ����ʱ������
always @(posedge pll_clk or negedge sys_rst_n) begin
    if (!sys_rst_n) begin
        cnt_main <= 32'b0;
    end else begin
        cnt_main <= cnt_main + 32'b1;
    end
end

// ���������߼�
always @(posedge clk_200m) begin
    if (pos_edge) begin  // ��⵽����������
        if (cycle_cnt == 0) 
            start_cnt <= main_cnt;  // ��¼��ʼʱ���
        
        if (cycle_cnt == N) begin   // �ﵽԤ��������
            end_cnt <= main_cnt;    // ��¼����ʱ���
            period_total <= end_cnt - start_cnt;  // ������ʱ���
        end
    end
end

//**************************************// ��ģ�����       //**********************************//

//**************************************// ģ�����         //**********************************//
endmodule
