#include "xPLChrono.h"

int main(int argc, char* argv[])
{
    xPLChrono xPLDev;

	Service* pService = Service::Create("xPLChrono", "Count when a device have the HIGH state", &xPLDev);
	int res = pService->Start(argc, argv);
	Service::Destroy();
	return res;
}
