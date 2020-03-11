#ifndef SAVES_H
#define SAVES_H

#include <string>
#include <fstream>

namespace leaderboard {
	void save(int arr[], std::string names[], const int len);
	void read(int arr[], std::string names[], const int len);
	void sort(int arr[], std::string[], const int len);
}

#endif // !SAVES_H
