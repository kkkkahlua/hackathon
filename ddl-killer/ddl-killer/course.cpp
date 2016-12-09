#include "course.h"
using namespace std;
Course::Course(string s) :name(s) { num = 0; };
void Course::init(int crt, int rt, double p1) {
	credit = crt;
	rate = rt;
	perc_each = p1;
	calc_value();
}
void Course::add(string& date1, int spent) {
	ddldate[num] = date1;
	hrs[num++] = spent;
}
void Course::del(int j) {
	for (int i = j; i < num-1; ++i) {
		ddldate[i] = ddldate[i + 1];
		hrs[i] = hrs[i + 1];
	}
	ddldate[num - 1] = hrs[num - 1] = 0;
	--num;
}
void Course::calc_value() {
	value = credit * perc_each;
}
