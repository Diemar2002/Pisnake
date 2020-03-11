#include "saves.h"
using namespace std;

void leaderboard::save(int arr[], string names[], const int len) {
	fstream file;
	file.open("pointfile.txt", fstream::trunc | fstream::out);

	for (int i = 0; i < len; i++)
		file << arr[i] << ' ';

	file << ' ';
	for (int i = 0; i < len; i++)
		file << names[i].c_str() << ' ';

	file.close();
}

void leaderboard::read(int arr[], string names[], const int len) {
	fstream file;
	file.open("pointfile.txt");

	char c;
	int cnt = 0;
	int num = 0;

	while (file.get(c) && !file.eof()) {
		if (cnt >= len)
			break;

		if (c == ' ') {
			arr[cnt] = num;
			num = 0;
			cnt++;
		}
		else {
			unsigned char digit = (int)c - 48;
			num *= 10;
			num += digit;
		}
	}

	string buff;
	cnt = 0;
	while (file.get(c) && !file.eof()) {
		if (cnt >= len)
			break;

		if (c == ' ') {
			buff += '\0';
			names[cnt] = buff;
			buff = "";
			cnt++;
		}
		else {
			buff += c;
		}
	}

	file.close();
}

void leaderboard::sort(int arr[], string names[], const int len) {
	bool sorted = false;

	while (!sorted) {
		sorted = true;
		for (int i = 0; i < (len - 1); i++) {
			int& a = arr[i];
			int& b = arr[i + 1];
			if (a < b) {
				int c = a;
				string buff = names[i];

				a = b;
				b = c;
				names[i] = names[i + 1];
				names[i + 1] = buff;
				sorted = false;
			}

		}
	}
}