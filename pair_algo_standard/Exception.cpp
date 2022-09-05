//
// Created by jan eckstein on 04.07.22.
//

#include "Exception.h"
#include <iostream>



Exception::Exception(std::string sum, std::string prob)
{
    problem = prob;
    summary = sum;
    std::cerr << "**  Exception ("<<summary<<") **\n";
    std::cerr << "Problem: " << problem << "\n\n";
}


