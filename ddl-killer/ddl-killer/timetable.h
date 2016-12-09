#pragma once
#include <vector>
#include <string>
#define total 20
const int _total_time = 12;
using namespace std;
struct node3 {
	vector<string> name;
	vector<int> stime, etime;
};
class Timetable {
public:
	Timetable();
	int freetime;
	bool busy[total];
	void Timetable::init(int num, node3 temp);
private:
	vector<string> name;
	vector<int> _end_time, _start_time;
	int _course_num;
	Timetable(string);
	void calc();
	void add();
};