module divider #(
	parameter 	freq_in 		= 50_000_000,
					freq_out 	= 100
)(
	input wire 			clk_in,
	input wire 			rst_n,
	
	output reg 			clk_out
);

reg [31 : 0] cnt_div;
	
always@(negedge rst_n or posedge clk_in) begin
	if (~rst_n) begin
		cnt_div	<= 32'b0;
		clk_out	<= 1'b0;
	end else if (cnt_div == freq_in / (2 * freq_out) - 1) begin
		cnt_div	<= 32'b0;
		clk_out 	<= ~clk_out;
	end else begin
		cnt_div	<= cnt_div + 32'b1;
		clk_out 	<= clk_out;
	end
end
	
endmodule
