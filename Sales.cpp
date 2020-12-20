#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Employees.h"
#include "WeeklySales.h"
#include "Corporate.h"


using namespace std;
Corporate c;
vector<Employees>employees;
vector<double>sales;
int LoadEmployees(string f);
Employees extractInfo(string line, string delim);
vector <double> extractSales(string line, string delim);
double salesRegion(string r, double s);
int processSales(string f);
void calcTotal();
string RegionBonus();

class Corporate
{
public:
	Corporate();
	double  Sales[4] = { 0,0,0,0 };
	double  Comissions[4] = { 0,0,0,0 };
	double totals[4] = { 0,0,0,0 };
private:

};

Corporate::Corporate()
{
}

int main()
{
    vector<string>tokens;
    vector<string> fields;
    vector<vector<WeeklySales> >qtr1, qtr2, qtr3, qtr4;
    int count = 0;
    count = LoadEmployees("EmployeeList.txt");

    if (count > 0) //process if there are Employees
    { 
        processSales("SalesReport.csv"); //Process Sales for each employee 
        for (int i = 0; i < employees.size(); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                double com = 0;
                double l = employees[i].Employee_Level;
                double y = employees[i].qtr[j];
                if (l == 1)
                {
                    com = y * 0.02;
                }
                else if (l == 2)
                {
                    com = y * 0.03;
                }
                else if (l == 3)
                {
                    com = y * 0.045;
                }
                employees[i].commission[j] += com;
                employees[i].yearlyCom += employees[i].commission[j]; 
            }
        }
        for (int i = 0; i < employees.size(); i++)
        {
            salesRegion(employees[i].Sales_Region, employees[i].yearlyTotal);
            if (employees[i].Sales_Region == RegionBonus())
            {
                employees[i].bonus = employees[i].yearlyTotal * 0.03;
            }

            //start
            int max = 0;
            for (int i = 0; i < employees.size(); i++)
            {
                if (employees[i].Employee_Title == "Sales Associate")
                {
                    if (employees[i].yearlyTotal > max)
                    {
                        max = employees[i].yearlyTotal;
                    }
                }
                else
                {
                    break;
                }
            }
            if (employees[i].yearlyTotal == max)
            {
                employees[i].bonus += employees[i].yearlyTotal * 0.03;
            }

        }
        calcTotal();
        cout.setf(ios::fixed);
        cout.setf(ios::showpoint);
        cout.precision(2);

        cout << "Annual Sales Report:" << endl << endl;
        cout << "---------------------------------------------------------------------------------" << endl;
        for (int i = 0; i < employees.size(); i++)
        {
            if (employees[i].Employee_Title == "Sales Associate")
            {
                cout << "ID " << employees[i].EmployeeID << endl
                    << "\t" << employees[i].lname << ", " << employees[i].fname << ":" << endl;
                for (int q = 0; q < 4; q++)
                {
                    cout << setw(20) << "Quarter " << q + 1 << ": " << endl
                        << "\t\t" << "Sales:" << setw(5) << "$ " << employees[i].qtr[q] << endl
                        << "\t\t" << "Salary:" << setw(4) << "$ " << (employees[i].salary / 4) << endl
                        << "\t\t" << "Award:" << setw(5) << "$ " << employees[i].commission[q] << endl;
                }
                cout << "\t" << "Summary: " << endl
                    << "\t\t" << "Total Salary:" << setw(10) << "$ " << employees[i].salary << endl
                    << "\t\t" << "Total Sales:" << setw(11) << "$ " << employees[i].yearlyTotal << endl
                    << "\t\t" << "Total Commission:" << setw(6) << "$ " << employees[i].yearlyCom << endl
                    << "\t\t" << "Total Bonus:" << setw(11) << "$ " << employees[i].bonus << endl
                    << endl;
            }
        }
        cout << "Companywide Statistics " << endl
            << "\t" << "Doohickey Sales:" << endl;
        for (int q = 0; q < 4; q++)
        {
            cout << setw(20) << "Quarter " << q + 1 << ": " << endl
                << "\t\t" << "Sales:" << setw(5) << "$ " << c.Sales[q] << endl
                << "\t\t" << "Award:" << setw(5) << "$ " << c.Comissions[q] << endl;
        }
        cout << "\t" << "Summary: " << endl
            << "\t\t" << "Total Salary:" << setw(10) << "$ " << c.totals[0] << endl
            << "\t\t" << "Total Sales:" << setw(11) << "$ " << c.totals[1] << endl
            << "\t\t" << "Total Commission:" << setw(6) << "$ " << c.totals[2] << endl
            << "\t\t" << "Total Bonus:" << setw(11) << "$ " << c.totals[3] << endl
            << endl;
    }
}

