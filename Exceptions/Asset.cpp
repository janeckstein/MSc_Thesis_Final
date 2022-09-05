//
// Created by jan eckstein on 04.07.22.
//
#include <iostream>
#include "Tick.h"
#include "Asset.h"
#include <ctime>
#include <cstring>

Asset::Asset() {

}

Asset::Asset(std::string name, int size) // Creates class with historical data of the Asset, with a fixed size
{
    if (size<1) throw Exception("Error in size", "The size of historical data must be at least equal to 1");
    mName=name;
    mData=new Tick[size];
    mSize = size;
    mFilled=0;

    // We fill in the array mData with objects of class Tick.
    for (int i=0; i<mSize; i++)
    {
        mData[i] = Tick(name, 0.0, "0000/00/00"); // Ticks with date 0000/00/00 are considered as Null
    }

}


std::string Asset::getName(void) {
    return mName;
}

int Asset::getFilled(void) {
    return mFilled;
}

std::ostream& operator<<(std::ostream& output, Asset& q) {
    std::string name = q.getName();
    output<<"Asset name: "<<name<<"\n";
    if (!q.mData[0].isNull()) {
        output<<"Start date: "<<q.mData[0].getDate()<<"\n";
        output<<"End date: "<<q.mData[q.mFilled-1].getDate()<<"\n";
    }
    output<<"Length of available data: "<<q.mFilled<<"\n\n";

    output<<"  Date\t\t Price\n ------\t\t-------\n\n";
    for (int i=0; i<q.mSize; i++) {
        if (q.mData[i].isNull()) continue;
        output<<q.mData[i].getDate()<<"\t "<<q.mData[i].getPrice()<<"\n";
    }

    return output;
}

void Asset::addData(Tick q) {
    // We add the Tick q to the historical data.- If historical data is full, throw exception
    if (mFilled==mSize) throw Exception("Data size limit reached", "There is no space to store more historical data");

    // If Tick name doesn't match Asset name, throw exception
    if (q.getName()!=mName) throw Exception("Invalid name", "Tick's name does not match Asset's name");



    // Sort Ticks. Notice that the elements from 0 to mFilled-1 are already sorted.
    if (mFilled==0) {
        mData[0]=q;
        mFilled++;
        return;
    }


    for (int i=0; i<mFilled; i++) {
        {
            if (mData[i].getDate()>q.getDate()) {
                mData[mFilled]=mData[i];
                mData[i]=q;
                break;
            }
            if (i==mFilled-1) {

                mData[mFilled]=q;
            }
        }
       // check later when adding tick data in trading code
      //  mData[mFilled]=q;
    }



    mFilled++;
}

Tick Asset::getData(int i) {
    if (i<0 || i>=mSize)  throw Exception("Out of bound", "The index is out of bound");
    return mData[i];
}
