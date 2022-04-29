#include "hls_mixcolumn.h"
void aes_mix_columns(uint8_t state[16], uint8_t state_out[16])
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS RESOURCE variable=state_out core=ROM_1P_BRAM
#pragma HLS INTERFACE s_axilite port=state_out
#pragma HLS RESOURCE variable=state core=ROM_1P_BRAM
#pragma HLS INTERFACE s_axilite port=state

    uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};
    uint8_t s[4];
    int i, j, r;
   
    L1: for (i = 0; i < 4; i++) {
        L2 :for (r = 0; r < 4; r++) {
            s[r] = 0;
            L3: for (j = 0; j < 4; j++) {
                s[r] = s[r] ^ ((((y[r * 4 + j] >> 0) & 1) * state[i * 4 + j]) ^ (((y[r * 4 + j] >> 1) & 1) * ((state[i * 4 + j] << 1) ^ (((state[i * 4 + j] >> 7) & 1) * 0x1b))) );
            }
        }
        L4: for (r = 0; r < 4; r++) {
            state_out[i * 4 + r] = s[r];
        }
    }
}
