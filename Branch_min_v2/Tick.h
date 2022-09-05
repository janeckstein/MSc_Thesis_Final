//
// Created by jan eckstein on 04.07.22.
//

#ifndef MAIN_CPP_TICK_H
#define MAIN_CPP_TICK_H

#include <cmath>
#include "Exception.h"//  This class throws errors using the class "error"
#include <ctime>

class Tick;

class Tick
{
private:
    // member variables
    double mPrice;   // price of the stock
    std::string mName; // name of the stock
    std::string mDate; // date of the Tick

public:
    Tick(); // constructor
    Tick(std::string, double, std::string); // constructor
    std::string getName(void); // Get stock's name
    double getPrice(void); // Get stock's price
    std::string getDate(void); // Get stock's date
    bool isNull(void); // Returns true if the Tick is null


    //output
    friend std::ostream& operator<<(std::ostream&, Tick&);


};



#endif //MAIN_CPP_TICK_H
