#ifndef EXCEPTION_HEADERFILE
#define EXCEPTION_HEADERFILE

#include <exception>
#include <string>

class CustomException : public std::exception {
public:
    virtual const char* what() const throw() = 0;
};

#endif
