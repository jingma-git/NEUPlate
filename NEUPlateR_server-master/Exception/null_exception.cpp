#include "null_exception.h"

const char* NullException::what() const throw()
{
    return "null value is return";
}
