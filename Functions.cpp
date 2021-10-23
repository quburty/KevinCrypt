#include "Functions.h"
#include "KeyExpansion256.h"
#include <iostream>

byte State::basicKey[32] = { 0x2b, 0x7e, 0x15, 0x16,
					0x28, 0xae, 0xd2, 0xa6,
					0xab, 0xf7, 0x15, 0x88,
					0x09, 0xcf, 0x4f, 0x3c,
					0x01,0x02,0x03,0x04,
					0x05,0x06,0x07,0x08,
					0x0a,0x0b,0x0c,0x0d,
					0x0e,0x0f,0x10,0x11 };//origin:keyexpansion256.h

const int constMatrix[4][4] = {
	{2,3,1,1},
	{1,2,3,1},
	{1,1,2,3},
	{3,1,1,2}
};

const int constInvMatrix[4][4] = {
	{14,11,13,9},
{9,14,11,13},
{13,9,14,11},
{11,13,9,14}
};

using namespace std;

//void ShowKey()
//{
//	cout << "KEY IS: ";
//	for (int i = 0; i < 16; ++i)
//		cout << hex << basicKey[i].to_ulong() << " ";
//	cout << endl;
//}

BYTE BytetoBYTE(byte b)
{
	return (BYTE)b.to_ulong();
}

byte BYTEToBitset(BYTE B)
{
	byte b(B);
	return b;
}

BYTE x_time(BYTE b, BYTE n)
{

	BYTE temp = 0, mask = 0x01;

	for (int i = 0; i < 8; i++) {
		if (n & mask)
			temp ^= b;

		if (b & 0x80)
			b = (b << 1) ^ 0x1B;
		else
			b <<= 1;

		mask <<= 1;
	}

	return temp;
}

State::State()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] = 16;
		}
	}
}

State::State(BYTE _asciCode[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] = _asciCode[i][j];
		}
	}
}

State::State(BYTE _asciCode[16])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] = _asciCode[i * 4 + j];
		}
	}
}

State::State(BYTE * _asciCode, int len)
{
	for (int i = 0; i < len; i++)
	{
		asciCode[i / 4][i % 4] = _asciCode[i];
	}
	for (int i = len; i < 16; i++)
	{
		asciCode[i / 4][i % 4] = 16 - len;
	}
}

char* State::operator= (State state)
{
	char temp[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i * 4 + j] = state.asciCode[i][j];
		}
	}
	return temp;
}

State State::operator= (char cArr[16])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] = cArr[i * 4 + j];
		}
	}
	return *this;
}

//State::operator char*()
//{
//	return (char*)asciCode;
//}

char * State::operator[](int idx)
{
	return (char*)asciCode[idx];
}

void State::SubBytes()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] = sBox1DArr[asciCode[i][j]];
		}
	}
}

void State::InvSubBytes()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] = invSBox1DArr[asciCode[i][j]];
		}
	}
}

void State::ShiftRows()
{
	//첫번째 행은 바꿀필요 없으니까 i=1부터 시작한다. 
	for (int i = 1; i < 4; i++)
	{
		BYTE tempArr[4];
		for (int j = 0; j < 4; j++)
			tempArr[j] = asciCode[i][j];
		for (int j = 0; j < 4; j++)
			asciCode[i][j] = tempArr[(j + i) % 4];
	}
}

void State::InvShiftRows()
{
	for (int i = 1; i < 4; i++)
	{
		BYTE tempArr[4];
		for (int j = 0; j < 4; j++)
			tempArr[j] = asciCode[i][j];
		for (int j = 0; j < 4; j++)
			asciCode[i][j] = tempArr[(j - i + 4) % 4];
	}
}

void State::MixColumns()//시발 섺쓰!
{

	for (int i = 0; i < 4; i++) {
		BYTE temp[4] = { 0, };

		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				temp[j] ^= x_time(asciCode[k][i], constMatrix[j][k]);

		asciCode[0][i] = temp[0] % 256;
		asciCode[1][i] = temp[1] % 256;
		asciCode[2][i] = temp[2] % 256;
		asciCode[3][i] = temp[3] % 256;
	}
}

