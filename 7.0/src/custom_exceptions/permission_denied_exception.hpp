#ifndef PERMISSION_DENIED_EXCEPTION_HEADERFILE
#define PERMISSION_DENIED_EXCEPTION_HEADERFILE

#include "../custom_exception.hpp"

class PermissionDeniedException : public CustomException {
public:
    const char* what() const throw() {
        return "Permission Denied";
    }
};

#endif
