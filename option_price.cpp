//
// Created by Kenneth McAvoy on 10/09/20.
//

#include "option.h"
#include "pricing_method.h"
#include "option_price.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/math/distributions.hpp>

boost::math::normal_distribution<> dist(0 , 1);

double norm_cdf(const double& x) {
    double z = 1.0 / (1.0 + 0.2316419 * x);
    double z_sum = z * (0.319381530 + z * (-0.356563782 + z * (1.781477937 + z * (-1.821255978 + 1.330274429 * z))));

    if (x >= 0.0) {
        return (1.0 - (1.0/(pow(2*M_PI,0.5))) * exp(-0.5*x*x) * z_sum);
    } else {
        return 1.0 - norm_cdf(-x);
    }
}

string Option_Price::getFlag() {
    return flag;
}

Option_Price::Option_Price(double t, double K, double S, double r, double sigma, string optiontype): Pricing_Method(), Option(t,K,S,r,sigma) {
    //cout << "Custom Option" << endl;
    if (optiontype == "C" || optiontype == "c") {
        flag = "C";
    } else if (optiontype == "P" || optiontype == "p") {
        flag = "P";
    } else {
        flag = "C";
        cout << "Incorrect Option type, defaulted to Call Option" << endl;
    }
}

double Option_Price::Black_Scholes_Option_Price() {
    double t = getTime();
    double K = getStrike();
    double S = getAsset();
    double r = getRf();
    double sigma = getVolatility();

    if (flag == "C") {
        double d1 = (log(S/K) + (r + 0.5*sigma*sigma)*t)/(sigma*(pow(t,0.5)));
        double d2 = (log(S/K) + (r - 0.5*sigma*sigma)*t)/(sigma*(pow(t,0.5)));
//        cout << "d1 = " << d1 << endl;
//        cout << "d2 = " << d2 << endl;
        double prc = S * boost::math::cdf(dist, d1) - K*exp(-r*t) * boost::math::cdf(dist, d2);
//        cout << "prc Call = " << prc << endl;
//        cout << "d1 hand: " << norm_cdf(d1) << endl;
//        cout << "d1 boost: " << boost::math::cdf(dist, d1) << endl;


        return prc;
    } else if (flag == "P") {
        double d1 = (log(S/K) + (r + 0.5*sigma*sigma)*t)/(sigma*(pow(t,0.5)));
        double d2 = (log(S/K) + (r - 0.5*sigma*sigma)*t)/(sigma*(pow(t,0.5)));
//        cout << "d1 = " << d1 << endl;
//        cout << "d2 = " << d2 << endl;
        double prc = -S * boost::math::cdf(dist, -1*d1) + K*exp(-r*t) * boost::math::cdf(dist, -1*d2);
//        cout << "prc Put = " << prc << endl;
//        cout << "d1 hand P: " << norm_cdf(-1*d1) << endl;
//        cout << "d1 boost P: " << boost::math::cdf(dist, -1*d1) << endl;

        return prc;
    } else {
        cout << "Incorrect Flag Parameter" << endl;
        return 0;
    }
}

double Option_Price::Binomial_Option_Price(int N) {
    double t = getTime();
    double K = getStrike();
    double S = getAsset();
    double r = getRf();
    double sigma = getVolatility();

    double deltaT = t / N;
    double u = exp(sigma*sqrt(deltaT));
    double d = exp(-sigma*sqrt(deltaT));
    double p = (exp(r*deltaT) - d) / (u - d);
    double p1 = exp(-r*deltaT)*p;
    double p2 = exp(-r*deltaT)*(1 - p);

//    vector<double> Payoff;
    vector<double> Value;

    if (flag == "C") {
        for (int i = 0; i <= N; i++) {
            Value.push_back(max(0.0, S*pow(u, N - i)*pow(d, i) - K));
//            Payoff.push_back(max(0.0, S*pow(u, N - i)*pow(d, i) - K));
//            cout << max(0.0, S*pow(u, N - i)*pow(d, i) - K) << endl;
        }
        cout << "---" << endl;
        for (int k = N - 1; k >= 0; k--) {
            for (int i = 0; i <= k; i++) {
                Value[i] = p1*Value[i] + p2*Value[i + 1];
//                cout << i << "v: " << Value[i] << endl;
//                cout << i << "p: " << p1*Payoff[i] + p2*Payoff[i + 1] << endl;
            }
        }
        return Value[0];
    } else if (flag == "P") {
        for (int i = 0; i <= N; i++) {
            Value.push_back(max(0.0, K - S*pow(u, N - i)*pow(d, i)));
        }
        for (int k = N - 1; k >= 0; k--) {
            for (int i = 0; i <= k; i++) {
                Value[i] = p1*Value[i] + p2*Value[i + 1];
            }
        }
        return Value[0];
    } else {
        cout << "Incorrect Flag Parameter" << endl;
        return 0;
    }

}

double Option_Price::option_delta() {
    double t = getTime();
    double K = getStrike();
    double S = getAsset();
    double r = getRf();
    double sigma = getVolatility();

    double d1 = ((log(S/K)) + (r + (0.5*pow(sigma, 2)))*t) / (sigma*sqrt(t));

    if (flag == "C"){
        return boost::math::cdf(dist, d1);
    } else if (flag == "P") {
        return (boost::math::cdf(dist, d1)-1);
    } else {
        cout << "Incorrect Flag - return 0" << endl;
        return 0;
    }
}

double Option_Price::impliedvol(double price_target, double lowerlimit, double upperlimit, double tolerance) {
    double t = getTime();
    double K = getStrike();
    double S = getAsset();
    double r = getRf();

    double midpoint = (lowerlimit + upperlimit)/2.0;
    Option_Price optiontmp(t, K, S, r, midpoint, "c");
    double optPrice = optiontmp.Black_Scholes_Option_Price();
    int count = 0;
    do {
        if (optPrice < price_target) {
            lowerlimit = midpoint;
        } else if (optPrice > price_target) {
            upperlimit = midpoint;
        }
        midpoint = (lowerlimit + upperlimit)/2.0;
        Option_Price optiontmp(t, K, S, r, midpoint, "c");
        optPrice = optiontmp.Black_Scholes_Option_Price();
        count += 1;
        if (count == 1000)
        {
            break;
        }
    } while (fabs(optPrice - price_target) > tolerance);
//    cout << "Count: " << count << endl;
    return midpoint;
}
