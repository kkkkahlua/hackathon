#include "timetable.h"
using namespace std;
Timetable::Timetable() {
	freetime = _total_time;
}
void Timetable::init(int num,node3 temp) {
	_course_num = num;
	for (int i = 0; i < num; ++i) {
		name.push_back(temp.name[i]);
		_end_time.push_back(temp.etime[i]);
		_start_time.push_back(temp.stime[i]);
	}
	calc();
}
void Timetable::calc() {
	for (int i = 0; i < _course_num; ++i) {
		freetime -= (_end_time[i] - _start_time[i] + 1);
		for (int j = _start_time[i]; j <= _end_time[i]; ++j) {
			busy[j] = true;
		}
	}
}
void Timetable::add() {

}