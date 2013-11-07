
/*******************************************************************/
/* This is an example of the multithreaded uspport for OpenSSL EVP */
/*                             ciphers                             */
/*                                                                 */
/*                     Written by Joshua Miller                    */
/*                               2013                              */
/*******************************************************************/

#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "crypto.h"

#define BUFF_SIZE 67108864

int main(int argc, char*argv[])
{

    int crypto_buf_len = BUFF_SIZE / N_CRYPTO_THREADS;
    int crypto_cursor;
    char* data;

    // ----------- [ Initialize crypto
    unsigned char* password = (unsigned char*) "12345";
    char* cipher = (char*) "aes-128";
    crypto enc(EVP_ENCRYPT, PASSPHRASE_SIZE, password, cipher);
    crypto dec(EVP_DECRYPT, PASSPHRASE_SIZE, password, cipher);


    // ----------- [ Create sample data
    if (!(data = (char*) malloc(BUFF_SIZE*sizeof(char)))){
	fprintf(stderr, "Unable to allocate sample data\n");
	exit(1);
    }

    for (int i = 0; i < BUFF_SIZE; i++){
	data[i] = (char) (i % 255);
    }

    int n_rounds = 10;

    // ----------- [ Run encryption
    for (int j = 0; j < n_rounds; j++){
	crypto_cursor = 0;
	for (int i = 0; i < N_CRYPTO_THREADS; i ++){
	    pass_to_enc_thread(data+crypto_cursor, data+crypto_cursor, crypto_buf_len, &enc); 
	    crypto_cursor += crypto_buf_len;
	
	}
	join_all_encryption_threads(&enc);

	if (j == 0){
	    printf("\n\nEncrypted buffer:\n");
	    for (int i = 0; i < 128; i++)
		printf("%d ", data[i]);
	}

	// ----------- [ Run decryption
	crypto_cursor = 0;
	for (int i = 0; i < N_CRYPTO_THREADS; i ++){
	    pass_to_enc_thread(data+crypto_cursor, data+crypto_cursor, crypto_buf_len, &dec); 
	    crypto_cursor += crypto_buf_len;
	    
	}
	join_all_encryption_threads(&dec);

    }


    printf("\n\nDecrypted buffer:\n");
    for (int i = 0; i < 128; i++)
	printf("%d ", data[i]);

    
    return 0;
    
}
