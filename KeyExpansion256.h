#pragma once

#include "Const.h"
#include "Bitset_Typedef.h"



const int Nr = 14;  //AES-128 requires 10 rounds of encryption  
//aes258버전에서는 14라운드가 필요

const int Nk = 8;   //Nk Represents the number of word s that are input keys  


//Convert four byte s to one word.
word Word(byte& k1, byte& k2, byte& k3, byte& k4);

/**
 *  Cyclic left shift by byte
 *  That is to say, [a0, a1, a2, a3] becomes [a1, a2, a3, a0]
 */
word RotWord(word& rw);

/**
 *  S-box transformation for each byte in input word
 */
word SubWord(word sw);

/**
 *  Key Extension Function - Extended 128-bit key to w[4*(Nr+1)]
 */
 //This function makes (4 * (Nr + 1)) amounts of w[i].
void KeyExpansion(byte key[4 * Nk], word w[4 * (Nr + 1)]);