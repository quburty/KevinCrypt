#include "KeyEdit.h"
#include "Functions.h"
#include "KISA_SHA256.h"
#include <iostream>

void KeyEdit::Static_Key_Edit_Apply()
{
	BYTE tempArr[32];
	SHA256_Encrypt(BYTE_Arr, len, tempArr);
	for (int i = 0; i < 32; i++)
	{
		State::basicKey[i] = tempArr[i];
	}
}

void KeyEdit::Initialize(BYTE arr[32], int len)
{
	this->len = len;
	for (int i = 0; i < len; i++)
		BYTE_Arr[i] = arr[i];
	for (int i = len; i < 32; i++)
		BYTE_Arr[i] = 0x00;
}

bool KeyEdit::Edit(std::string key)
{
	if (key.size() > 32)
		return false;
	str = key;
	for (unsigned int i = 0; i < key.size(); i++)
		BYTE_Arr[i] = key[i];
	for (unsigned int i = static_cast<unsigned int>(key.size()); i < 32; i++)
		BYTE_Arr[i] = 0x00;
	Static_Key_Edit_Apply();

	return true;
}

void KeyEdit::PrintClassKey()
{
	using namespace std;
	cout << "Current Key: " << str << endl;
}
