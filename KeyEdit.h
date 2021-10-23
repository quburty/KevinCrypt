#pragma once

#include <string>

typedef unsigned char BYTE;

class KeyEdit
{
private:
	BYTE BYTE_Arr[32];
	int len;
	std::string str;

	void Static_Key_Edit_Apply();

public:
	KeyEdit(BYTE arr[32], int len)
	{
		Initialize(arr, len);
	}

	void Initialize(BYTE arr[32], int len);

	bool Edit(std::string key);

	void PrintClassKey();
};