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

	output 	wire		[7 : 0]			uart_rx_data,
	input 	wire						uart_rx_port,
	output 	wire 						uart_tx_port
);

//**************************************// ������д         //**********************************//
reg         [1023 : 0]      string_tx;
reg         [7 : 0]         string_tx_length;
reg                         string_tx_req;
wire                        string_tx_busy;
wire                        string_tx_done;

wire        [1023 : 0]      string_rx;
wire        [7 : 0]         string_rx_length;
wire                        string_rx_busy;
wire                        string_rx_done;

assign  string_tx = string_rx;
//**************************************// �ź�����         //**********************************//
always @(negedge sys_rst_n or posedge sys_clk) begin
    
end

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

    .tx_string						(string_tx),
    .tx_length						(string_tx_length),
    .tx_req							(string_tx_req),
    .tx_busy						(string_tx_busy),
    .tx_done						(string_tx_done),

    .rx_string						(string_rx),
    .rx_length						(string_rx_length),
    .rx_busy						(string_rx_busy),
    .rx_done						(string_rx_done),

	.uart_rx_port					(uart_rx_port),
	.uart_tx_port					(uart_tx_port)
);

//**************************************// ģ�����         //**********************************//
endmodule
