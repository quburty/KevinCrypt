#pragma once

//#include "StrCrypt.h"
#include "Bitset_Typedef.h"
#include <string>
#include <fstream>

class State;

//TODO:"temp"를 define 이나 typedef나 const로 바꿀것.아니다 걍 멤버변수로 바꾸자

//파일 이름을 받은 다음
//복사 파일을 만들고
//한줄씩 암호화 한 정보를 입력한다. 
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