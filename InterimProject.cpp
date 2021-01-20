//
// Created by Kenneth McAvoy on 10/19/20.
//

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>

#include "option.h"
#include "pricing_method.h"
#include "option_price.h"

#include <ql/quantlib.hpp>
#include <ql/time/date.hpp>
#include <ql/time/calendar.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace QuantLib;

int main()
{

    std::cout << "Interim Project - Part 1 - 1" <<endl;

    boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
            normRVGen(boost::mt19937(123), boost::normal_distribution<>());

    double S = 100.0;
    double rf = 0.025;
    double vol = 0.24;
    double T = 0.4;
    double mew = 0.05;
    double N = 100.0;
    double delta_time = (T / N);
    double strike = 105;
    double d_t = 1.0/252.0;

    std::vector<int> periods(100);
    std::iota(periods.begin(), periods.end(), 0);


    double pricepaths[100][1000];
    vector<double> Z(100, 0);
    for (int j = 0; j < 1000; ++j) {
        for (int k = 0; k < 100; ++k) {
            Z[k] = normRVGen();
        }
        for (int i = 0; i < 100; ++i) {
            if (i == 0) {
                pricepaths[i][j] = S;
            } else {
                pricepaths[i][j] = pricepaths[i-1][j] + mew*pricepaths[i-1][j]*delta_time +
                        vol*pricepaths[i-1][j]*sqrt(delta_time)*Z[i];
            }
        }
    }

//    cout << "Sample Sim Prices" << endl;
//    cout << pricepaths[0][0] << " " << pricepaths[0][1] << " " << pricepaths[0][2] << endl;
//    cout << pricepaths[1][0] << " " << pricepaths[1][1] << " " << pricepaths[1][2] << endl;
//    cout << pricepaths[99][0] << " " << pricepaths[99][1] << " " << pricepaths[99][2] << endl << endl;


    std::ofstream out_pricingPaths("./Output/pricingPaths.csv");
    for (auto& row : pricepaths) {
        for (auto col : row)
            out_pricingPaths << col <<',';
        out_pricingPaths << '\n';
    }
    out_pricingPaths.close();
    std::cout << "File saved in output folder as pricingPaths.csv" <<endl;

    std::cout << "Interim Project - Part 1 - 1 - Complete" <<endl << endl;
    std::cout << "Interim Project - Part 1 - 2" <<endl;

    double bsCallprices[100][1000];
    double bsCalldelta[100][1000];

    for (int j = 0; j < 1000; ++j) {
        for (int i = 0; i < 100; ++i) {
            Option_Price optiontmp(T-(i+1)*delta_time, strike, pricepaths[i][j], rf, vol, "c");
            bsCallprices[i][j] = optiontmp.Black_Scholes_Option_Price();
            bsCalldelta[i][j] = optiontmp.option_delta();
        }
    }

//    cout << "BS Sample Call Prices" << endl;
//    cout << bsCallprices[0][0] << " " << bsCallprices[0][1] << " " << bsCallprices[0][2] << endl;
//    cout << bsCallprices[1][0] << " " << bsCallprices[1][1] << " " << bsCallprices[1][2] << endl;
//    cout << bsCallprices[99][0] << " " << bsCallprices[99][1] << " " << bsCallprices[99][2] << endl << endl;
//
//    cout << "BS Sample Call Delta" << endl;
//    cout << bsCalldelta[0][0] << " " << bsCalldelta[0][1] << " " << bsCalldelta[0][2] << endl;
//    cout << bsCalldelta[1][0] << " " << bsCalldelta[1][1] << " " << bsCalldelta[1][2] << endl;
//    cout << bsCalldelta[99][0] << " " << bsCalldelta[99][1] << " " << bsCalldelta[99][2] << endl << endl;

    std::ofstream out_bsCallprices("./Output/bsCallprices.csv");
    for (auto& row : bsCallprices) {
        for (auto col : row)
            out_bsCallprices << col <<',';
        out_bsCallprices << '\n';
    }
    out_bsCallprices.close();
    std::cout << "File saved in output folder as bsCallprices.csv" <<endl;

    std::ofstream out_bsCalldelta("./Output/bsCalldelta.csv");
    for (auto& row : bsCalldelta) {
        for (auto col : row)
            out_bsCalldelta << col <<',';
        out_bsCalldelta << '\n';
    }
    out_bsCalldelta.close();
    std::cout << "File saved in output folder as bsCalldelta.csv" <<endl;

    double Bvalues[100][1000];

    for (int j = 0; j < 1000; ++j) {
        for (int i = 0; i < 100; ++i) {
            if (i == 0) {
                Bvalues[i][j] = bsCallprices[i][j] - bsCalldelta[i][j]*pricepaths[i][j];
            } else {
                Bvalues[i][j] = bsCalldelta[i-1][j]*pricepaths[i][j] + Bvalues[i-1][j]*exp(rf*delta_time) -
                        bsCalldelta[i][j]*pricepaths[i][j]; //d_t?
            }
        }
    }

//    cout << "B Sample Values" << endl;
//    cout << Bvalues[0][0] << " " << Bvalues[0][1] << " " << Bvalues[0][2] << endl;
//    cout << Bvalues[1][0] << " " << Bvalues[1][1] << " " << Bvalues[1][2] << endl;
//    cout << Bvalues[99][0] << " " << Bvalues[99][1] << " " << Bvalues[99][2] << endl << endl;

    std::ofstream out_Bvalues("./Output/Bvalues.csv");
    for (auto& row : Bvalues) {
        for (auto col : row)
            out_Bvalues << col <<',';
        out_Bvalues << '\n';
    }
    out_Bvalues.close();
    std::cout << "File saved in output folder as Bvalues.csv" <<endl;


    double HE[100][1000];

    for (int j = 0; j < 1000; ++j) {
        for (int i = 0; i < 100; ++i) {
            if (i == 0) {
                HE[i][j] = 0;
            } else {
                HE[i][j] = bsCalldelta[i-1][j]*pricepaths[i][j] + Bvalues[i-1][j]*exp(rf*delta_time) - bsCallprices[i][j]; //d_t?
            }
        }
    }

//    cout << "HE Sample Values" << endl;
//    cout << HE[0][0] << " " << HE[0][1] << " " << HE[0][2] << endl;
//    cout << HE[1][0] << " " << HE[1][1] << " " << HE[1][2] << endl;
//    cout << HE[99][0] << " " << HE[99][1] << " " << HE[99][2] << endl << endl;

    ofstream out_HE("./Output/HE.csv");
    for (auto& row : HE) {
        for (auto col : row)
            out_HE << col <<',';
        out_HE << '\n';
    }
    out_HE.close();
    std::cout << "File saved in output folder as HE.csv" <<endl;


    std::cout << "Interim Project - Part 1 - 2 - Complete" <<endl << endl;
    std::cout << "Interim Project - Part 2" <<endl;

    Calendar calendar = UnitedStates();
    string t0;
    string tn;
    string user_T;
    string user_K_str;

//    Test case
//    t0 = "2011-07-05";
//    tn = "2011-07-29";
//    user_T = "2011-09-17";
//    user_K_str = "500";

    std::cout << "Please Specify the Parameters" << endl;
    std::cout << "Enter the option window start date (yyyy-mm-dd):" << endl;
    std::cin >> t0;
    std::cout << "Enter the option window end date (yyyy-mm-dd):" << endl;
    std::cin >> tn;
    std::cout << "Enter the time to maturity (yyyy-mm-dd):" << endl;
    std::cin >> user_T;
    std::cout << "Enter the strike price:" << endl;
    std::cin >> user_K_str;

    if (t0.length() != 10 || tn.length() != 10 || user_T.length() != 10) {
        cout << "Please enter the dates in the format yyyy-mm-dd" << endl;
        exit(2);
    }

    Date td1, td2, td3;
    td1 = DateParser::parseFormatted(t0, "%Y-%m-%d");
    td2 = DateParser::parseFormatted(tn, "%Y-%m-%d");
    td3 = DateParser::parseFormatted(user_T, "%Y-%m-%d");
    double dayst12 = calendar.businessDaysBetween(td1, td2, true, true);
    double dayst13 = calendar.businessDaysBetween(td1, td3, true, true);
    if (dayst12 <= 0 || dayst13 <= 0) {
        cout << "End Date or Maturity Date is before Starting Date, please try again.";
        std::exit(2);
    }
    cout << endl;

    vector<double> interestrate;
    vector<string> interestdate;
    string strinterestDate;
    string strRate;
    bool interestimportflag = false;

    ifstream infileInterest("./data/interest.csv");
    if (infileInterest.fail()) {
        cout << "File not found" << endl;
        exit(1);
    } else {
        cout << "Interest file opened" << endl;
    }
    while (!infileInterest.eof()) {
        getline(infileInterest, strinterestDate, ',');
        getline(infileInterest, strRate, '\n');

        if (strinterestDate == t0) {
            interestimportflag = true;
        }
        if (interestimportflag) {
            interestdate.push_back(strinterestDate);
            interestrate.push_back((stod(strRate)/100.0));
//            cout << strinterestDate << " " << strRate << endl;
        }
        if (strinterestDate == tn) {
            interestimportflag = false;
        }
    }
    infileInterest.close();
//    cout << "First Interest Rate: " << interestdate[0] << " " << interestrate[0] << endl;
//    cout << "Last Interest Rate: " << interestdate.back() << " " << interestrate.back() << endl;
    cout << "Interest file Imported" << endl;


    vector<double> closeprice;
    vector<string> closingdate;
    string strclosingdate;
    string strcloseprice;
    bool closingimportflag = false;

    ifstream googleclosing_data("./data/sec_GOOG.csv");
    if (googleclosing_data.fail()) {
        cout << "File not found" << endl;
        exit(1);
    } else {
        cout << "Google Closing Price file opened" << endl;
    }
    while (!googleclosing_data.eof()) {
        getline(googleclosing_data, strclosingdate, ',');
        getline(googleclosing_data, strcloseprice, '\n');
        if (strclosingdate == t0) {
            closingimportflag = true;
        }
        if (closingimportflag) {
            closingdate.push_back(strclosingdate);
            closeprice.push_back(stod(strcloseprice));
        }
        if (strclosingdate == tn) {
            closingimportflag = false;
        }
    }
    googleclosing_data.close();
//    cout << "First Close Price: " << closingdate[0] << " " << closeprice[0] << endl;
//    cout << "Last Close Price: " << closingdate.back() << " " << closeprice.back() << endl;
    cout << "Google Closing Price file Imported" << endl;


    vector<string> optiondate;
    vector<string> optionT;
    vector<string> optionflag;
    vector<double> optionK;
    vector<double> optionbid;
    vector<double> optionoffer;
    vector<double> optionprice;

    string googleoption_datestr;
    string googleoption_exdatestr;
    string googleoption_flagstr;
    string googleoption_strikestr;
    string googleoption_bestbidstr;
    string googleoption_bestofferstr;
    bool optionimportflag = false;

    ifstream googleoption_data("./data/op_GOOG.csv");
    if (googleoption_data.fail()) {
        cout << "File not found" << endl;
        exit(1);
    } else {
        cout << "Google Option file opened" << endl;
    }
    while (!googleoption_data.eof()) {
        getline(googleoption_data, googleoption_datestr, ',');
        getline(googleoption_data, googleoption_exdatestr, ',');
        getline(googleoption_data, googleoption_flagstr, ',');
        getline(googleoption_data, googleoption_strikestr, ',');
        getline(googleoption_data, googleoption_bestbidstr, ',');
        getline(googleoption_data, googleoption_bestofferstr, '\n');

        if (googleoption_datestr == t0) {
            optionimportflag = true;
        }
        if (optionimportflag) {
            if (googleoption_exdatestr == user_T) {
                if (googleoption_strikestr == user_K_str) {
                    if (googleoption_flagstr == "C") {
                        optiondate.push_back(googleoption_datestr);
                        optionT.push_back(googleoption_exdatestr);
                        optionflag.push_back(googleoption_flagstr);
                        optionK.push_back(stod(googleoption_strikestr));
                        optionbid.push_back(stod(googleoption_bestbidstr));
                        optionoffer.push_back(stod(googleoption_bestofferstr));
                        optionprice.push_back((stod(googleoption_bestbidstr) +
                                               stod(googleoption_bestofferstr)) / 2);

                        if (googleoption_datestr == tn) {
                            optionimportflag = false;
                        }

                    }
                }
            }
        }
    }
    googleoption_data.close();
    cout << "Google Option file Imported" << endl;
//    cout << "First Option: " << optiondate[0] << " " << optionT[0] << " " << optionflag[0] << " " << optionK[0] <<
//         " " << optionbid[0] << " " << optionoffer[0] << " " << optionprice[0] << endl;
//    cout << "Last Option: " << optiondate.back() << " " << optionT.back() << " " << optionflag.back() << " " <<
//         optionK.back() << " " << optionbid.back() << " " << optionoffer.back() << " " << optionprice.back() << endl;


    vector<double> impliedVolatility;
    vector<double> optionDelta;
    vector<double> optionB;
    vector<double> optionHE;
    vector<double> PNLnoHE;
    vector<double> HE_diff;

    double epsilon = 1e-25;
    double lowerLimit = 1e-20;
    double upperLimit = 1.000000;

    for(int i = 0; i < optionprice.size(); i++){
        Date optDate, maturityDate;
        optDate = DateParser::parseFormatted(optiondate[i], "%Y-%m-%d");
        maturityDate = DateParser::parseFormatted(user_T, "%Y-%m-%d");
        double daysbetween = calendar.businessDaysBetween(optDate, maturityDate, true, true);
        double timetoMaturity = daysbetween * d_t;
        Option_Price optionvol(timetoMaturity, optionK[i], closeprice[i], interestrate[i], 0.0, "c");
        double impliedvol = optionvol.impliedvol(optionprice[i], lowerLimit, upperLimit, epsilon);
        Option_Price optiontmp(timetoMaturity, optionK[i], closeprice[i], interestrate[i], impliedvol, "c");
        double delta = optiontmp.option_delta();
        impliedVolatility.push_back(impliedvol);
        optionDelta.push_back(delta);
        if (i == 0) {
            optionB.push_back(optionprice[i] - (optionDelta[i] * closeprice[i]));
            optionHE.push_back(0.0);
            HE_diff.push_back(optionHE[0]);
        } else {
            optionB.push_back((optionDelta[i-1]*closeprice[i]) + (optionB[i-1] * exp(interestrate[i-1]*d_t)) -
            (optionDelta[i]*closeprice[i]));
            optionHE.push_back((optionDelta[i-1]*closeprice[i]) + (optionB[i-1]*exp(interestrate[i-1]*d_t)) -
                               (optionprice[i]));
            HE_diff.push_back(optionHE[i]-optionHE[i-1]);
        }
        PNLnoHE.push_back(optionprice[0] - optionprice[i]);
    }

    ofstream out_results("./Output/results.csv");
    out_results << "Option Window Start Date: " << t0 << "\n";
    out_results << "Option Window End Date: " << tn << "\n";
    out_results << "Option Maturity Date: " << user_T << "\n";
    out_results << "Option Strike Price: " << user_K_str << "\n";


    out_results << "date" << "," << "S" << "," << "V" << "," << "implied vol"
                << "," << "delta" << "," << "hedgingerror" << "," << "PNL" << "," << "PNLwHedge" << "\n";
    for (int i = 0; i < optionprice.size(); ++i) {
        out_results << closingdate[i] << "," << closeprice[i] << "," << optionprice[i] << "," << impliedVolatility[i]
        << "," << optionDelta[i] << "," << HE_diff[i] << "," << PNLnoHE[i] << "," << optionHE[i] << "\n";
    }
    out_results.close();
    std::cout << endl << "Analysis Complete" <<endl;
    std::cout << "File saved in output folder as results.csv" <<endl;

    std::cout << "Interim Project - Part 2 - Complete" <<endl;
    std::cout << "Interim Project - End" <<endl;
    return 0;
}

