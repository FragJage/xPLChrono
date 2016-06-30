#include "UnitTest/UnitTest.h"
#include "TestCounter.h"
#include "TestxPLChrono.h"

using namespace std;

int main()
{
    UnitTest unitTest;

	unitTest.addTestClass(new TestCounter());
	unitTest.addTestClass(new TestxPLChrono());
	unitTest.run();

	return 1;
}
