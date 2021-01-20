//
// Created by Kenneth McAvoy on 10/9/20.
//

#ifndef ASSIGNMENT_3_OPTION_PRICE_H
#define ASSIGNMENT_3_OPTION_PRICE_H



#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

class Option_Price: public Option, public Pricing_Method {
public:

    Option_Price() {
        //cout << "Default Option" << endl;
    }

    Option_Price(double, double, double, double, double, string);

    string flag;
    string getFlag();
    double Black_Scholes_Option_Price();
    double Binomial_Option_Price(int N);
    double option_delta();
    double impliedvol(double, double, double, double);

};

#endif //ASSIGNMENT_3_OPTION_PRICE_H
