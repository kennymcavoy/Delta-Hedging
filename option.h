//
// Created by Kenneth McAvoy on 10/9/20.
//

#ifndef ASSIGNMENT_3_OPTION_H
#define ASSIGNMENT_3_OPTION_H


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Option {
private:
    double time_to_maturity;
    double strike_prc;
    double asset_prc;
    double rf_rate;
    double volatility;

    void init() {
        time_to_maturity = 1;
        strike_prc = 110;
        asset_prc= 100;
        rf_rate = 0.03;
        volatility = 0.01;
    }



public:
    Option(double, double, double, double, double); // Constructor (time_to_maturity, strike_prc, asset_prc, rf_rate, volatility)
    Option() { // Default Constructor
        init();
    }
    ~Option(); // Deconstructor
    //Bond(const Bond&); // Copy Constructor



    double getTime();
    double getStrike();
    double getAsset();
    double getRf();
    double getVolatility();

};

#endif //ASSIGNMENT_3_OPTION_H
