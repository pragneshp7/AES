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

// uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};

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

wire [7:0] sin;
wire [7:0] state_in;
wire [7:0] state_out;
wire [1:0] yin;


control dut_control();
func_unit dut_func_unit(sin, state_in, y_in, state_out);


endmodule