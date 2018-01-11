#pragma once

#include <string>
#include "crypto/blowfish.hh"

class ASHE{
    static const unsigned int ASHE_MAX;/*n*/
    static const std::string key;
    static blowfish bf;
    uint64_t IV;
    long ciphertext;
public:
    ASHE(int iv);

    long get_ciphertext(){return ciphertext;}


    std::pair<long,uint64_t> encrypt(unsigned int plaintext);
    unsigned int decrypt(long ciphertext);
    uint64_t get_IV(){return IV;};

    static uint64_t Fi(uint64_t IV){return bf.encrypt(IV)%100000;}
    static uint64_t Fi_1(uint64_t IV){return bf.encrypt(IV-1)%100000;}
    static std::pair<long,std::vector<uint64_t>> sum(std::vector<ASHE>);
    static uint64_t decrypt_sum(std::pair<long,std::vector<uint64_t>>);
};