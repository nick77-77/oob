#include "Employees.h"
#include <string>
using namespace std;

Employees::Employees()
{
}
Employees::Employees(string id, string ln, string fn, string d, string dh, string t, double l,
	string r, string s, string g)
{
	EmployeeID = id;
	lname = ln;
	fname = fn;
	dob = d;
	doh = dh;
	Employee_Title = t;
	Employee_Level = l;
	Sales_Region = r;
	Employee_Status = s;
	gender = g;
	salary = calcSalary(l);
};
