#ifndef NOT_FOUND_EXCEPTION_HEADERFILE
#define NOT_FOUND_EXCEPTION_HEADERFILE

#include "../custom_exception.hpp"

class NotFoundException : public CustomException {
public:
    const char* what() const throw() {
        return "Not Found";
    }
};

#endif
