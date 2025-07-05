//********************************//
//! ģ��: uart���ͽ��չ���ģ��
//  ����: ���ڷ���ʲô������ʲô+\r\n,����������������������
//********************************//
module uart_try#(
	parameter 		BAUD_RATE 	= 115_200
)
(
	input 	wire						sys_clk,
	input 	wire 						sys_rst_n,

	output 	reg 		[7 : 0]     	test_data,

	output 	wire		[7 : 0]			uart_rx_data,
	input 	wire						uart_rx_port,
	output 	wire 						uart_tx_port
);

//**************************************// ������д         //**********************************//

//**************************************// �ź�����         //**********************************//


//**************************************// �������         //**********************************//


//**************************************// ��ģ�����       //**********************************//
//? �ַ����շ�����ģ��
uart_string_handle#(
	.BAUD_RATE 	(115_200)
)
ins_uart_string_handle
(
	.sys_clk						(sys_clk),
	.sys_rst_n						(sys_rst_n),

    .tx_string						(),
    .tx_length						(),
    .tx_req							(),
    .tx_busy						(),
    .tx_done						(),

    .rx_string						(),
    .rx_length						(),
    .rx_busy						(),
    .rx_done						(),

	.uart_rx_port					(uart_rx_port),
	.uart_tx_port					(uart_tx_port)
);

//**************************************// ģ�����         //**********************************//
endmodule
