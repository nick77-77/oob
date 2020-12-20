#pragma once
#ifndef EMPLOYEES_HEADER
#define EMPLOYEES_HEADER


#include "Weeklysales.h"
#include <string>
using namespace std;

class Employees
{
public:
	Employees();
	Employees(string id, string ln, string fn, string d, string dh, string t, double l,
		string r, string s, string g);
	string EmployeeID;
	string lname;
	string fname;
	string dob;
	string doh;
	string Employee_Title;
	double Employee_Level = 0;
	string Sales_Region;
	string Employee_Status;
	string gender;
	double salary = 0;
	double qtr[4] = { 0,0,0,0 };
	double yearlyTotal = 0;
	double commission[4] = { 0,0,0,0 };
	double yearlyCom = 0;
	double bonus = 0;

private:

	double calcSalary(double l)
	{
		int sal = 0;
		if (l == 1)
		{
			sal = 350 * 52;
			return sal;
		}
		else if (l == 2)
		{
			sal = 400 * 52;
			return sal;
		}
		else if (l == 3)
		{
			sal = 450 * 52;
			return sal;
		}
		else
			return sal;
	}

};

#endif