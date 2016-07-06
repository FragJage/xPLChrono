#include <cassert>
#include "UnitTest/UnitTest.h"
#include "../src/Counter.h"
#include "TimeMock.h"

using namespace std;

class TestCounter : public TestClass<TestCounter>
{
public:
    TestCounter();
    ~TestCounter();
    bool Set();
    bool GetSource();
    bool SetSavedDuration();
    bool Duration();
    bool IsWithoutReset();
    bool UpdateDuration();
    bool RazPeriodHour();
    bool RazPeriodDay();
    bool RazPeriodMonth();
    bool RazPeriodYear();
};
