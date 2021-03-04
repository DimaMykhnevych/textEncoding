#include <tchar.h> 
#include <iostream>
#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

const char* PATH = "data.txt";
const int name_length = 40;

// task 2
void defineEncodingType()
{
	printf("Current encoding:\n");
	if (sizeof(TCHAR) == 1) {
		printf("ASCII encoding (Multi-byte character set)\n");
	}
	else {
		printf("Unicode encoding (Unicode character set)\n");
	}
	printf("\n\n");
}

//task 6
void print_names(const char names[][name_length], const size_t names_count) {

	if (_tsetlocale(LC_ALL, _T("Russian")))
	{
		for (unsigned int i = 0; i < names_count; i++)
		{
			printf("%s\n", names[i]);
		}
	}
	else
	{
		printf("\"%s\" Fail to locale\n", names);
	}
	printf("\n\n");
}

//task 7
void ConvertToUnicode(const char source[][name_length], wchar_t dest[][name_length],
	size_t size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		MultiByteToWideChar(CP_ACP, 0, source[i], sizeof(source[i]),
			dest[i], sizeof(source[i]));
	}
}

// task 8
void PrintUnicodeArray(wchar_t str[][name_length], size_t size)
{
	if (_wsetlocale(LC_ALL, L"Russian"))
	{
		for (unsigned int i = 0; i < size; i++)
		{
			_tprintf(_T("%ls\n"), str[i]);

		}
	}

	wchar_t title[] = L"Family names";

	for (unsigned int i = 0; i < size; i++)
	{
		MessageBoxW(0, str[i], title, MB_OK);
	}
	printf("\n\n");
}

 //task 9
 //compare function
int CompareElements(const void* el1, const void* el2)
{
	return wcscmp((wchar_t*)el1, (wchar_t*)el2);
}
//sorting function
void QSortUnicodeArray(wchar_t arr[][name_length], size_t size)
{
	qsort(arr, size, sizeof(arr[0]), CompareElements);
}
//function to output array
void print_vector(vector<wstring> strings) {
	if (_wsetlocale(LC_ALL, L"Russian"))
	{
		for (unsigned int i = 0; i < strings.size(); i++) {\
			_tprintf(_T("%ls\n"), strings[i].c_str());
		}
	}
	printf("\n\n");
}

// task 10
LPBOOL res;
void ConvertToASCII(wchar_t source[][name_length], char destination[][name_length], size_t size)
{
	char sym = ' ';
	for (unsigned int i = 0; i < size; i++)
	{
		WideCharToMultiByte(CP_ACP, 0, source[i], sizeof(source[i]) / sizeof(source[0][0]),
			destination[i], sizeof(source[i]) / sizeof(source[0][0]), &sym, res);
	}
	if (res) {
		printf("Some symbols weren't displayed");
	}
}

//task 12
BYTE* get_file_bytes();
size_t get_file_length();
void reverse_unicode_file();
void reverse_ascii_file();

void reverse_unicode_file() {
	wifstream read_file(PATH);
	wstring str;
	vector<wstring> all_text;
	wchar_t first_two_bytes[3];
	read_file.read(first_two_bytes, 2);
	first_two_bytes[2] = '\0';
	while (getline(read_file, str)) {
		all_text.push_back(str);
	}
	read_file.close();

	vector<wstring> reversed_text;
	for (wstring txt : all_text) {
		wstring reversed_str = L"";
		for (int i = txt.size() - 2; i >= 0; i -= 2) {
			reversed_str += txt[i];
			reversed_str += txt[i + 1];
		}
		reversed_text.push_back(reversed_str);
	}

	wofstream write_file(PATH);
	write_file << first_two_bytes;
	for (size_t i = 0; i < all_text.size(); i++) {
		write_file << reversed_text[i];
	}
	write_file.close();
}

void reverse_ascii_file() {
	ifstream read_file(PATH);
	string str;
	vector<string> all_text;

	while (getline(read_file, str)) {
		all_text.push_back(str);
	}
	read_file.close();

	ofstream write_file(PATH);
	for (size_t i = 0; i < all_text.size(); i++) {
		reverse(all_text[i].begin(), all_text[i].end());
		write_file << all_text[i];
	}
	write_file.close();
}

bool is_unicode_file() {
	BYTE* bytes = get_file_bytes();			//get the content of file in bytes
	size_t file_length = get_file_length();	//get the file length
	int flag = -1;
	return IsTextUnicode(bytes, file_length, &flag); //return the boolean value
}

BYTE* get_file_bytes(){
	ifstream file(PATH);
	size_t len = get_file_length();
	char* ret = new char[len];
	file.read(ret, len);		//reading the file data
	file.close();				//closing the file
	return (BYTE*)ret;					//return file data
}

size_t get_file_length() {
	ifstream file(PATH);
	file.seekg(0, ios::end);   //move pointer to the end of file
	size_t len = file.tellg(); //get the length of the file from the beginning
	file.seekg(0, ios::beg);   //return pointer to the start of the file
	return len;				   // return size of the file
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	const int names_count = 6;
	const char names[names_count][name_length] = {
		"Михневич Татьяна Константиновна",
		"Михневич Дмитрий Константинович",
		"Михневич Константин Георгиевич",
		"Флягина Анастасия Константиновна",
		"Флягин Владислав Константинович",
		"Флягин Константин Юриевич"
	};
	
	//task 2
	defineEncodingType();

	//task 6
	printf("Family names: \n");
	print_names(names, names_count);

	//task7
	wchar_t unicodeNames[names_count][name_length];
	ConvertToUnicode(names, unicodeNames, names_count);

	//task8
	printf("Unicode Family names: \n");
	PrintUnicodeArray(unicodeNames, names_count);

	//task 9
	printf("Sorted Unicode Family names with qsort function: \n");
	QSortUnicodeArray(unicodeNames, names_count);
	PrintUnicodeArray(unicodeNames, names_count);
	//task 9 template sort function
	printf("Sorted Unicode Family names with sort function: \n");
	vector<wstring> vector_names = {			//vector of family names to use sort template 
		L"Михневич Дмитрий Константинович",		// because it isn't working with array of c-style strings
		L"Михневич Татьяна Константиновна",
		L"Михневич Константин Георгиевич",
		L"Флягина Анастасия Константиновна",
		L"Флягин Владислав Константинович",
		L"Флягин Константин Юриевич"
	};
	sort(vector_names.begin(), vector_names.end());
	print_vector(vector_names);

	//task 10
	char asciiNames[names_count][name_length];
	ConvertToASCII(unicodeNames, asciiNames, names_count);

	//task 11
	printf("ASCII family names: \n");
	print_names(asciiNames, names_count);

	//task 12
	if (is_unicode_file()) {
		reverse_unicode_file();
	}
	else {
		reverse_ascii_file();
	}
}
