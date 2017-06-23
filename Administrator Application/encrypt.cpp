/*Encryption method - custom salted hash vigenere*/

#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include "sha1.hpp"
#include "encrypt.hpp"

using namespace std;

Encrypt::Encrypt() {
    
}

string Encrypt::enctext (char in_plain[], char in_key[]) {
    SHA1 hash;
    char *plaintext;
    char *key;
    char encryptedtext[40];
    int j;
    int k;
    int increment;
    
    const string prehashplain = in_plain;
    hash.update(prehashplain);
    const string posthashplain = hash.final();
    plaintext = new char[posthashplain.length()+1];
    strcpy(plaintext, posthashplain.c_str());
    
    const string prehashkey = in_key;
    hash.update(prehashkey);
    const string posthashkey = hash.final();
    key = new char[posthashkey.length() + 1];
    strcpy(key, posthashkey.c_str());
    
    for (int i=0; i < 40; i++) {
        if ((int)plaintext[i] + (int)key[i] == 200) {
            encryptedtext[i] = (char)57;
        } else if ((int)plaintext[i] + (int)key[i] == 201) {
            encryptedtext[i] = (char)56;
        } else if ((int)plaintext[i] + (int)key[i] == 202) {
            encryptedtext[i] = (char)55;
        } else if ((int)plaintext[i] + (int)key[i] == 203) {
            encryptedtext[i] = (char)54;
        } else if ((int)plaintext[i] + (int)key[i] == 204) {
            encryptedtext[i] = (char)53;
        } else {

            if ((int)plaintext[i] <= 57) {
                j = (int)plaintext[i]-48;
            } else {
                j = (int)plaintext[i] - 87;
            }
            if ((int)key[i] <= 57) {
                k = (int)key[i] - 48;
            } else {
                k = (int)key[i] - 87;
            }

            increment = j+k;
            encryptedtext[i] = (char)(122-increment);
        }
    }
    
    const string thirdpass = encryptedtext;
    hash.update(thirdpass);
    const string finalstring = hash.final();
    
    return finalstring;
	

    
    
}
