//********************************//
//! ģ��: spi_slave
//  ����: SPI����ģ�飬������������ź�
//********************************//
module spi_slave#(
	parameter 		CPOL = 1'b1						,			// ʱ�Ӽ��Կ���
					CPHA = 1'b1									// ʱ����λ����
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

//**************************************// ������д         //**********************************//
//? ״̬��
localparam					S0_IDLE			= 4'd0,				// ����
							S1_SCLK_IDLE	= 4'd1,				// sclkʱ�ӵȴ�
							S2_SCLK_EDGE	= 4'd2,				// sclkʱ�ӱ���
							S3_LAST_HALF	= 4'd3,				// ���յȴ�
							S4_ACK			= 4'd4,				// ����ack
							S5_FINISH		= 4'd5;				// ִ��1��spi�������

//**************************************// �ź�����         //**********************************//
reg 	[7 : 0] 			mosi_shift;							// mosi��λ�Ĵ���
reg		[7 : 0]				miso_shift;							// miso��λ�Ĵ���
reg		[3 : 0]				state;								// ״̬�Ĵ���
reg		[15 : 0]			cnt_clk;							// ʱ�Ӽ�����
reg		[4 : 0]				cnt_sclk_edge;						// sclk���ؼ�����

assign 		mosi			= mosi_shift[7];					// �������
assign 		data_rx 		= miso_shift;						// ��������
assign		wr_ack			= (state == S4_ACK);				// �����ֽ����ack

//? csͬ��
reg 		cs_pre;
always @(negedge sys_rst_n or posedge sys_clk)
	if (!sys_rst_n)	cs_pre = cs;
	else 			cs_pre = cs;
wire   		cs_pos;
wire  		cs_neg;
assign 		cs_pos = ~cs_pre & cs;
assign 		cs_neg = cs_pre & ~cs;	

//? csͬ��
reg 		cs_pre;
always @(negedge sys_rst_n or posedge sys_clk)
	if (!sys_rst_n)	cs_pre = cs;
	else 			cs_pre = cs;
wire   		cs_pos;
wire  		cs_neg;
assign 		cs_pos = ~cs_pre & cs;
assign 		cs_neg = cs_pre & ~cs;	
//**************************************// �������         //**********************************//
//? ״̬������
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		state	<= S0_IDLE;
	else case(state)
		S0_IDLE:	
			if (wr_req)
				state	<= S1_SCLK_IDLE;
			else 
				state	<= S0_IDLE;
		S1_SCLK_IDLE:	
			// �ȴ�ʱ��
			if (cnt_clk == clk_div_val)
				state	<= S2_SCLK_EDGE;
			else 
				state	<= S1_SCLK_IDLE;
		S2_SCLK_EDGE:	
			if (cnt_sclk_edge == 5'd15)
				state	<= S3_LAST_HALF;
			else 
				state	<= S1_SCLK_IDLE;
		S3_LAST_HALF:	
			if (cnt_clk == clk_div_val)
				state	<= S4_ACK;
			else 
				state	<= S3_LAST_HALF;
		S4_ACK:	
				state	<= S5_FINISH;
		S5_FINISH:	
				state	<= S0_IDLE;
	endcase
end

//? ʱ�Ӽ���
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		cnt_clk	<= 16'b0;
	else if (state == S1_SCLK_IDLE || state == S3_LAST_HALF)
		cnt_clk	<= cnt_clk + 16'b1;
	else
		cnt_clk	<= 16'b0;
end

//? sclk���ؼ���
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		cnt_sclk_edge	<= 5'b0;
	else if (state == S2_SCLK_EDGE)
		cnt_sclk_edge	<= cnt_sclk_edge + 5'b1;
	else if (state == S0_IDLE)
		cnt_sclk_edge	<= 5'b0;
	else
		cnt_sclk_edge	<= cnt_sclk_edge;
end

//? sclkʱ�� 
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		sclk	<= 1'b0;
	else if (state == S0_IDLE)
		sclk	<= CPOL;
	else if (state == S2_SCLK_EDGE)
		sclk	<= ~sclk;
	else
		sclk	<= sclk;
end

//? mosi��λ
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		mosi_shift	<= 8'b0;
	else if (state == S0_IDLE && wr_req)
		mosi_shift	<= data_tx;
	else if (CPHA == 1'b0 && state == S2_SCLK_EDGE && cnt_sclk_edge[0] == 1'b1)
		mosi_shift	<= {mosi_shift[6 : 0], mosi_shift[7]};
	else if (CPHA == 1'b1 && state == S2_SCLK_EDGE && (cnt_sclk_edge != 5'd0 && cnt_sclk_edge[0] == 1'b0))
		mosi_shift	<= {mosi_shift[6 : 0], mosi_shift[7]};
	else
		mosi_shift	<= mosi_shift;
end

//? miso��λ
always@(negedge sys_rst_n or posedge sys_clk) begin
	if (~sys_rst_n) 
		miso_shift	<= 8'b0;
	else if (state == S0_IDLE && wr_req)
		miso_shift	<= 8'b0;
	else if (CPHA == 1'b0 && state == S2_SCLK_EDGE && cnt_sclk_edge[0] == 1'b0)
		miso_shift	<= {miso_shift[6 : 0], miso};
	else if (CPHA == 1'b1 && state == S2_SCLK_EDGE && cnt_sclk_edge[0] == 1'b1)
		miso_shift	<= {miso_shift[6 : 0], miso};
	else
		miso_shift	<= miso_shift;
end

//**************************************// ��ģ�����       //**********************************//
//**************************************// ģ�����         //**********************************//
endmodule
