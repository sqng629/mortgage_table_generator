//
//  loan.cpp
//  
//
//  Created by Sherman Ng on 3/15/15.
//
//

#include "loan.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <regex>



using namespace std;


struct date
{
    int month;
    int day;
    int year;
};

double calculate_monthly_payment(double principal, int num_payments, double interest_rate);

int parse_date(string date_string, date& date_struct);



int main(int argc, char* argv[])
{
    
    if (argc != 5)
    {
        cout << "incorrect number of arguments" << endl;
        return 1;
    }
    
    ofstream output_file("loan.csv");
    
    //4 parameters to the program:
    //1. principal
    //2. length of the loan
    //3. interest rate
    //4. start date of the loan
    
    //example command line execution of the program:
    //  ./loan 200000 30 0.035 11/25/2013
    
    double principal_remaining = atof(argv[1]);
    int num_payments = atoi(argv[2]) * 12;
    double rate = atof(argv[3]);
    
    date current_date;
    
    if (parse_date(argv[4], current_date))
    {
        return 1;
    }
    
    
    
    double monthly_payment = calculate_monthly_payment(principal_remaining,  num_payments, rate);
    
    output_file << "date" << "," << "loan" << "," << "monthly payments" << "," << "interest paid" << "," << "principal paid off" << endl;
    
    output_file << std::fixed << std::setprecision(2);
    
    for (int i = 0; i < num_payments; i++)
    {
        double interest = principal_remaining * rate / 12.0f;
        double principal_paid_off = monthly_payment - interest;
        
        output_file << current_date.month << "-" << current_date.day << "-" << current_date.year << ",";
        
        output_file << principal_remaining << "," << monthly_payment << "," << interest <<","<< principal_paid_off << endl;
        
        if (current_date.month == 12) {
            current_date.year++;
            current_date.month = 1;
        }
        else
        {
            current_date.month++;
        }
        
        principal_remaining = principal_remaining - principal_paid_off;
    }
    
    
    output_file.close();
    return 0;
}

double calculate_monthly_payment(double principal, int num_payments, double interest_rate)
{
    //i = first principal payoff
    //i + i(1+r/12) + i(1+r/12)^2.......+i(1+r/12)^(num_payments - 1) = principal
    
    double coeff = 0.0f;
    
    for (int i = 0; i < num_payments; ++i) {
        coeff += pow(1 + interest_rate/12.0f, i);
    }
    
    double first_payoff = principal / coeff;
    
    double monthly_payment = first_payoff + interest_rate * principal/12.0f;
    
    return monthly_payment;
    
}

int parse_date(string date_string, date& date_struct)
{
    
    std::regex pattern("([0-9]{1,2})[/-]{1}([0-9]{1,2})[/-]{1}([0-9]{4})");
    smatch m;
    if (regex_search(date_string, m, pattern)) {

        string month_s = m[1];
        string day_s = m[2];
        string year_s = m[3];
        date_struct.month = atoi(month_s.c_str());
        date_struct.day = atoi(day_s.c_str());
        date_struct.year = atoi(year_s.c_str());
        
        return 0;
    }
    else
    {
        cout << "date format incorrect" << endl;
        return 1;
    }
    
    
}