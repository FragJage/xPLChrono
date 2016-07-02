#include "UnitTest/UnitTest.h"
#include "TestCounter.h"
#include "TestxPLChrono.h"

using namespace std;

int main()
{
    UnitTest unitTest;

	unitTest.addTestClass(new TestCounter());
	unitTest.addTestClass(new TestxPLChrono());
	if(!unitTest.run()) return 1;
	return 0;
}
