int dh;
uint32_t* ps_reg;
uint32_t* pl_reg;
uint32_t* acc_virtual_address;

void dm_call(unsigned int target_addr, unsigned int offset, int mem_pll, uint32_t *clk_reg){
	uint32_t *pm0;
	pm0 = clk_reg + (((target_addr + offset) & MAP_MASK)>>2);
}

int dm_call_ret(unsigned int target_addr, unsigned int offset, int mem_pll, uint32_t *clk_reg){
	//mem_pll - 1 if memory and 0 if pll
	uint32_t *pm0;
	int ret_val;
	pm0 = clk_reg + (((target_addr + offset) & MAP_MASK)>>2);
	ret_val = *pm0;
	return ret_val;
}

void pm_call(unsigned int target_addr, unsigned int offset, unsigned int value, int mem_pll, uint32_t *clk_reg){
	uint32_t *pm0;
	pm0 = clk_reg + (((target_addr + offset) & MAP_MASK)>>2);
	*pm0 = value;
}

void change_ps_freq(int scenario){
	//printf("Scenario is %d\n",scenario);
	unsigned int val;
	switch(scenario){
		case 0:
			printf("\t Setting PS Freq to 1499MHz\n");
			val = (0<<16)|(45<<8);
			pm_call(0xfd1a0000,0x20,val,0,ps_reg);
			val = (63<<25)|(825<<13)|(3<<10)|(3<<5)|(12<<0);
			pm_call(0xfd1a0000,0x24,val,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00002d08,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00002d09,0,ps_reg);
			dm_call(0xfd1a0000,0x44,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00002d08,0,ps_reg);
			do{
				//printf("\t\t\tLOCK_CALL = 0x%.8x\n",dm_call_ret(0xfd1a0000,0x44,0));
				//printf("\t\t\tWaiting for lock\n");
			} while ((dm_call_ret(0xfd1a0000,0x44,0,ps_reg)&(0x1))!=1);
			pm_call(0xfd1a0000,0x20,0x00002d00,0,ps_reg);
			//printf("\tPS Freq setting to 1499MHz done\n");
			break;
		case 1:
			printf("\t Setting PS Freq to 999MHz\n");
			val = (0<<16)|(30<<8);
			pm_call(0xfd1a0000,0x20,val,0,ps_reg);
			val = (63<<25)|(1000<<13)|(3<<10)|(4<<5)|(6<<0);
			pm_call(0xfd1a0000,0x24,val,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00001e08,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00001e09,0,ps_reg);
			dm_call(0xfd1a0000,0x44,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00001e08,0,ps_reg);
			do{
				//printf("\t\t\tLOCK_CALL = 0x%.8x\n",dm_call_ret(0xfd1a0000,0x44,0));
				//printf("\t\t\tWaiting for lock\n");
			} while ((dm_call_ret(0xfd1a0000,0x44,0,ps_reg)&(0x1))!=1);
			pm_call(0xfd1a0000,0x20,0x00001e00,0,ps_reg);
			//printf("\tPS Freq setting to 999MHz done\n");
			break;
		case 2:
			printf("\t Setting PS Freq to 416.6MHz\n");
			val = (1<<16)|(25<<8);
			pm_call(0xfd1a0000,0x20,val,0,ps_reg);
			val = (63<<25)|(1000<<13)|(3<<10)|(3<<5)|(10<<0);
			pm_call(0xfd1a0000,0x24,val,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00011908,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00011909,0,ps_reg);
			dm_call(0xfd1a0000,0x44,0,ps_reg);
			pm_call(0xfd1a0000,0x20,0x00011908,0,ps_reg);
			do{
				//printf("\t\t\tLOCK_CALL = 0x%.8x\n",dm_call_ret(0xfd1a0000,0x44,0));
				//printf("\t\t\tWaiting for lock\n");
			} while ((dm_call_ret(0xfd1a0000,0x44,0,ps_reg)&(0x1))!=1);
			pm_call(0xfd1a0000,0x20,0x00011900,0,ps_reg);
			//printf("\tPS Freq setting to 416.6MHz done\n");
			break;
		default:
			printf("ERROR, not a valid scenario\n");
	}		
}

void change_pl_freq(int scenario){
	uint32_t *pl0;
	pl0 = pl_reg + (((0xFF5E0000 + 0xC0) & PL_MASK)>>2);
	//printf("PL Address = 0x%.8x\n", *clk_reg);//TODO print address
	
	switch(scenario) {
		case 0:
			printf("\t Setting PL Freq to 300MHz\n");
			*pl0 = (1<<24)		//Bit 24 - clock enable
					| (1<<16) 	//Bit 23:16 - divisor 1
					| (5<<8);	//Bit 15:0 - divisor 0
			//printf("\t\tPL Reg Value = 0x%.8x\n", *pl0);
			//printf("\tPL Freq setting to 300MHz done\n");
			break;
		case 1:
			printf("\t Setting PL Freq to 187.5MHz\n");
			*pl0 = (1<<24)		//Bit 24 - clock enable
					| (1<<16) 	//Bit 23:16 - divisor 1
					| (8<<8);	//Bit 15:0 - divisor 0
			//printf("\t\tPL Reg Value = 0x%.8x\n", *pl0);
			//printf("\tPL Freq setting to 187.5MHz done\n");
			break;
		case 2:
			printf("\t Setting PL Freq to 100MHz\n");
			*pl0 = (1<<24)		//Bit 24 - clock enable
					| (1<<16) 	//Bit 23:16 - divisor 1
					| (15<<8);	//Bit 15:0 - divisor 0
			//printf("\t\tPL Reg Value = 0x%.8x\n", *pl0);
			//printf("\tPL Freq setting to 100MHz done\n");
			break;
		default:
			printf("ERROR, not a valid scenario\n");
	}
		
	//munmap(clk_reg,0x1000);	
	//munmap(clk_reg,0x1000);
}
