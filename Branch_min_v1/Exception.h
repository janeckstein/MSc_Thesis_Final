//
// Created by jan eckstein on 04.07.22.
//

#ifndef MAIN_CPP_EXCEPTION_H
#define MAIN_CPP_EXCEPTION_H

#include <string>

class Exception
{
public:
    std::string problem, summary;
    Exception(std::string sum, std::string problem);
    void DebugPrint();
};


#endif //MAIN_CPP_EXCEPTION_H
