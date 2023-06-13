#ifndef BAD_REQUEST_EXCEPTION_HEADERFILE
#define BAD_REQUEST_EXCEPTION_HEADERFILE 

#include "../custom_exception.hpp"

class BadRequestException : public CustomException {
public:
    const char* what() const throw() {
        return "Bad Request";
    }
};

#endif