int LoadEmployees(string f)
{
    int count = 0;

    //read and count passengers to recCount
    string lineRead;
    ifstream employee;
    employee.open(f);
    bool headerRead = false;

    if (employee.is_open())
    {
        while (getline(employee, lineRead))
        {
            if (headerRead)
            {
                employees.push_back(extractInfo(lineRead, ","));
                count++;
            }
            else headerRead = true;
        }
    }
    else
    {
        cout << "File did not open successfully. Critical Error.\n";
        return 0;
    }
    employee.close();
    return count;
}

Employees extractInfo(string line, string delim)
{
    //equivilent from splitstring
    size_t pos = 0;
    string token;
    vector<string> tokens;

    //Employees e; //type Employee used for loading when splitting the line.
    int counter = 0;

    while ((pos = line.find(delim)) != string::npos)
    {
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delim.length());
    }
    //instead of Employees e and individually categoritzing it.
    if (tokens.size() > 1)
    {
        tokens.push_back(line);
        Employees e(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], stod(tokens[6]), tokens[7], tokens[8], tokens[9]); //pass it to Employees  method as i push it back into the vector
        return e;
    }

}


int processSales(string f)
{
    string lineRead;
    ifstream Report;
    Report.open(f);
    int count = 0;
    int i = 0;
    int j = 0;

    bool headerRead = false;

    if (Report.is_open())
    {
        while (getline(Report, lineRead))
        {
            if (headerRead)
            {
                sales = extractSales(lineRead, ",");
                count++;

                int ctr = 0;
                for (Employees e : employees)
                {
                    if (stod(e.EmployeeID) == sales[0]) //match sutdent ids here 
                    {
                        break;
                    }
                    else
                    {
                        ctr++;
                    }
                }
                if (sales.size() > 1)
                {
                    if (ctr < employees.size())
                    {
                        if (employees[ctr].Employee_Title == "Sales Associate")
                        {
                            WeeklySales w(sales[0], sales[1], sales[2]);
                            employees[ctr].qtr[i] += sales[2]; //add weekly totals into qtr
                            if (sales[1] == 13 || sales[1] == 26 || sales[1] == 39)
                            {
                                employees[ctr].yearlyTotal += employees[ctr].qtr[i];
                                i++; //everytime it hits the end of the quarter add 1
                            }
                            if (sales[1] == 52) //reset at the end of the year
                            {
                                employees[ctr].yearlyTotal += employees[ctr].qtr[i];
                                i = 0;
                            }
                            // employees[ctr].salary = calcSalary(employees[ctr].Employee_Level);


                             /* if (j < 13) //counter to track week # (works but static?)
                              {
                                  j++;
                                  employees[ctr].qtr[i] += sales[2];
                                  if (j == 13)
                                  {
                                      j = 0;
                                      i++;
                                  }
                              }*/
                        }
                    }
                }
            }
            else headerRead = true;
        }
    }
    else
    {
        cout << "File did not open successfully. Critical Error.\n";
        return 0;
    }
    Report.close();
    return count;
}

double reg[4];

string RegionBonus()
{
    int max = 0;
    for (int i = 0; i < 4; i++)
        if (reg[i] > max)
        {
            max = reg[i];
        }
    if (max == reg[0])
    {
        return "A";
    }
    if (max == reg[1])
    {
        return "B";
    }
    if (max == reg[2])
    {
        return "C";
    }
    if (max == reg[3])
    {
        return "D";
    }
}
double salesRegion(string r, double s) // make s the Yearly total 
{
    //goal : get the total for each region to see the sum of it
    if (r == "A")
    {
        reg[0] += s; //push it back into an array to see how many
        return reg[0];
    }
    else if (r == "B")
    {
        reg[1] += s;
        return reg[1];
    }
    else if (r == "C")
    {
        reg[2] += s;
        return reg[2];
    }
    else
    {
        reg[3] += s;
        return reg[3];
    }

}

vector <double> extractSales(string line, string delim)
{
    //equivilent from splitstring
    size_t pos = 0;
    string token;
    vector<double> tokens;
    int counter = 0;
    //use toke[0] to locate specific student (loop counter to go through students	vector of course 

    while ((pos = line.find(delim)) != string::npos)
    {
        token = line.substr(0, pos);
        tokens.push_back(stod(token));
        line.erase(0, pos + delim.length());
    }
    tokens.push_back(stod(line));

    return tokens;
}

void calcTotal()
{
    for (int i = 0; i < employees.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            c.Sales[j] += employees[i].qtr[j];
            c.Comissions[j] += employees[i].commission[j];
        }

        c.totals[0] += employees[i].yearlyTotal;
        c.totals[1] += employees[i].salary;
        c.totals[2] += employees[i].yearlyCom;
        c.totals[3] += employees[i].bonus;

    }

}