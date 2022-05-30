AES, Advanced Encryption Standard, is actually a set of standards: FIPS 197 , and the AES algorithm we call is actually the Rijndael algorithm.

NIST (National INstitute of Standards and Technology) publicly solicited more efficient and secure alternative DES encryption algorithms on September 12, 1997. A total of 15 algorithms were selected in the first round, of which 5 algorithms were shortlisted for the finals, namely MARS, RC6 , Rijndael, Serpent and Twofish. After three years of verification, evaluation and public discussion, the Rijndael algorithm was finally selected.

# Rijndael algorithm 
The Rijndael algorithm was proposed by the Belgian scholars Joan Daemen and Vincent Rijmen, and the name of the algorithm is a combination of the names of the two authors. Rijndael's strengths are the combination of safety, performance, efficiency, achievability and flexibility.

In this project the algorithm was accelerated using OpenMP and a hardware accelerator (both HLS and Verilog were used to create different versions of the accelerator). On the Ultra96 board, a speedup of 135 was achieved using OpenMP threads interfaced with four instantiations of the hardware accelerator as compared to the software only implementation presented in https://github.com/matt-wu/AES. 

This project was submitted in partial fulfillment of the requirements for EE 382N-4 Advanced MCU Systems and SDS 374C Parallel Computing for Science and Engineering. For more detailed information about the implementation, please refer to the presentation here - https://drive.google.com/file/d/1_Y8oWi9hOzH1ATFfZ1l3dme8iAuYKKu_/view?usp=sharing. For more insight into the usage of OpenMP in the project, please refer to the report here - https://drive.google.com/file/d/1vX7OhnjW1jeV7P_NjXSispWVh3mrcbdD/view?usp=sharing.

# File structure 

In the AES/presentation folder several different implementations can be found with their source code, kernal and bit files (if required). The presentation contains a detailed description of each implementation. The verif files include a self checking testbench that is used to check the sanity of the implementation.

The AES/verilog_files folder contains the Verilog files of the hardware accelerator.

The AES/hls folder contains the HLS files required to generate the hardware accelerator


