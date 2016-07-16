#include <iostream>
#include <exception>
#include "xPLChrono.h"

int main(int argc, char* argv[])
{
    int res = 0;

    try
    {
        xPLChrono xPLDev;

        Service* pService = Service::Create("xPLChrono", "Count when a device have the HIGH state", &xPLDev);
        res = pService->Start(argc, argv);
        Service::Destroy();
    }
    catch(const exception &e)
    {
        std::cout << e.what();
    }
	return res;
}
