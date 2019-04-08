#include <exception>
#pragma once

using namespace std;

class Exception: public exception {
private:
    const char *message;
public:
    Exception();
    Exception(const char *message);
    virtual ~Exception();
    const char* what() const throw();
};