
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "structures.h"
#include <sys/stat.h>
#include <ctime>
using namespace std;
 


void AddRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

void SubBytes(unsigned char * state) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[state[i]];
	}
}


void ShiftRows(unsigned char * state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];
	
	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];
	
	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

 
void MixColumns(unsigned char * state) {
	unsigned char tmp[16];

	tmp[0] = (unsigned char) mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	tmp[1] = (unsigned char) state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	tmp[2] = (unsigned char) state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	tmp[3] = (unsigned char) mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];

	tmp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	tmp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	tmp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	tmp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];

	tmp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	tmp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	tmp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	tmp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];

	tmp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	tmp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	tmp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	tmp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}


void Round(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, key);
}


void FinalRound(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, key);
}


void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
	unsigned char state[16]; // Stores the first 16 bytes of original message

	for (int i = 0; i < 16; i++) {
		state[i] = *(message+i);
	}

	int numberOfRounds = 9;

	AddRoundKey(state, expandedKey); // Initial round

	for (int i = 0; i < numberOfRounds; i++) {
		Round(state, expandedKey + (16 * (i+1)));
	}

	FinalRound(state, expandedKey + 160);

	// Copy encrypted state to buffer
	for (int i = 0; i < 16; i++) {
		*(encryptedMessage+i) = state[i];
	}
}


int main() {

	cout << "=============================" << endl;
	cout << " 128-bit AES Encryption   " << endl;
	cout << "=============================" << endl;
	clock_t begin,end;
        double time_spent;
	begin=clock();
       struct stat sb;
       stat("F4.txt", &sb); 
       FILE *fp=fopen("F4.txt", "r"); 
       char * message = new char[sb.st_size+1];
            
       fread(message, 1, sb.st_size, fp);
       fclose(fp);
   
	// Pad message to 16 bytes
	long originalLen = sb.st_size;
       
	long paddedMessageLen = originalLen;
	
	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
	}

	unsigned char *paddedMessage = new unsigned char [paddedMessageLen];
	for (long i = 0; i < paddedMessageLen; i++) {
		if (i >= originalLen) {
			paddedMessage[i] = 0;
		}
		else {
			paddedMessage[i] = message[i];
		}
	}

	unsigned char * encryptedMessage =  new unsigned char[paddedMessageLen];

	string str;
	ifstream infile;
	infile.open("keyfile", ios::in | ios::binary);

	if (infile.is_open())
	{
		getline(infile, str); 
		infile.close();
	}

//	else cout << "Unable to open file";

	istringstream hex_chars_stream(str);
	unsigned char key[16];
	long i = 0;
	unsigned long c;
	while (hex_chars_stream >> hex >> c)
	{
		key[i] = c;
		i++;
	}

	unsigned char expandedKey[176];

	KeyExpansion(key, expandedKey);

	for (long i = 0; i < paddedMessageLen; i += 16) {
		AESEncrypt(paddedMessage+i, expandedKey, encryptedMessage+i);
	}




	ofstream outfile;
	outfile.open("encrypt.txt", ios::out );
	if (outfile.is_open())
	{
		for (long i = 0; i < paddedMessageLen; i++){
		outfile <<hex<<encryptedMessage[i];
		
		}
		outfile.close();

	}
	 



	// Free memory
	delete[] paddedMessage;
	delete[] encryptedMessage;
	end = clock();
	time_spent=(double)(end-begin)/CLOCKS_PER_SEC;
         cout << "Encryption time: " <<time_spent << endl; 
	return 0;
}
