#include <exception>
#include "UnitTest/UnitTest.h"
#include "TestCounter.h"
#include "TestxPLChrono.h"

using namespace std;

int main()
{
    UnitTest unitTest;
    int ret = 0;


    try
    {
        unitTest.addTestClass(new TestCounter());
        unitTest.addTestClass(new TestxPLChrono());
    }
    catch(const exception &e)
    {
        unitTest.displayError(e.what());
        ret = -1;
    }

    if(ret!=-1)
        if(!unitTest.run()) ret = 1;

	return ret;
}
