#include "Exception.h"

Exception::Exception(const char *message) {
    this->message = message;
}

Exception::Exception() {
}

Exception::~Exception() {
}

const char* Exception::what() const throw() {
    return message;
}