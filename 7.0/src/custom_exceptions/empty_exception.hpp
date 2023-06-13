#ifndef EMPTY_EXCEPTION_HEADERFILE
#define EMPTY_EXCEPTION_HEADERFILE 

#include "../custom_exception.hpp"

class EmptyException : public CustomException {
public:
    const char* what() const throw() {
        return "Empty";
    }
};

#endif
