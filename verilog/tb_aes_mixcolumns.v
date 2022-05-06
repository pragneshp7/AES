module tb_aes_mixcolumns(
);

reg tb_clk;
reg tb_reset;
reg tb_start_in;
reg [31:0] tb_state0;
reg [31:0] tb_state1;
reg [31:0] tb_state2;
reg [31:0] tb_state3;
wire [31:0] tb_state_out0;
wire [31:0] tb_state_out1;
wire [31:0] tb_state_out2;
wire [31:0] tb_state_out3; 
wire tb_done;

aes_mixcolumns dut_aes_mixcolumns (tb_clk, tb_reset, tb_start_in, tb_state0, tb_state1, tb_state2, tb_state3, tb_state_out0, tb_state_out1, tb_state_out2, tb_state_out3, tb_done);

initial #1000 $finish;

initial begin
  // Initialize Inputs
  tb_clk = 0;
  tb_reset = 0;
  #18;
  tb_reset = 1;
  #10
  tb_state0 = 32'h33221100;
  tb_state1 = 32'h77665544;
  tb_state2 = 32'hbbaa9988;
  tb_state3 = 32'hffeeddcc;
  #10
  tb_start_in = 1'b1;
end

initial begin
	forever #5 tb_clk = ~tb_clk;
end

initial begin  
	$dumpfile("dump.vcd");
	$dumpvars;
end

endmodule