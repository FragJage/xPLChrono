#include <cassert>
#include <thread>
#include "xPLLib/Schemas/SchemaObject.h"
#include "Plateforms/Plateforms.h"
#include "UnitTest/UnitTest.h"
#ifdef WIN32
    #include "Thread/mingw.thread.h"
#else
    #include <thread>
#endif
#include "../src/xPLChrono.h"
#include "ControlSockMock.h"
#include "TimeMock.h"


class TestxPLChrono : public TestClass<TestxPLChrono>
{
public:
    TestxPLChrono();
    ~TestxPLChrono();
    static void ThreadStart(xPLChrono* pxPLDev);
    bool Start();
    bool StdConfig();
    bool SetAdvConfig();
    bool SourceMissing();
    bool Counter();
    bool GetAdvConfig();
    bool ModifyAdvConfig();
    bool Stop();
    bool ReStart();
    bool SavedValue();
    bool ResetSavedValue();
    bool DelAdvConfig();
    bool ReStop();
    xPLChrono xPLDev;
};
