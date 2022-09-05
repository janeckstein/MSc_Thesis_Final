#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include "Exception.h"
#include "Tick.h"
#include "Asset.h"
#include "benchmark/benchmark.h"

void loadData(Asset&, std::string);
//void loadData(Asset& S, std::string file, Tick* storage) {
void loadData(Asset& S, std::string file) {

/*
        Loads all historical data available in the given file into
        the object S, of class Asset.

*/

    std::ifstream input0(file);
    assert(input0.is_open());
    int lines = 0;
    std::string date;
    double price;

    while (!input0.eof()) {
        input0 >> date >> price;
        lines += 1;
    }
    input0.close();

    S = Asset("S&P 500", lines);
    std::ifstream input(file.c_str());

    assert(input.is_open());
    while (!input.eof()) {
        input >> date >> price;
        S.addData(Tick(S.getName(), price, date));

        input.close();

    }
}

static double new_v = 0, old_v = 0 , mean = 0, sd = 0, sum_m = 0, sum_s = 0;


void trading_func(double* portfolio, const int& DAYS_CONSIDERED, Asset& SP500, Asset& NASDAQ, double& money, const double& INITIAL_MONEY) {

    for (int i = DAYS_CONSIDERED; i < SP500.getFilled() - 1; i++) {
        // Update portfolio worth
        money *= portfolio[2] + portfolio[0] * SP500.getData(i).getPrice() / SP500.getData(i - 1).getPrice() +
                 portfolio[1] * NASDAQ.getData(i).getPrice() / NASDAQ.getData(i - 1).getPrice();
        if (i == DAYS_CONSIDERED) {
            for (int j = i - DAYS_CONSIDERED; j < i; j++) {
                //std::cout << j << "\n";
                sum_m += SP500.getData(j).getPrice() / NASDAQ.getData(j).getPrice();
                sum_s += pow(SP500.getData(j).getPrice() / NASDAQ.getData(j).getPrice(), 2);
            }
        } else {

            new_v = SP500.getData(i - 1).getPrice() / NASDAQ.getData(i - 1).getPrice();
            old_v = SP500.getData(i - 1 - DAYS_CONSIDERED).getPrice() /
                    NASDAQ.getData(i - 1 - DAYS_CONSIDERED).getPrice();
            sum_m += new_v - old_v;
            sum_s += new_v * new_v - old_v * old_v;
            };
        mean = sum_m / DAYS_CONSIDERED;
        sd = sqrt(sum_s / DAYS_CONSIDERED - mean * mean);
        const double MARGIN = sd * 2;
// old if structure
        if (SP500.getData(i).getPrice() / NASDAQ.getData(i).getPrice() > mean + MARGIN) {
            // SP500 is outperforming NASDAQ, as their ratio is big. Thus, short SP500 and buy NASDAQ.
            portfolio[0] = -1.0;
            portfolio[1] = 1.0;
            portfolio[2] = 1.0;
        } else if (SP500.getData(i).getPrice() / NASDAQ.getData(i).getPrice() < mean - MARGIN) {
            // SP500 is underperforming NASDAQ, as their ratio is small. Thus, buy SP500 and short NASDAQ.
            portfolio[0] = 1.0;
            portfolio[1] = -1.0;
            portfolio[2] = 1.0;
        } else {
            // The ratio of SP500 and NASDAQ is inside the "normal" values.
            portfolio[0] = 0.0;
            portfolio[1] = 0.0;
            portfolio[2] = 1.0;
        }
    }
}

static void BM_Pairs_Trading(benchmark::State& state) {
    // We load the historical data for S&P 500 and NASDAQ
    Asset SP500, NASDAQ;
    loadData(SP500, "my_sp500.txt" );
    loadData(NASDAQ, "my_nasdaq.txt");
    const static int DAYS_CONSIDERED = 30; // Number of days we consider
    static const double INITIAL_MONEY = 1000.0; // Start with 1000$
    double money;
    double portfolio[3];
    for (auto _ : state) {
        money = INITIAL_MONEY; // Portfolio worth
        portfolio[0] = 1.0;
        portfolio[1] = -1.0;
        portfolio[2] = 1;// The portfolio {x, y, z} indicates weights in the portfolio of SP500, NASDAQ and cash, respectively.
        trading_func(portfolio, DAYS_CONSIDERED, SP500, NASDAQ, money, INITIAL_MONEY);
        new_v = 0;
        old_v = 0;
        mean = 0;
        sd = 0;
        sum_m = 0;
        sum_s = 0;

        //uncomment for results output
/*
        std::cout << "\n---------------------------------------------\n";
        std::cout << "Results\n\n" << "  Start date: " << SP500.getData(0).getDate() << "\n" << "  End date: "
                  << SP500.getData(SP500.getFilled() - 2).getDate() << "\n";
        std::cout << "  Return: " << (money / INITIAL_MONEY - 1) * 100 << "%";
        std::cout << "\n---------------------------------------------\n";


        std::cout << "\n";
*/
    }
}
// Register the function as a benchmark
BENCHMARK(BM_Pairs_Trading);
// Run the benchmark
BENCHMARK_MAIN();
