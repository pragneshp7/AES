module func_unit (
input [7:0] s,
input [7:0] state,
input [1:0] y,
output [7:0] state_out
);

wire [7:0] s_out;
assign state_out = s_out;  
assign s_out = s^(((y&1)*state)^(((y>>1)&1)*((state<<1)^(((state>>7)&1)*8'h1b))));
endmodule

module control (
input clk,
input reset,
input start_done,
output [3:0] s_num,
output [3:0] y_num,
output [3:0] input_state_num,
output done_signal
);

reg [3:0] counter_16;
reg [3:0] counter_s_num;
reg [1:0] counter_4; 
reg [1:0] counter_16_4;
reg [5:0] counter_64;
wire done;

assign y_num = counter_16;
assign s_num = counter_s_num;
assign input_state_num = (counter_16_4*4)+counter_4; //TO-DO
assign done = (counter_64 == 63) ? 1 : 0;
assign done_signal = done;

always @(posedge clk) begin //counter_16
if (!reset || start_done) begin 
	counter_16 <= 0;
	counter_16_4 <= 0;
end
else if (!done) begin 
	if (counter_16 == 15) begin
		counter_16 <= 0;
		counter_16_4 <= counter_16_4 + 1;
	end
	else begin
		counter_16 <= counter_16 + 1;
	end
end
end

always @(posedge clk) begin //counter_s_num
if (!reset || start_done) begin 
	counter_s_num <= 0;
end
else if (!done) begin 
	if ((counter_s_num == 3) || (counter_s_num == 7) || (counter_s_num == 11) || (counter_s_num == 15)) begin
		counter_s_num <= counter_s_num + 1;
	end
end
end

always @(posedge clk) begin //counter_4
if (!reset || start_done) begin 
	counter_4 <= 0;
end
else if (!done) begin 
	if (counter_16 == 3) begin
		counter_4 <= 0;
	end
	else begin
		counter_4 <= counter_4 + 1;
	end
end
end

always @(posedge clk) begin //counter_4
if (!reset || start_done) begin 
	counter_64 <= 0;
end
else if (counter_64 == 63) begin
	counter_64 <= counter_64;
end
else begin
	counter_64 <= counter_64 + 1;
end
end

endmodule

module singlePulse(CLK, D, SP);
     input CLK, D;
     output SP;
     reg Q;
  	 wire Qn;
     assign Qn = ~Q;
     assign SP = D & Qn;

     always @(posedge CLK) begin
         Q <= D;
     end
endmodule

module aes_mixcolumns (
input clk,
input reset,
input start_in,
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
wire [1:0] y_in;
wire [3:0] s_num, input_state_num, y_num;
wire start, done_signal; 
reg start_done;

always @(posedge clk) begin
if (!reset) begin
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

always @(posedge clk) begin
if (!reset || start) begin
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
	s[s_num] <= state_out; 
end
end

// uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};
assign state_out0 = {s[3],s[2],s[1],s[0]};
assign state_out1 = {s[7],s[6],s[5],s[4]};
assign state_out2 = {s[11],s[10],s[9],s[8]};
assign state_out3 = {s[15],s[14],s[13],s[12]};

assign state_in = input_state[input_state_num]; 
assign y_in = y[y_num]; 
assign sin = s[s_num]; 
assign done = done_signal;

always @(posedge clk) begin //start_done - should become 1 after one cycle of getting start signal
if (!reset) begin
	start_done <= 1'b0;
end
else if (start) begin
	start_done <= 1'b1;
end
else begin
	start_done <= 1'b0;
end
end

control dut_control(clk, reset, start_done, s_num, y_num, input_state_num, done_signal);
func_unit dut_func_unit(sin, state_in, y_in, state_out);
singlePulse dut_singlePulse (clk, start_in, start);

endmodule


