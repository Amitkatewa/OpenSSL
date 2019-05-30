# OpenSSL
Project Title

OPENSSL LIBRARY ACCELERATION USING GPU FOR FASTER CRYPTOGRAPHY

Prerequisites

HARDWARE RESOURCES REQUIRED
Sr. No. Parameter Minimum Requirement Justification
1 CPU Speed 2.2GHz For faster processing
2 RAM 4 GB Can be Expanded
3 Graphics Card version Nvidia 720M Can be Expanded
4 Graphics Card 2 GB Can be Expanded

SOFTWARE RESOURCES REQUIRED
1. Operating System:Ubuntu 16.04 LTS or higher version
2. Programming Language:-Cuda
3. Software tool:- CUDA 9.0 toolkit
4. Cloud Server

Installing
Cuda Installation :-Cuda Installation
OpenSSL Installation:- OpenSSL installation
Running the tests



1:Cuda Codes
nvcc AES.cu


g++ encrypt.cpp
g++ decrypt.cpp

OPENSLL CODES:-
md5sum ssldecrypt.txt 
openssl 	aes-128-cbc -a -e -in F4.txt -out sslencrypt.txt
openssl aes-128-cbc -a -d -in sslencrypt.txt -out ssldecrypt.txt

./a.out key.txt F4.txt encrypt.txt decrypt.txt
