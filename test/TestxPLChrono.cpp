#include "TestxPLChrono.h"

using namespace std;

TestxPLChrono::TestxPLChrono() : TestClass("xPLChrono", this)
{
	addTest("Start", &TestxPLChrono::Start);
	addTest("StdConfig", &TestxPLChrono::StdConfig);
	addTest("SetAdvConfig", &TestxPLChrono::SetAdvConfig);
	addTest("Counter", &TestxPLChrono::Counter);
	addTest("GetAdvConfig", &TestxPLChrono::GetAdvConfig);
	addTest("SourceMissing", &TestxPLChrono::SourceMissing);
	addTest("ModifyAdvConfig", &TestxPLChrono::ModifyAdvConfig);
	addTest("Stop", &TestxPLChrono::Stop);
	addTest("ReStart", &TestxPLChrono::ReStart);
	addTest("SavedValue", &TestxPLChrono::SavedValue);
	addTest("ResetSavedValue", &TestxPLChrono::ResetSavedValue);
	addTest("DelAdvConfig", &TestxPLChrono::DelAdvConfig);
	addTest("ReStop", &TestxPLChrono::ReStop);
}

TestxPLChrono::~TestxPLChrono()
{
}

void TestxPLChrono::ThreadStart(xPLChrono* pxPLDev)
{
    char exeName[] = "test";
    char confName[] = "config";
    char* argv[2];

    argv[0]= exeName;
    argv[1]= confName;
    pxPLDev->ServiceStart(2, argv);
}

bool TestxPLChrono::Start()
{
    string msg;
    xPL::SchemaObject sch;

    remove("config");
    SetMockTime(2000, 1, 1, 12, 30, 0);

    thread integrationTest(ThreadStart, &xPLDev);
    integrationTest.detach();

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("xPL Chrono"==sch.GetValue("appname"));

    return true;
}

bool TestxPLChrono::StdConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schCfg(xPL::SchemaObject::cmnd, "config", "response");

    schCfg.SetValue("interval", "30");
    schCfg.SetValue("newconf", "test");
    msg = schCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.default");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);     //Pass Hbeat message
    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("30"==sch.GetValue("interval"));
    assert("fragxpl-chrono.test"==sch.GetSource());

    return true;
}

bool TestxPLChrono::SetAdvConfig()
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
    assert("sensor"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("testdevice"==sch.GetValue("device"));
    assert("count"==sch.GetValue("type"));
    assert("0"==sch.GetValue("current"));

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

bool TestxPLChrono::SourceMissing()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::trig, "sensor", "basic");
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "request");

    schAdvCfg.SetValue("configname", "misssrc");
    schAdvCfg.SetValue("source", "");
    schAdvCfg.SetValue("unit", "SECONDE");
    schAdvCfg.SetValue("resetperiod", "2");
    schAdvCfg.SetValue("resetunit", "HOUR");
    schAdvCfg.SetValue("savevalue", "1");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("sensor"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("misssrc"==sch.GetValue("device"));
    assert("count"==sch.GetValue("type"));
    assert("0"==sch.GetValue("current"));

    return true;
}

bool TestxPLChrono::Counter()
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

bool TestxPLChrono::GetAdvConfig()
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
    assert("2"==sch.GetValue("resetperiod"));

    return true;
}

bool TestxPLChrono::ModifyAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfgReq(xPL::ISchema::cmnd, "advanceconfig", "request");
    xPL::SchemaObject schAdvCfgCur(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfgReq.SetValue("configname", "testdevice");
    schAdvCfgReq.SetValue("source", "fragxpl-owfs.default:lamp");
    schAdvCfgReq.SetValue("unit", "SECONDE");
    schAdvCfgReq.SetValue("resetperiod", "120");
    schAdvCfgReq.SetValue("resetunit", "MINUTE");
    schAdvCfgReq.SetValue("savevalue", "1");
    msg = schAdvCfgReq.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);
    msg = ControlSockMock::GetLastSend(10);     //Pass request state of device fragxpl-owfs.default:lamp

    schAdvCfgCur.SetValue("command", "request");
    schAdvCfgCur.SetValue("configname", "testdevice");
    msg = schAdvCfgCur.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("advanceconfig"==sch.GetClass());
    assert("current"==sch.GetType());
    assert("testdevice"==sch.GetValue("configname"));
    assert("fragxpl-owfs.default:lamp"==sch.GetValue("source"));
    assert("120"==sch.GetValue("resetperiod"));

    return true;
}

bool TestxPLChrono::Stop()
{
    string msg;
    xPL::SchemaObject sch;

    xPLDev.ServicePause(true);
    Plateforms::delay(800);
    xPLDev.ServicePause(false);
    xPLDev.ServiceStop();

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("hbeat"==sch.GetClass());
    assert("end"==sch.GetType());
    Plateforms::delay(200);
    return true;
}

bool TestxPLChrono::ReStart()
{
    string msg;

    thread integrationTest(ThreadStart, &xPLDev);
    integrationTest.detach();

    msg = ControlSockMock::GetLastSend(10);     //Pass hbeat message
    msg = ControlSockMock::GetLastSend(10);     //Pass request current lamp
    return true;
}

bool TestxPLChrono::SavedValue()
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

bool TestxPLChrono::ResetSavedValue()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schSensor(xPL::ISchema::cmnd, "sensor", "request");


    xPLDev.ServiceStop();
    msg = ControlSockMock::GetLastSend(10);     //Pass hbeat message
    Plateforms::delay(200);

    SetMockTime(2000, 1, 1, 17, 00, 0);

    thread integrationTest(ThreadStart, &xPLDev);
    integrationTest.detach();
    msg = ControlSockMock::GetLastSend(10);     //Pass hbeat message
    Plateforms::delay(200);

    schSensor.SetValue("request", "current");
    schSensor.SetValue("device", "testdevice");
    msg = schSensor.ToMessage("fragxpl-test.default", "fragxpl-chrono.test");
    ControlSockMock::SetNextRecv(msg);

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("0"==sch.GetValue("current"));

    return true;
}

bool TestxPLChrono::DelAdvConfig()
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

    msg = ControlSockMock::GetLastSend(10); //Pass message1
    msg = ControlSockMock::GetLastSend(10); //Pass message2
    msg = ControlSockMock::GetLastSend(10); //Pass message3

    return true;
}

bool TestxPLChrono::ReStop()
{
    string msg;
    xPL::SchemaObject sch;

    xPLDev.ServiceStop();

    msg = ControlSockMock::GetLastSend(10);
    sch.Parse(msg);
    assert("hbeat"==sch.GetClass());
    assert("end"==sch.GetType());
    Plateforms::delay(200);

    remove("config");
    return true;
}

