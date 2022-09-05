//
// Created by jan eckstein on 04.07.22.
//

#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include "Exception.h"
#include "Tick.h"
#include "Asset.h"

void loadData(Asset&, std::string);
void updateMoney(double&, double, double, bool);

void updateMoney(double& m, double past, double future, bool buy) {
    if (buy) {
        m*=future/past;
        std::cout<<"Long. Money: "<<m<<"$. Change: "<<100*(future/past-1)<<"%.\n";
    } else {
        m*=past/future;
        std::cout<<"Short. Money: "<<m<<"$. Change: "<<100*(past/future-1)<<"%.\n";
    }
}


void loadData(Asset& S, std::string file) {
    /*

    Loads all historical data available in the given file into
    the object S, of class Asset.

    */

    std::ifstream input0(file);
    assert(input0.is_open());
    int lines=0;
    std::string date;
    double price;
    while (!input0.eof()) {
        input0 >> date >> price;
        lines+=1;
    }
    input0.close();
    S=Asset("S&P 500", lines);

    std::ifstream input(file.c_str());

    assert(input.is_open());
    while (!input.eof()) {
        input >> date >> price;
        S.addData(Tick(S.getName(), price, date));
    }
    input.close();

}

void trading_func(double* portfolio, const int& DAYS_CONSIDERED, Asset& SP500, Asset& NASDAQ, double& money, const double& INITIAL_MONEY) {
    for(int i=DAYS_CONSIDERED; i<SP500.getFilled()-1; i++) {
        // Update portfolio worth
        money*=portfolio[2]+portfolio[0]*SP500.getData(i).getPrice()/SP500.getData(i-1).getPrice()+portfolio[1]*NASDAQ.getData(i).getPrice()/NASDAQ.getData(i-1).getPrice();

        // Show current state of portfolio.
        // std::cout<<"> Portfolio weights: \n\t"<<100*portfolio[0]<<"% in SP500\n\t"<<100*portfolio[1]<<"% in NASDAQ\n\t"<<100*portfolio[2]<<"% in cash\n"<<"  Worth: "<<money<<"$\n  Return: "<<(money/INITIAL_MONEY-1)*100<<"%\n  Date: "<<SP500.getData(i).getDate();
        // std::cout<<"\n\n";


        double mean=0, sd=0;
        for (int j=i-DAYS_CONSIDERED; j<i; j++) {
            mean+=SP500.getData(j).getPrice()/NASDAQ.getData(j).getPrice();
            sd+=pow(SP500.getData(j).getPrice()/NASDAQ.getData(j).getPrice(),2);

        }
        mean/=DAYS_CONSIDERED;
        sd=sqrt(sd/DAYS_CONSIDERED-pow(mean, 2));
        const double MARGIN=sd*2;
        if (SP500.getData(i).getPrice()/NASDAQ.getData(i).getPrice()>mean+MARGIN) {
            // SP500 is outperforming NASDAQ, as their ratio is big. Thus, short SP500 and buy NASDAQ.
            portfolio[0]=-1.0;
            portfolio[1]=1.0;
            portfolio[2]=1.0;
        } else if (SP500.getData(i).getPrice()/NASDAQ.getData(i).getPrice()<mean-MARGIN) {
            // SP500 is underperforming NASDAQ, as their ratio is small. Thus, buy SP500 and short NASDAQ.
            portfolio[0]=1.0;
            portfolio[1]=-1.0;
            portfolio[2]=1.0;
        } else {
            // The ratio of SP500 and NASDAQ is inside the "normal" values.
            portfolio[0]=0.0;
            portfolio[1]=0.0;
            portfolio[2]=1.0;
        }
    }
}

int main(int argc, char* argv[]) {
    // We load the historical data for S&P 500 and NASDAQ
    Asset SP500, NASDAQ;
    loadData(SP500, "my_sp500.txt");
    loadData(NASDAQ, "my_nasdaq.txt");

    const double INITIAL_MONEY=1000.0; // Start with 1000$
    double money=INITIAL_MONEY; // Portfolio worth
    double portfolio[3]={1.0, -1.0, 1}; // The portfolio {x, y, z} indicates weights in the portfolio of SP500, NASDAQ and cash, respectively.
    const int DAYS_CONSIDERED=30; // Number of days we consider
/*
    for(int i=DAYS_CONSIDERED; i<SP500.getFilled()-1; i++) {
        // Update portfolio worth
        money*=portfolio[2]+portfolio[0]*SP500.getData(i).getPrice()/SP500.getData(i-1).getPrice()+portfolio[1]*NASDAQ.getData(i).getPrice()/NASDAQ.getData(i-1).getPrice();

        // Show current state of portfolio.
        std::cout<<"> Portfolio weights: \n\t"<<100*portfolio[0]<<"% in SP500\n\t"<<100*portfolio[1]<<"% in NASDAQ\n\t"<<100*portfolio[2]<<"% in cash\n"<<"  Worth: "<<money<<"$\n  Return: "<<(money/INITIAL_MONEY-1)*100<<"%\n  Date: "<<SP500.getData(i).getDate();
        std::cout<<"\n\n";


        double mean=0, sd=0;
        for (int j=i-DAYS_CONSIDERED; j<i; j++) {
            mean+=SP500.getData(j).getPrice()/NASDAQ.getData(j).getPrice();
            sd+=pow(SP500.getData(j).getPrice()/NASDAQ.getData(j).getPrice(),2);

        }
        mean/=DAYS_CONSIDERED;
        sd=sqrt(sd/DAYS_CONSIDERED-pow(mean, 2));
        const double MARGIN=sd*2;
        if (SP500.getData(i).getPrice()/NASDAQ.getData(i).getPrice()>mean+MARGIN) {
            // SP500 is outperforming NASDAQ, as their ratio is big. Thus, short SP500 and buy NASDAQ.
            portfolio[0]=-1.0;
            portfolio[1]=1.0;
            portfolio[2]=1.0;
        } else if (SP500.getData(i).getPrice()/NASDAQ.getData(i).getPrice()<mean-MARGIN) {
            // SP500 is underperforming NASDAQ, as their ratio is small. Thus, buy SP500 and short NASDAQ.
            portfolio[0]=1.0;
            portfolio[1]=-1.0;
            portfolio[2]=1.0;
        } else {
            // The ratio of SP500 and NASDAQ is inside the "normal" values.
            portfolio[0]=0.0;
            portfolio[1]=0.0;
            portfolio[2]=1.0;
        }
    }
*/
    trading_func(portfolio, DAYS_CONSIDERED, SP500, NASDAQ, money, INITIAL_MONEY);

    std::cout<<"\n---------------------------------------------\n";
    std::cout<< "Results\n\n"<<"  Start date: "<<SP500.getData(0).getDate()<<"\n"<<"  End date: "<<SP500.getData(SP500.getFilled()-2).getDate()<<"\n";
    std::cout<<"  Return: "<<(money/INITIAL_MONEY-1)*100<<"%";
    std::cout<<"\n---------------------------------------------\n";


    std::cout<<"\n";
}