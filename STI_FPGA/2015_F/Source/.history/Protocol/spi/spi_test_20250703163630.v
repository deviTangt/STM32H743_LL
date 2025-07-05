//********************************//
//! ģ��: spi_test
//  ����: spi_master��spi_slave����ģ��
//********************************//
module spi_test(
    input  wire                     sys_clk         ,   //? ϵͳʱ��
    input  wire                     sys_rst_n           //? ϵͳ��λ������Ч
);

//**************************************// ������д         //**********************************//


//**************************************// �ź�����         //**********************************//


//**************************************// �������         //**********************************//



//**************************************// ��ģ�����       //**********************************//
module spi_master#(
	.CPOL = 1'b1						,			// ʱ�Ӽ��Կ���
	.CPHA = 1'b1									// ʱ����λ����
)
(	
	input  	wire                    sys_clk			,     		// ʱ���ź�
	input  	wire                    sys_rst_n		,   		// ��λ�ź�

	output 	wire                    cs				,          	// Ƭѡ�ź�
	output 	reg                    	sclk			,        	// ʱ���ź����
	output 	wire                    mosi			,        	// ���豸������豸����������
	input  	wire                    miso			,        	// ���豸������豸���������

	input  	wire                    cs_ctrl			,     		// Ƭѡ�����ź� cs = cs_ctrl
	input  	wire	[15:0]      	clk_div_val		,          	// ʱ�ӷ�Ƶֵ

	input  	wire                    wr_req			,      		// д�����ź�
	output 	wire                    wr_ack			,      		// дӦ���ź�

	input  	wire	[7:0]			data_tx			,           // ��������
	output 	wire	[7:0]       	data_rx 		          	// �������
);

module spi_slave#(
	.CPOL = 1'b1						,			// ʱ�Ӽ��Կ���
	.CPHA = 1'b1									// ʱ����λ����
)
(	
	input  	wire                    sys_clk			,     		// ʱ���ź�
	input  	wire                    sys_rst_n		,   		// ��λ�ź�

	input 	wire                    cs				,          	// Ƭѡ�ź�
	input 	wire                    sclk			,        	// ʱ���ź����
	input 	wire                    mosi			,        	// ���豸������豸����������
	output  wire                    miso			,        	// ���豸������豸���������

	input  	wire	[15:0]      	clk_div_val		,          	// ʱ�ӷ�Ƶֵ

	output 	wire                    re_ack			,      		// ��Ӧ���ź�

	input  	wire	[7:0]			data_tx			,           // ��������
	output 	wire	[7:0]       	data_rx 		          	// �������
);
//**************************************// ģ�����         //**********************************//
endmodule
