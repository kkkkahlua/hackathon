#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstring>
#include <algorithm>
#include <fstream>
#include "course.h"
#include "timetable.h"
//#define _CRT_SECURE_NO_WARNINGS
using namespace std;
vector<Course> course;
Timetable timetable[7];
struct node {
	int timetot, _course_no, _homework_no, rate, time;
	double value;
};
struct node2 {
	int _course_no, _homework_no, _day;
};
struct node4 {
	int start, day, _course_no, _homework_no;
	string date;
};

node que[lastingmax];
int _que_len = 0;

node2 _homework_arrange[lastingmax];
int _homework_arrange_tot = 0;

node4 ans[lastingmax];

int _day[lastingmax], _free_time[lastingmax], _free_time_suffix[lastingmax], _course_no[lastingmax], _homework_no[lastingmax];
bool _find_the_solution, occupy[lastingmax], occupy2[lastingmax];
const int month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
int gap;

void delque(int id);
void _import_course();
void _import_timetable();
bool same(string s, char* s2, int l2);
void add(char* p);
void del(char* p); 
int trans(char* p, int len);
void calctrans(char* p, int& year, int& month, int& day);
bool larger(string s1, string s2);
bool is_leap_year(int year);
int diff(string s1, string s2, int& y1, int& m1, int& d1, int& y2, int& m2, int& d2);
int calc(int c, int y, int m, int d);
void generate_calendar(string date, string temp);
void resort(string date);
void work();
void dfs(int no);
void print(string& date);
string _calc_date(string& date, int day);

int main() {
	freopen("arrangement.txt", "w", stdout);

	_import_timetable();
	_import_course();
	string date;
	ifstream fin("ddl.txt");
	while (fin >> date && date[0] != '#') {
//		cout << "hhh\n";
		int num;
		fin >> num;
		fin.get();
//		cout << num << endl;
		for (int i = 0; i < num; ++i) {
//			cout << "hhh\n";
			char command[60] = { 0 };
			fin.getline(command, 60);
//			cout << command << endl;
			if (command[0] == 'A') {
//				cout << "a\n";
				add(command+4);
//				cout << "ay\n";
			}
			else {
//				cout << "d\n";
				del(command+7);
			}
		}
		resort(date);
		work();
		print(date);
	}

	fclose(stdout);
	return 0;
}

