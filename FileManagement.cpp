#include "FileManagement.h"
#include "Functions.h"
#include <iostream>

#define FILE_MAX_BITE 1048576

//�ٲ��������� ��������� �� ������ �ִ°�
#define STATE_SIZE 16

using namespace std;


bool FileManagement::EncryptLine(const long long & line)
{
	statePtr = new State();
	file.seekg(line*STATE_SIZE, ios::beg);
	for(int i=0;i<4;i++)
		file.read((*statePtr)[i],4);

	statePtr->Encrypt();

	copyFile.seekp(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < 4; i++)
		copyFile.write((*statePtr)[i], 4);

	delete statePtr;
	statePtr = nullptr;
	return true;
}


bool FileManagement::EncryptLastLine(long long line)
{
	int len = fileLen % STATE_SIZE;

	statePtr = new State();
	file.seekg(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < len; i++)
	{
		file.read(&(*statePtr)[i / 4][i % 4], 1);
	}
	statePtr->Rebuild(len);
	statePtr->Encrypt();

	copyFile.seekp(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < 4; i++)
		copyFile.write((*statePtr)[i], 4);

	delete statePtr;
	statePtr = nullptr;
	return true;
}

bool FileManagement::AddDummyLine(long long line)
{
	State temp;
	temp.Encrypt();
	copyFile.seekp(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < 4; i++)
		copyFile.write(temp[i], 4);
	return false;
}

bool FileManagement::DecryptLine(const long long & line)
{
	statePtr = new State();
	file.seekg(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < 4; i++)
		file.read((*statePtr)[i], 4);

	statePtr->Decrypt();

	copyFile.seekp(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < 4; i++)
		copyFile.write((*statePtr)[i], 4);

	delete statePtr;
	statePtr = nullptr;
	return true;
}

bool FileManagement::DecryptLastLine(long long line)
{
	int len;
	statePtr = new State();
	file.seekg(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < 4; i++)
		file.read((*statePtr)[i], 4);

	statePtr->Decrypt();

	if (statePtr->UnRebuild(len) == false)
		return false;

	copyFile.seekp(line*STATE_SIZE, ios::beg);
	for (int i = 0; i < len; i++)
		copyFile.write(&(*statePtr)[i/4][i%4], 1);

	delete statePtr;
	statePtr = nullptr;
	return true;
}

bool FileManagement::CheckEncryptable()
{
	
	return DecryptLastLine(fileLen / STATE_SIZE - 1);
}

FileManagement::FileManagement(string _fileName)
{
	fileName = _fileName;
}

//return -1 when failed to open file
void FileManagement::FileLen()
{
	//file.open(fileName.c_str(), ios::binary);
	file.open(fileName, ios::in | ios::binary);
	if (file.is_open())
	{
		file.seekg(0, ios::end);
		fileLen = (long long)(file.tellg());
		file.seekg(0, ios::beg);

		file.close();
	}
	else
		fileLen = -1;
}

bool FileManagement::EncryptFile(string copyFIleName)
{
	//TODO: �̰� �ù� ���� String���� �ϴ� ����� ���� State�� �ٲ��

	FileLen();
	if (fileLen<=0)
		return false;
	if (fileLen > FILE_MAX_BITE)
		return false;

	file.open(fileName, ios::in|ios::binary);
	copyFile.open(copyFIleName, ios::out|ios::binary);
	
	bool success = true;

	if (fileLen%STATE_SIZE == 0)
	{
		for (long long i = 0; i < fileLen / STATE_SIZE; i++)
		{
			if (EncryptLine(i))
				cout << "line " << i << " Encrypted." << endl;
			else
			{
				cout << "line " << i << " encrypt failed" << endl;
				success = false;
				break;
			}
		}
		AddDummyLine(fileLen / STATE_SIZE);
		cout << "last line Encrypted." << endl;
	}
	else
	{
		for (long long i = 0; i < fileLen / STATE_SIZE; i++)
		{
			if (EncryptLine(i))
				cout << "line " << i << " Encrypted." << endl;
			else
			{
				cout << "line " << i << " encrypt failed" << endl;
				success = false;
				break;
			}
		}
		if (EncryptLastLine(fileLen / STATE_SIZE))
			cout << "last line Encrypted." << endl;
		else
		{
			cout << "last line encrypt failed." << endl;
			success = false;
		}
	}

	if (!success)
	{
		file.close();
		copyFile.close();
		cout << "Error occured in process. Closing file. End function." << endl;
		system("pause");
		return false;
	}

	cout << "Encrypt 100% completed! Closing file.." << endl;
	
	file.close();
	remove(fileName.c_str());

	copyFile.close();
	rename("temp", fileName.c_str());

	cout << "File process completed!" << endl;


	return true;
}

bool FileManagement::DecryptFile(string copyFileName)
{
	FileLen();

	if (fileLen <= 0)
		return false;
	if (fileLen > FILE_MAX_BITE)
		return false;

	file.open(fileName, ios::in| ios::binary);
	copyFile.open(copyFileName, ios::out|ios::binary);

	//��ȣȭ �������� Ȯ��
	if (file.is_open())
	{
		file.seekg(STATE_SIZE, ios::end);
		if (!CheckEncryptable())
		{
			cout << "�� ������ ��ȣ�� Ʋ�Ȱų�, kevincrypt�� ��ȣȭ �� ������ �ƴ� �� �����ϴ�.." << endl;
			system("pause");
			return false;
		}
	}
	else
	{
		return false;
	}

	bool success = true;

	for (long long i = 0; i < fileLen / STATE_SIZE-1; i++)
	{
		if(DecryptLine(i))
			cout << "line " << i << " Decrypted." << endl;
		else
		{
			cout << "line" << i << "decrypt failed" << endl;
			success = false;
		}
	}
	if (DecryptLastLine(fileLen / STATE_SIZE - 1))
		cout << "last line Decrypted." << endl;
	else
	{
		cout << "last line decrypt failed" << endl;
		success = false;
	}

	if (!success)
	{
		cout << "Decryption failed." << endl;
		return false;
	}

	cout << "Decrypt 100% completed!" << endl;

	file.close();
	remove(fileName.c_str());

	copyFile.close();
	rename("temp", fileName.c_str());

	cout << "File process completed!" << endl;

	return true;
	
}

//bool FileManagement::AddBYTEStoFile(BYTE BArr[16])
//{
//	if (file.is_open())
//	{
//		file.write((char*)BArr, 16);
//		return true;
//	}
//	else
//		return false;
//}

bool FileManagement::MakeCopyFile(string copyFileName)
{
	return false; //������


	ofstream writeFile;
	writeFile.open(copyFileName, ios::binary);
	writeFile.write("", 0);
	writeFile.close();
	return true;
}
