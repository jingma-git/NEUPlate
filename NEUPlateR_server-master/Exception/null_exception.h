#ifndef NULLEXCEPTION_H
#define NULLEXCEPTION_H

#include <exception>

class NullException : public std::exception
{
public:
    const char* what() const throw();
};

#endif // NULLEXCEPTION_H
