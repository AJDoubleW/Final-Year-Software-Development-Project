/*Encryption of the password using username - custom salted hash vigenere*/

#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <string>

class Encrypt
{
public:
    Encrypt();
    std::string enctext(char [], char []);
    
    
};


#endif /* ENCRYPT_HPP */

