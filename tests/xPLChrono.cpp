#include "UnitTest/UnitTest.h"
#include "xPLChrono.h"
#include <cassert>
#include <thread>
#include "thread/mingw.thread.h"
#include "SimpleSock/ControlSockMock.h"
#include "xPLLib/Schemas/SchemaObject.h"
#include "Plateforms/Plateforms.h"

using namespace std;

extern void SetMockTime(int year, int month, int day, int hour, int minute, int seconde);


vector<TestClass> UnitTest::m_TestClassList;
xPLChrono xPLDev;

void start()
{
    char exeName[] = "test";
    char confName[] = "config";
    char* argv[2];

    argv[0]= exeName;
    argv[1]= confName;
    xPLDev.ServiceStart(2, argv);
}

TEST_METHOD(xPLChrono, Start)
{
    string msg;
    xPL::SchemaObject sch;

    remove("config");
    SetMockTime(2000, 1, 1, 12, 30, 0);

    thread integrationTest(start);
    integrationTest.detach();

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("xPL Chrono"==sch.GetValue("appname"));

    return true;
}

TEST_METHOD(xPLChrono, StdConfig)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schCfg(xPL::SchemaObject::cmnd, "config", "response");

    schCfg.SetValue("interval", "30");
    schCfg.SetValue("newconf", "test");
    msg = schCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.default");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("30"==sch.GetValue("interval"));
    assert("fragxpl-chrono.test"==sch.GetSource());

    return true;
}

TEST_METHOD(xPLChrono, SetAdvConfig)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::trig, "sensor", "basic");
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "request");

    schAdvCfg.SetValue("configname", "testdevice");
    schAdvCfg.SetValue("source", "fragxpl-owfs.default:lamp");
    schAdvCfg.SetValue("unit", "SECONDE");
    schAdvCfg.SetValue("resetperiod", "2");
    schAdvCfg.SetValue("resetunit", "HOUR");
    schAdvCfg.SetValue("savevalue", "1");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("fragxpl-owfs.default"==sch.TargetAddress.ToString());
    assert("sensor"==sch.GetClass());
    assert("request"==sch.GetType());
    assert("current"==sch.GetValue("request"));
    assert("lamp"==sch.GetValue("device"));

    schSensor.SetValue("device", "lamp");
    schSensor.SetValue("current", "HIGH");
    schSensor.SetValue("type", "output");
    msg = schSensor.ToMessage("fragxpl-owfs.default", "*");
    ControlSockMock::SetNextRecv(msg);
    Plateforms::delay(500);

    return true;
}

TEST_METHOD(xPLChrono, Counter)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::trig, "sensor", "basic");

    SetMockTime(2000, 1, 1, 14, 00, 0);
    msg = ControlSockMock::GetLastSend(10); //Pass the hbeat message

    schSensor.SetValue("device", "lamp");
    schSensor.SetValue("current", "LOW");
    schSensor.SetValue("type", "output");
    msg = schSensor.ToMessage("fragxpl-owfs.default", "*");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("sensor"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("testdevice"==sch.GetValue("device"));
    assert("5400"==sch.GetValue("current"));

    return true;
}

TEST_METHOD(xPLChrono, GetAdvConfig)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfg.SetValue("command", "request");
    schAdvCfg.SetValue("configname", "testdevice");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("advanceconfig"==sch.GetClass());
    assert("current"==sch.GetType());
    assert("testdevice"==sch.GetValue("configname"));
    assert("fragxpl-owfs.default:lamp"==sch.GetValue("source"));

    return true;
}

TEST_METHOD(xPLChrono, Stop)
{
    string msg;

    xPLDev.ServicePause(true);
    xPLDev.ServicePause(false);
    xPLDev.ServiceStop();

    msg = ControlSockMock::GetLastSend(10);     //Pass hbeat message
    return true;
}

TEST_METHOD(xPLChrono, ReStart)
{
    string msg;
    xPL::SchemaObject sch;

    thread integrationTest(start);
    integrationTest.detach();

    msg = ControlSockMock::GetLastSend(10);     //Pass hbeat message
    return true;
}

TEST_METHOD(xPLChrono, SavedValue)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::cmnd, "sensor", "request");

    schSensor.SetValue("request", "current");
    schSensor.SetValue("device", "testdevice");
    msg = schSensor.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("5400"==sch.GetValue("current"));

    return true;
}

TEST_METHOD(xPLChrono, DelAdvConfig)
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfg.SetValue("command", "delete");
    schAdvCfg.SetValue("configname", "testdevice");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);
    Plateforms::delay(500);

    schAdvCfg.ClearValues();
    schAdvCfg.SetValue("command", "delete");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);
    Plateforms::delay(500);

    return true;
}

TEST_METHOD(xPLChrono, ReStop)
{
    xPLDev.ServiceStop();
    return true;
}

