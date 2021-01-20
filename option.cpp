//
// Created by Kenneth McAvoy on 10/09/20.
//

#include "option.h"

Option::Option(double t, double K, double S, double r, double sigma) // Constructor
{
    time_to_maturity = t;
    strike_prc = K;
    asset_prc = S;
    rf_rate = r;
    volatility = sigma;
}


Option::~Option() //Destructor
{

}

double Option::getTime() {
    return time_to_maturity;
}
double Option::getStrike() {
    return strike_prc;
}
double Option::getAsset() {
    return asset_prc;
}
double Option::getRf() {
    return rf_rate;
}
double Option::getVolatility() {
    return volatility;
}