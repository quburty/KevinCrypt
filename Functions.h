#pragma once


#include "Bitset_Typedef.h"

//#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

//using namespace std;

//void ShowKey();

//void ShowEveryRKey()
//{
//	for (int i = 0; i < 4 * (Nr + 1); ++i)
//		cout << "w[" << dec << i << "] = " << hex << w[i].to_ulong() << endl;
//}

BYTE BytetoBYTE(byte b);

byte BYTEToBitset(BYTE B);

BYTE x_time(BYTE b, BYTE n);

class State
{
private:
	BYTE asciCode[4][4];
	

public:

	static byte basicKey[32];

	State();
	State(BYTE _asciCode[4][4]);

	State(BYTE _asciCode[16]);

	State(BYTE* _asciCode, int len);

	char* operator= (State state);

	State operator= (char cArr[16]);

	operator char* () = delete;

	char* operator[] (int idx);

	void SubBytes();
	
	void InvSubBytes();

	void ShiftRows();

	void InvShiftRows();

	void MixColumns();

	void InvMixColumns();

	//함수에 문제가 있지만, 암호화,복호화 할 때 문제가 없으므로 그대로 사용함. 
	void AddRoundKey(word w[4]);

	void AddRoundKey(word w1, word w2, word w3, word w4);

	void AddRoundKey(BYTE b[16]);

	void ShowState();

	void Round(int num, word w[4]);

	void InvRound(int num, word w[4]);

	void Encrypt();

	void Decrypt();

	void Rebuild(const int& currentLen);
	bool UnRebuild(int& len);

	//friend ostream& operator<<(ostream& os, const State& state);

};

