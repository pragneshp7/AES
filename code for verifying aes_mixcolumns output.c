#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <time.h>
#include <string.h>

void aes_mix_columns(uint8_t *state)
{
    uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};
    uint8_t s[4];
    int i, j, r;
   
    for (i = 0; i < 4; i++) {
        for (r = 0; r < 4; r++) {
            s[r] = 0;
            for (j = 0; j < 4; j++) {
                s[r] = s[r] ^ ((((y[r * 4 + j] >> 0) & 1) * state[i * 4 + j]) ^ (((y[r * 4 + j] >> 1) & 1) * ((state[i * 4 + j] << 1) ^ (((state[i * 4 + j] >> 7) & 1) * 0x1b))) );
            printf ("s[%d] = s[%d] ^ ((((y[%d * 4 + %d] >> 0) & 1) * state[%d * 4 + %d]) ^ (((y[%d * 4 + %d] >> 1) & 1) * ((state[%d * 4 + %d] << 1) ^ (((state[%d * 4 + %d] >> 7) & 1) * 0x1b))) )\n", r, r, r, j, i, j,r,j,i,j,i,j);
                
            }
        }
        for (r = 0; r < 4; r++) {
            state[i * 4 + r] = s[r];
            printf("state[%d * 4 + %d] = s[%d]\n",i,r,r);
        }
    }
}
int main()
{
    uint8_t buf2[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
              0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    aes_mix_columns(buf2);
    return 0;
}

/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <time.h>
#include <string.h>

void aes_mix_columns(uint8_t *state)
{
    uint8_t y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2};
    uint8_t s[4];
    int i, j, r;
   
    for (i = 0; i < 4; i++) {
        for (r = 0; r < 4; r++) {
            s[r] = 0;
            for (j = 0; j < 4; j++) {
                s[r] = s[r] ^ ((((y[r * 4 + j] >> 0) & 1) * state[i * 4 + j]) ^ (((y[r * 4 + j] >> 1) & 1) * ((state[i * 4 + j] << 1) ^ (((state[i * 4 + j] >> 7) & 1) * 0x1b))) );
                //printf ("s[%d] = s[%d] ^ ((((y[%d * 4 + %d] >> 0) & 1) * state[%d * 4 + %d]) ^ (((y[%d * 4 + %d] >> 1) & 1) * ((state[%d * 4 + %d] << 1) ^ (((state[%d * 4 + %d] >> 7) & 1) * 0x1b))) )\n", r, r, r, j, i, j,r,j,i,j,i,j);
                //printf ("%d = %d ^ ((((%d >> 0) & 1) * %d) ^ (((%d >> 1) & 1) * ((%d << 1) ^ (((%d >> 7) & 1) * 0x1b))) )\n", s[r], s[r], y[r * 4 + j], state[i * 4 + j], y[r * 4 + j], state[i * 4 + j], state[i * 4 + j]);
                //printf ("s[%d] = %x, y[%d] = %x, state[%d] = %x \n",r,s[r], r * 4 + j, y[r * 4 + j], i * 4 + j, state[i * 4 + j] );
                //printf("s[%d] = %x\n",r,s[r]); 
            }
        }
        for (r = 0; r < 4; r++) {
            state[i * 4 + r] = s[r];
        //    printf("state[%d * 4 + %d] = s[%d]\n",i,r,r);
        }
    }
}
int main()
{
    uint8_t buf2[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
              0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    aes_mix_columns(buf2);
    for (int i = 0; i<16; i++){
        printf("%x ,", buf2[i]);
    }
    printf("\n");
    uint8_t buf3[16] = { 0x12, 0x34, 0x56, 0x78, 0x11, 0x22, 0x33, 0x44,
              0x13, 0x14, 0x15, 0x16, 0xab, 0xbc, 0xcd, 0xde };
    aes_mix_columns(buf3);
    for (int i = 0; i<16; i++){
        printf("%x ,", buf3[i]);
    }
    return 0;
}
