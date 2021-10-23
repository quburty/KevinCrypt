#pragma once

//#include "StrCrypt.h"
#include "Bitset_Typedef.h"
#include <string>
#include <fstream>

class State;

//TODO:"temp"�� define �̳� typedef�� const�� �ٲܰ�.�ƴϴ� �� ��������� �ٲ���

//���� �̸��� ���� ����
//���� ������ �����
//���پ� ��ȣȭ �� ������ �Է��Ѵ�. 
class FileManagement
{
private:
	std::string fileName;
	State* statePtr = nullptr;
	long long fileLen;
	std::ifstream file;
	std::ofstream copyFile;
	bool EncryptLine(const long long & line);
	bool EncryptLastLine(long long line);
	bool AddDummyLine(long long line);
	bool DecryptLine(const long long & line);
	bool DecryptLastLine(long long line);
	bool CheckEncryptable();
public:
	FileManagement(std::string _fileName);
	void FileLen();
	bool EncryptFile(std::string copyFIleName = "temp");
	bool DecryptFile(std::string copyFileName = "temp");
	bool AddBYTEStoFile(BYTE BArr[16]) = delete;
	bool MakeCopyFile(std::string copyFileName = "temp");
};