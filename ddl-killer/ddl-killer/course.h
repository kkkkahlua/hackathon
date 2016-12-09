#pragma once
#include <string>
#include <vector>
#define lastingmax 1000
using namespace std;
class Course {
private:
	int credit;
	double perc_each;
public:
	int rate,num;
	string name;
	int hrs[lastingmax];
	string ddldate[lastingmax];
	double value;
	Course(string s);	
	void Course::init(int crt, int rt, double p1);
	void add(string& date1, int spent);
	void del(int j);
	void calc_value();
};