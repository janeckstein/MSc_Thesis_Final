//
// Created by jan eckstein on 04.07.22.
//

#ifndef MAIN_CPP_ASSET_H
#define MAIN_CPP_ASSET_H

#include <cmath>
#include "Exception.h"//  This class throws errors using the class "error"
#include "Tick.h"
#include<vector>

class Asset;

class Asset
{
private:
    // member variables
    int mSize, mFilled;
    std::string mName; // name of the Asset
    std::vector<Tick> mData;

public:
    Asset();//constructor
    Asset(std::string, int); // constructor
    std::string getName(void); // get Asset's name
    int getFilled(void); // returns mFilled
    Tick getData(int); // returns mFilled
    void addData(Tick); // add historical data
    friend std::ostream& operator<<(std::ostream&, Asset&);

};


#endif //MAIN_CPP_ASSET_H
