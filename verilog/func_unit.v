module func_unit (
input [7:0] s,
input [7:0] state,
input [1:0] y,
output [7:0] state_out
);


assign s_out = s ^ (((y & 1) * state) ^ (((y >> 1) & 1) * ((state << 1) ^ (((state >> 7) & 1) * 0x1b))) )
endmodule

module top_aes_mixcolumns (
input clk,
input reset,
input start,
input [31:0] state0,
input [31:0] state1,
input [31:0] state2,
input [31:0] state3,
output [31:0] state_out0,
output [31:0] state_out1,
output [31:0] state_out2,
output [31:0] state_out3, 
output done
);

reg [1:0] y [15:0];
reg [7:0] s [15:0];
reg [7:0] input_state [15:0];
wire [7:0] sin;
wire [7:0] state_in;
wire [7:0] state_out;
wire [1:0] yin;

always(posedge clk) begin
if (reset) begin
	input_state[0] <= 8'b0;
	input_state[1] <= 8'b0;
	input_state[2] <= 8'b0;
	input_state[3] <= 8'b0;	
	input_state[4] <= 8'b0;
	input_state[5] <= 8'b0;
	input_state[6] <= 8'b0;
	input_state[7] <= 8'b0;	
	input_state[8] <= 8'b0;
	input_state[9] <= 8'b0;
	input_state[10] <= 8'b0;
	input_state[11] <= 8'b0;	
	input_state[12] <= 8'b0;
	input_state[13] <= 8'b0;
	input_state[14] <= 8'b0;
	input_state[15] <= 8'b0;		
end
else if (start) begin
	input_state[0] <= state0[7:0];
	input_state[1] <= state0[15:8];
	input_state[2] <= state0[23:16];
	input_state[3] <= state0[31:24];	
	input_state[4] <= state1[7:0];
	input_state[5] <= state1[15:8];
	input_state[6] <= state1[23:16];
	input_state[7] <= state1[31:24];	
	input_state[8] <= state2[7:0];
	input_state[9] <= state2[15:8];
	input_state[10] <= state2[23:16];
	input_state[11] <= state2[31:24];	
	input_state[12] <= state3[7:0];
	input_state[13] <= state3[15:8];
	input_state[14] <= state3[23:16];
	input_state[15] <= state3[31:24];
end
end

initial begin
y[0] = 2'd2;
y[1] = 2'd3;
y[2] = 2'd1;
y[3] = 2'd1;
y[4] = 2'd1;
y[5] = 2'd2;
y[6] = 2'd3;
y[7] = 2'd1;
y[8] = 2'd1;
y[9] = 2'd1;
y[10] = 2'd2;
y[11] = 2'd3;
y[12] = 2'd3;
y[13] = 2'd1;
y[14] = 2'd1;
y[15] = 2'd2;
end

always(posedge clk) begin
if (reset || start) begin
	s[0] <= 8'b0;
	s[1] <= 8'b0;
	s[2] <= 8'b0;
	s[3] <= 8'b0;	
	s[4] <= 8'b0;
	s[5] <= 8'b0;
	s[6] <= 8'b0;
	s[7] <= 8'b0;	
	s[8] <= 8'b0;
	s[9] <= 8'b0;
	s[10] <= 8'b0;
	s[11] <= 8'b0;	
	s[12] <= 8'b0;
	s[13] <= 8'b0;
	s[14] <= 8'b0;
	s[15] <= 8'b0;	
end
else begin
	s[xyz] <= state_out; //TO-DO same as assign sin
end
end

// uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};
assign state_out0 = {s[3],s[2],s[1],s[0]};
assign state_out1 = {s[7],s[6],s[5],s[4]};
assign state_out2 = {s[11],s[10],s[9],s[8]};
assign state_out3 = {s[15],s[14],s[13],s[12]};

assign state_in = input_state[xyz]; //TO-DO
assign y_in = y[xyz]; //TO-DO
assign sin = s[xyz]; //TO-DO



control dut_control();
func_unit dut_func_unit(sin, state_in, y_in, state_out);


endmodule