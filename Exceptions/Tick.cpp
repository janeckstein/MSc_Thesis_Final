//
// Created by jan eckstein on 04.07.22.
//
#include <iostream>
#include "Tick.h"
#include <ctime>

Tick::Tick() {

}

Tick::Tick(std::string name, double price, std::string date)
{
    // The price of the stock cannot be negative.

    if (price<0) {
        throw Exception("Negative price",
                        "The price of the stock must non negative");

    }

    // date must have the format YYYY/MM/DD
    struct tm tm;
    if (date!="0000/00/00" && !strptime(date.c_str(), "%Y/%m/%d", &tm)) {
        throw Exception("Invalid date format", "Date format must be YYYY/MM/DD");
    }

    mPrice=price;
    mName=name;
    mDate=date;


}

std::string Tick::getName(void) {
    return mName;
}

double Tick::getPrice(void) {
    if (mPrice<0) {
        throw Exception("Negative price","The price of the stock must non negative");
    }
    return mPrice;
}


std::string Tick::getDate(void) {
    return mDate;
}


bool Tick::isNull(void) {
    return mDate=="0000/00/00";
}

std::ostream& operator<<(std::ostream& output, Tick& q) {
    // Used to display a nice output of the class

    std::string name = q.getName();
    double price = q.getPrice();
    std::string date=q.getDate();
    output<<"Name: "<<name<<"\nPrice: "<<price<<"\nDate: "<<date<<"\n";

    return output;
}