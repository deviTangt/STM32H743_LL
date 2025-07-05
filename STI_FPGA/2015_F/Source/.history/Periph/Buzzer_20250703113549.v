//********************************//
//! ģ��: 
//  ����: 
//********************************//
module adc(
	input  wire                     sys_clk         ,   //? ϵͳʱ��
	input  wire                     sys_rst_n           //? ϵͳ��λ������Ч
);
//**************************************// ������д         //**********************************//

parameter               device_id       = 7'b1010_100,
						reg_addr        = 8'b0000_0000;

//**************************************// �ź�����         //**********************************//

reg     [7 : 0]         adc_data_temp;

//**************************************// �������         //**********************************//



//**************************************// ��ģ�����       //**********************************//

//**************************************// ģ�����         //**********************************//
endmodule

module Buzzer(
	input wire 				sys_clk,
	input wire 				sys_rst_n,
	
	input wire [3 : 0] 	sta_key,
	input wire [3 : 0] 	sta_key_pos,
	
	output reg				bepeer
);

wire 	clk_500;
wire 	clk_1000;
wire 	clk_2000;
wire 	clk_3000;
divider #(50_000_000, 500) 				d1(sys_clk, sys_rst_n, clk_500);
divider #(50_000_000, 1000) 				d2(sys_clk, sys_rst_n, clk_1000);
divider #(50_000_000, 2000) 				d3(sys_clk, sys_rst_n, clk_2000);
divider #(50_000_000, 3000) 				d4(sys_clk, sys_rst_n, clk_3000);

reg clk_sel;
always@(sta_key)begin
	case(sta_key)
		4'b0001: clk_sel <= clk_500;
		4'b0010: clk_sel <= clk_1000;
		4'b0100: clk_sel <= clk_2000;
		4'b1000: clk_sel <= clk_3000;
		default: clk_sel <= 1'b0;
	endcase
end

always@(negedge sys_rst_n or posedge clk_sel)
	if (~sys_rst_n)
		bepeer	<= 1'b0;
	else
		bepeer	<= ~bepeer;

//*********************************************// end	
endmodule
