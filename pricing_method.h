//
// Created by Kenneth McAvoy on 10/9/20.
//

#ifndef ASSIGNMENT_3_PRICING_METHOD_H
#define ASSIGNMENT_3_PRICING_METHOD_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

class Pricing_Method {
public:
    virtual double Black_Scholes_Option_Price()=0;
    virtual double Binomial_Option_Price(int N)=0;
};

#endif //ASSIGNMENT_3_PRICING_METHOD_H
