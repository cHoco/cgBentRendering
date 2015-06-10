#ifndef EIN_APPLICATION_HPP
#define EIN_APPLICATION_HPP

#include "Ein.hpp"

class EinApplication
{
public:
    EinApplication();
    ~EinApplication();

    double GetTime();

    void Terminate();

    static void setErrorCallback(void (*errorCallback)(int, const char *));
};
#endif