void _import_course() {
	ifstream fin("course.txt");
	int crt, rt;
	double p1;
	string name;
	while (fin >> name && name[0]!='#') {
//		cout << name << endl;
		fin >> crt >> rt >> p1;
		Course temp(name);
		temp.init(crt, rt, p1);
		course.push_back(temp);
	}
//	cout << "hhh\n";
}
void _import_timetable() {
	ifstream fin("timetable.txt");
	string day;
	while (fin >> day && day[0]!='#') {
//		cout << day << endl;
//		cout << "hhh\n";
		int num;
		fin >> num;
		node3 temp;
		for (int i = 0; i < num; ++i) {
			string name;
			int stime, etime;
			fin >> name >> stime >> etime;
			temp.name.push_back(name);
			temp.stime.push_back(stime);
			temp.etime.push_back(etime);
		}
		if (day == "Sunday") {
			timetable[0].init(num, temp);
		}
		else if (day == "Monday") {
			timetable[1].init(num, temp);
		}
		else if (day == "Tuesday") {
			timetable[2].init(num, temp);
		}
		else if (day == "Wednesday") {
			timetable[3].init(num, temp);
		}
		else if (day == "Thursday") {
			timetable[4].init(num, temp);
		}
		else if (day == "Friday") {
			timetable[5].init(num, temp);
		}
		else if (day == "Saturday") {
			timetable[6].init(num, temp);
		}
	}
//	cout << "hhh\n";
}
bool same(string s, char* s2, int l2) {
	int l1 = s.size();
	int len = max(l1, l2);
	const char *p = s.c_str();
	for (int i = 0; i<len; ++i) {
		if (*(p + i) != s2[i]) {
			return false;
		}
	}
	return true;
}
void add(char* s) {
	char name[20] = { 0 }, date[11] = { 0 };
	
	int len = strchr(s, ' ') - s;

	memcpy(name, s, len);
	strcpy(s, s + len + 1);

	len = strchr(s, ' ') - s;
	memcpy(date, s, len);
	strcpy(s, s + len + 1);
	string datee(&date[0], &date[strlen(date)]);

	int spent = 0;
	for (size_t i = 0; i < strlen(s); ++i) {
		spent *= 10;
		spent += s[i] - '0';
	}

	for (size_t i = 0; i != course.size(); ++i) {
		if (same(course[i].name,name,strlen(name))) {
			course[i].add(datee, spent);
			break;
		}
	}
}
void del(char* s) {
	char name[20] = { 0 };

	int len = strchr(s, ' ') - s;
	memcpy(name, s, len);
	strcpy(s, s + len + 1);

	for (size_t i = 0; i < course.size(); ++i) {
		for (int j = 0; j < course[i].num; ++j) {
			if (same(course[i].name, name, strlen(name)) && same(course[i].ddldate[j], s, strlen(s))) {
				course[i].del(j);
				for (int k = 0; k < _que_len; ++k) {
					if (que[k]._course_no == i+1 && que[k]._homework_no == j+1) {
						delque(k);
					}
				}
				break;
			}
		}
	}
}
int trans(char* p, int len) {
	int ans = 0;
	for (int i = 0; i<len; ++i) {
		ans *= 10;
		ans += p[i] - '0';
	}
	return ans;
}
void calctrans(char* p, int& year, int& month, int& day) {
	year = trans(p, 4);
	month = trans(p + 5, 2);
	day = trans(p + 8, 2);
}
bool larger(string s1, string s2) {
	char s[20] = { 0 };
	memcpy(s, s1.c_str(), s1.size());

	int year1, year2, month1, month2, day1, day2;
	calctrans(s, year1, month1, day1);

	memcpy(s, s2.c_str(), s2.size());
	calctrans(s, year2, month2, day2);

	return (year1 > year2 || (year1 == year2 && month1 > month2) || (year1 == year2 && month1 == month2 && day1 > day2));
}
bool is_leap_year(int year) {
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}
int diff(string s1,string s2,int& y1,int& m1,int& d1,int& y2,int& m2,int& d2) {
	char s[20] = { 0 };
	memcpy(s, s1.c_str(), s1.size());
	calctrans(s,y1, m1, d1);
	memcpy(s, s2.c_str(), s2.size());
	calctrans(s, y2, m2, d2);
	int ans = 0, i;
	if (y2 - y1 > 1) {
		for (i = y1 + 1; i <= y2 - 1; ++i) {
			if (is_leap_year(i)) {
				ans += 366;
			}
			else {
				ans += 365;
			}
		}
	}
	if (y1 < y2) {
		for (i = m1 + 1; i <= 12; ++i) {
			ans += month[i];
			if (i == 2 && is_leap_year(y1)) {
				++ans;
			}
		}
		for (i = 1; i <= m2 - 1; ++i) {
			ans += month[i];
			if (i == 2 && is_leap_year(y2)) {
				++ans;
			}
		}
	}
	if (!(y1 == y2 && m1 == m2)) {
			ans += month[m1] - d1 + 1;
			ans += d2;
	}
	else {
		ans += d2 - d1 + 1;
	}
	return ans;
}
int calc(int c, int y, int m, int d)
{
	int ans = ((c / 4) - 2 * c + y + (y / 4) + (26 * (m + 1) / 10) + d - 1) % 7;
	if (ans<0)
		ans += 7;
	return ans;
}
void generate_calendar(string date,string temp) {
	int y1, y2, m1, m2, d1, d2;
	gap = diff(date, temp, y1, m1, d1, y2, m2, d2);
	int d = calc(y1 / 100, y1 % 100, (m1 + 9) % 12 + 3, d1);
	int _total_free_time = 0;
	for (int i = 1; i <= gap; ++i) {
		_total_free_time += timetable[d].freetime;
		_free_time[i] = timetable[d].freetime;
		_day[i] = d;
		++d;
		if (d == 7) d = 0;
	}
	_free_time_suffix[1] = _free_time[1];
	for (int i = 2; i <= gap; ++i) {
		_free_time_suffix[i] = _free_time_suffix[i - 1] + _free_time[i];
	}
}
void resort(string date) {
	string temp = "0000-00-00";
	for (size_t i = 0; i != course.size(); ++i) {
		for (size_t j = 0; j != course[i].num; ++j) {
			if (larger(course[i].ddldate[j], temp)) {
				temp = course[i].ddldate[j];
			}
		}
	}
	generate_calendar(date, temp);

	_que_len = 0;
	for (size_t i = 0; i != course.size(); ++i) {
		for (size_t j = 0; j != course[i].num; ++j) {
			int k = course[i].rate, p = _que_len - 1;
			int y1, y2, m1, m2, d1, d2;
			int timetot = _free_time_suffix[diff(date, course[i].ddldate[j], y1, m1, d1, y2, m2, d2)];
			int time = ceil((double)timetot / k);
			while (p >= 0 && (course[i].value > que[p].value || (course[i].value == que[p].value && time < que[p].time))) {
				que[p + 1] = que[p];
				--p;
			}
			que[++p].value = course[i].value;
			que[p].timetot = timetot;
			que[p].time = time;
			que[p]._course_no = i+1;
			que[p]._homework_no = j+1;
			que[p].rate = course[i].rate;
			++_que_len;
		}
	}
	//for (int i = 0; i < _que_len; ++i) {
	//	cout << que[i]._course_no << " ";
	//}
	//cout << endl;
}
void delque(int id) {
	for (int i = id + 1; i < _que_len - 1; ++i) {
		que[i] = que[id + 1];
	}
	--_que_len;
}
void work() {
	memset(occupy, 0, sizeof(occupy));
	dfs(0);
}
void dfs(int no) {
	//for (int i = 0; i < 50; ++i) {
	//	cout << _course_no[i] << " ";
	//}
	//cout << endl;

	if (no == _que_len) {
		_find_the_solution = true;
		return;
	}

	bool flag = false;
	for (int k = que[no].rate; k >= 1; --k) {
		for (int i = 0; i < lastingmax; ++i) {
			occupy2[i] = occupy[i];
		}
		flag = false;
		int p = ceil(que[no].timetot / (double)k);
		int limit = course[que[no]._course_no-1].hrs[que[no]._homework_no-1];
		while (p >= limit && occupy2[p]) {
			--p;
		}
		if (p < limit) {
			continue;
		}
		while (limit) {
			if (p < 1) break;
			if (occupy2[p]) {
				--p;
				continue;
			}
			else {
				occupy2[p] = true;
				_course_no[p] = que[no]._course_no;
				_homework_no[p] = que[no]._homework_no;
				--p; --limit;
			}
		}
		if (limit) continue;
		flag = true;
		for (int i = 0; i < lastingmax; ++i) {
			occupy[i] = occupy2[i];
		}
		dfs(no+1);
		if (_find_the_solution) return;
	}
	if (!flag) return;
}
string _calc_date(string& date, int day) {
	--day;
	char s[20] = { 0 };
	memcpy(s, date.c_str(), date.size());
	int y, m, d;
	calctrans(s, y, m, d);
	d += day;
	while ((is_leap_year(y) && m == 2 && d > month[m] + 1) || (!is_leap_year(y) && m == 2 && d > month[m]) || (d > month[m])) {
		d -= month[m];
		++m;
		if (m == 13) {
			++y;
			m = 1;
		}
	}

	char ch = '-', add = '0';
	if (d < 10) {
		sprintf(s, "%d%c%d%c%c%d", y, ch, m, ch, add, d);
	}
	else {
		sprintf(s, "%d%c%d%c%d", y, ch, m, ch, d);
	}
	string ss(&s[0], &s[strlen(s)]);
	return ss;
}
void print(string &date) {
	//for (int i = 0; i < 100; ++i) {
	//	cout << _course_no[i] << " ";
	//}
	//cout << endl;

	memset(_homework_arrange, 0, sizeof(_homework_arrange));
	_homework_arrange_tot = 0;
	int p = 0;
	while (!occupy[p]) {
		++p;
	}
	for (int i = 1; i <= gap; ++i) {
		int free_time = _free_time[i], j;
		for (j = 0; j < free_time; ++j) {
			if (occupy[p + j]) {
				_homework_arrange[_homework_arrange_tot]._course_no = _course_no[p + j];
				_homework_arrange[_homework_arrange_tot]._homework_no = _homework_no[p + j];
				_homework_arrange[_homework_arrange_tot++]._day = _day[p + j];
			}
//			else break;
		}
//		if (!occupy[p + j]) break;
		p += free_time;
	}
	int day = _day[1], freetime = timetable[day].freetime, start = 1, anstot = 0, d = 1;
	for (int i = 0; i < _homework_arrange_tot; ++i) {
		if (freetime) {
			--freetime;
		}
		else {
			++day; ++d;
			if (day == 7) day = 0;
			freetime = timetable[day].freetime;
			--freetime;
			start = 1;
		}
		for (int j = start; j <= _total_time; ++j) {
			if (!timetable[day].busy[j]) {
				ans[anstot].start = j;
				ans[anstot].day = day;
				string s = _calc_date(date, d);
				ans[anstot].date = s;
				ans[anstot]._course_no = _homework_arrange[i]._course_no;
				ans[anstot++]._homework_no = _homework_arrange[i]._homework_no;
				start = j + 1;
				break;
			}
		}
	}

	//for (int i = 0; i < _homework_arrange_tot; ++i) {
	//	cout << _homework_arrange[i]._day << " ";
	//	int courseno = _homework_arrange[i]._course_no-1;
	//	int homeworkno = _homework_arrange[i]._homework_no-1;
	//	cout << course[courseno].name << " ";
	//	cout << course[courseno].ddldate[homeworkno] << endl;
	//}

	cout << date << ":\n";
	for (int i = 0; i < anstot; ++i) {
		int courseno = ans[i]._course_no-1;
		int homeworkno = ans[i]._homework_no-1;
		cout << course[courseno].name << " " << ans[i].date << " ";
		int day = ans[i].day;
		switch (day) {
		case 0:cout << "Sunday "; break;
		case 1:cout << "Monday "; break;
		case 2:cout << "Tuesday "; break;
		case 3:cout << "Wednesday "; break;
		case 4:cout << "Thursday "; break;
		case 5:cout << "Friday "; break;
		case 6:cout << "Saturday "; break;
		}
		cout << ans[i].start << " ";
		cout << course[courseno].ddldate[homeworkno] << endl;
	}
	cout << endl;

}