void State::InvMixColumns()
{

	for (int i = 0; i < 4; i++) {
		BYTE temp[4] = { 0, };

		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				temp[j] ^= x_time(asciCode[k][i], constInvMatrix[j][k]);

		asciCode[0][i] = temp[0] % 256;
		asciCode[1][i] = temp[1] % 256;
		asciCode[2][i] = temp[2] % 256;
		asciCode[3][i] = temp[3] % 256;
	}
}

//함수에 문제가 있지만, 암호화,복호화 할 때 문제가 없으므로 그대로 사용함. 

void State::AddRoundKey(word w[4])
{
	BYTE tempBYTE[16];
	byte b[16];

	//b 할당
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				b[i * 4 + j][k] = w[i][j * 4 + k];
			}
		}
	}

	for (int i = 0; i < 16; i++)
		tempBYTE[i] = BytetoBYTE(b[i]);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] ^= tempBYTE[i * 4 + j];
		}
	}
}

void State::AddRoundKey(word w1, word w2, word w3, word w4)
{
	word wArr[4] = { w1, w2, w3,w4 };
	AddRoundKey(wArr);
}

void State::AddRoundKey(BYTE b[16])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			asciCode[i][j] ^= b[i * 4 + j];
		}
	}
}

void State::ShowState()
{
	//cout << setfill('0') << hex << uppercase;
	cout << endl;
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			//cout << setw(2) << asciCode[i][j] << "\t";
			//printf("%02X\t", asciCode[i][j]);
		}
		cout << endl << endl;
	}
	cout << endl;
}

void State::Round(int num, word w[4])
{
	if (num == 0)
		AddRoundKey(w[0], w[1], w[2], w[3]);
	else if (num > Nr)
	{
		SubBytes();
		ShiftRows();
		MixColumns();
		AddRoundKey(w[0], w[1], w[2], w[3]);
	}
	else
	{
		SubBytes();
		ShiftRows();
		AddRoundKey(w[0], w[1], w[2], w[3]);
	}
}

void State::InvRound(int num, word w[4])
{
	if (num == 0)
		AddRoundKey(w[0], w[1], w[2], w[3]);
	else if (num > Nr)
	{
		AddRoundKey(w[0], w[1], w[2], w[3]);
		InvMixColumns();
		InvShiftRows();
		InvSubBytes();
	}
	else
	{
		AddRoundKey(w[0], w[1], w[2], w[3]);
		InvShiftRows();
		InvSubBytes();
	}
}

void State::Encrypt()
{
	//byte key[32] = basicKey;
	word w[4 * (Nr + 1)];
	KeyExpansion(basicKey, w);
	for (int i = 0; i < Nr + 1; i++)
	{
		word wTempArr[4] = { w[i * 4],w[i * 4 + 1] ,w[i * 4 + 2] ,w[i * 4 + 3] };
		Round(i, wTempArr);
	}
}

void State::Decrypt()
{
	word w[4 * (Nr + 1)];
	KeyExpansion(basicKey, w);
	for (int i = Nr; i >= 0; i--)
	{
		word wTempArr[4] = { w[i * 4],w[i * 4 + 1] ,w[i * 4 + 2] ,w[i * 4 + 3] };
		InvRound(i, wTempArr);
	}
}

void State::Rebuild(const int& currentLen)
{
	for (int i = currentLen; i < 16; i++)
		asciCode[i / 4][i % 4] = 16 - currentLen;
}

//return false when last state doesn't right.
bool State::UnRebuild(int & len)
{
	int deltaLen = asciCode[3][3];
	bool correctState = true;
	for (int i = 16-deltaLen; i < 16; i++)
	{
		if (asciCode[i / 4][i % 4] != deltaLen)
		{
			correctState = false;
			break;
		}
	}
	if (!correctState)
		return false;

	len = 16 - deltaLen;
	return true;
}

//ostream &operator << (ostream& os, const State& state)
//{
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			if (state.asciCode[i][j] != 0xcc)
//				os << state.asciCode[i][j];
//		}
//	}
//	return os;
//}
