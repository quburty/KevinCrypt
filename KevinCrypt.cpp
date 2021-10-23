//KevinCrypt v1.3 made by Kim. 
//Any copies or other use are not allowed. 
//Using AES - 256 Methods.

#include "FileManagement.h"
#include "KeyEdit.h"
#include <iostream>

using namespace std;

//BYTE keyBYTEArr[16] = "Default_Setting";
//KiloByte keyStr = keyBYTEArr;
//
//void KeyEdit();
void ShowVersion();

int main()
{
	BYTE bARr[] = { 'a','b','c' };
	KeyEdit keyEditClass(bARr, 3);

	ShowVersion();
	string select;
	cout << endl;

	while (true)
	{
		cout << endl;
		cout << "┌──────────────────────────────┐" << endl;
		cout << "Menu: 1.Encrypt 2.Decrypt" << endl;
		cout << "3.File Encrypt 4.File Decrypt" << endl;
		cout << "5.Key Edit 6.Show Key" << endl;
		cout << "0.Show Version 99.Exit" << endl;
		cout << "└──────────────────────────────┘" << endl;
		//int selection;
		
		getline(cin, select);
		
		string str;

		if (select == "3")
		{
			string fileName;
			cout << "File name: ";
			//cin.ignore();
			getline(cin, fileName, '\n');
			if (fileName[0] == '"'&&fileName[fileName.size() - 1] == '"')
			{
				fileName.pop_back();
				fileName.erase(fileName.begin());
			}
			FileManagement* FM = new FileManagement(fileName);
			if (FM->EncryptFile() == false)
			{
				cout << "File Encrypt Failed.." << endl;
			}
			delete FM;
		}
		else if (select == "4")
		{
			cout << "File name: ";
			string fileName;
			//cin.ignore();
			getline(cin, fileName, '\n');
			if (fileName[0] == '"'&&fileName[fileName.size() - 1] == '"')
			{
				fileName.pop_back();
				fileName.erase(fileName.begin());
			}
			FileManagement* FM = new FileManagement(fileName);
			if (FM->DecryptFile() == false)
				cout << "File Decrypt Failed.." << endl;
			delete FM;
		}
		else if (select == "5")
		{
			string temp;
			cin >> temp;
			if (keyEditClass.Edit(temp))
			{
				cout << "key change success!" << endl;
				keyEditClass.PrintClassKey();
			}
			else
				cout << "key should less than 32 BYTEs.." << endl;
		}
		else if (select == "6")
			keyEditClass.PrintClassKey();
		else if (select == "99")
			return 0;
		else if (select == "0")
			ShowVersion();
		else
		{
			string fileName = select;
			if (fileName[0] == '"'&&fileName[fileName.size() - 1] == '"')
			{
				fileName.pop_back();
				fileName.erase(fileName.begin());
			}
			FileManagement* FM = new FileManagement(fileName);
			if (FM->EncryptFile() == false)
			{
				cout << "File Encrypt Failed.." << endl;
			}
			delete FM;
		}
		
		cin.clear();
		cin.ignore(1000, '\n');
	}

	
	return 0;
}

//void KeyEdit()
//{
//	cout << "Enter the key less than 32 chars: ";
//	cin >> keyStr;
//	if (keyStr.StrSHA256() == true)
//		cout << "key change successed!" << endl;
//	else
//		keyStr = (BYTE*)"Default";
//}

void ShowVersion()
{
	cout << "KevinCrypt v2.1 made by Kim. " << endl;
	cout << "Any copies or other use are not allowed." << endl;
	cout << "Using AES - 256 Methods." << endl;
}
