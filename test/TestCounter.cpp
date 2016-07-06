#include "TestCounter.h"

using namespace std;

TestCounter::TestCounter() : TestClass("Counter", this)
{
	addTest("Set", &TestCounter::Set);
	addTest("GetSource", &TestCounter::GetSource);
	addTest("SetSavedDuration", &TestCounter::SetSavedDuration);
	addTest("Duration", &TestCounter::Duration);
	addTest("IsWithoutReset", &TestCounter::IsWithoutReset);
	addTest("UpdateDuration", &TestCounter::UpdateDuration);
	addTest("RazPeriodHour", &TestCounter::RazPeriodHour);
	addTest("RazPeriodDay", &TestCounter::RazPeriodDay);
	addTest("RazPeriodMonth", &TestCounter::RazPeriodMonth);
	addTest("RazPeriodYear", &TestCounter::RazPeriodYear);
    SetMockTime(2000,01,01,12,00,00);
}

TestCounter::~TestCounter()
{
}

bool TestCounter::Set()
{
    Counter myTestCounter;

    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    return true;
}

bool TestCounter::GetSource()
{
    Counter myTestCounter;

    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    string source = myTestCounter.GetSource();

    assert("fragxpl-owfs.default:lamp"==source);
    return true;
}

bool TestCounter::SetSavedDuration()
{
    Counter myTestCounter;
    time_t myTime;

    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);

    SetMockTime(2000,01,01,12,00,00);
    myTime = time(nullptr);
    myTestCounter.SetSavedDuration(185, myTime);
    assert(185==myTestCounter.GetInternalDuration());

    SetMockTime(2000,01,01,12,05,00);
    myTestCounter.SetSavedDuration(185, myTime);
    assert(0==myTestCounter.GetInternalDuration());

    return true;
}

bool TestCounter::Duration()
{
    Counter myTestCounter;

    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    myTestCounter.SetSavedDuration(185, time(nullptr));
    assert(185==myTestCounter.GetInternalDuration());
    assert("185"==myTestCounter.GetDuration());

    myTestCounter.Set("fragxpl-owfs.default:lamp", "MINUTE", "20", "MINUTE", false);
    myTestCounter.SetSavedDuration(210, time(nullptr));
    assert("3"==myTestCounter.GetDuration());

    myTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "20", "MINUTE", false);
    myTestCounter.SetSavedDuration(3600+3600+1800, time(nullptr));
    assert("2.5"==myTestCounter.GetDuration());

    return true;
}

bool TestCounter::IsWithoutReset()
{
    Counter myTestCounter;

    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "", "", true);
    assert(true==myTestCounter.IsValueToSave());
    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    assert(false==myTestCounter.IsValueToSave());
    return true;
}

bool TestCounter::UpdateDuration()
{
    Counter myTestCounter;

    SetMockTime(2000,01,01,12,00,00);
    myTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,01,35);
    myTestCounter.UpdateDuration("LOW");
    assert("95"==myTestCounter.GetDuration());

    SetMockTime(2000,01,01,12,02,10);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,02,53);
    myTestCounter.UpdateDuration("LOW");
    assert("43"==myTestCounter.GetDuration());

    SetMockTime(2000,01,01,12,03,10);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,03,37);
    myTestCounter.UpdateDuration("LOW");
    assert("70"==myTestCounter.GetDuration());

    SetMockTime(2000,01,01,12,13,10);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,13,37);
    myTestCounter.UpdateDuration("LOW");
    SetMockTime(2000,01,01,12,13,39);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,13,42);
    myTestCounter.UpdateDuration("LOW");
    assert("30"==myTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodHour()
{
    Counter myTestCounter;

    SetMockTime(2000,01,01,12,00,00);
    myTestCounter.Set("fragxpl-owfs.default:lamp", "MINUTE", "2", "HOUR", false);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,13,58,00);
    myTestCounter.UpdateDuration("LOW");
    assert("118"==myTestCounter.GetDuration());

    SetMockTime(2000,01,01,14,12,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,15,03,00);
    myTestCounter.UpdateDuration("LOW");
    assert("51"==myTestCounter.GetDuration());

    SetMockTime(2000,01,01,15,12,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,15,21,00);
    myTestCounter.UpdateDuration("LOW");
    assert("60"==myTestCounter.GetDuration());

    SetMockTime(2000,01,02,01,01,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,01,15,00);
    myTestCounter.UpdateDuration("LOW");
    SetMockTime(2000,01,02,01,33,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,01,41,00);
    myTestCounter.UpdateDuration("LOW");
    assert("22"==myTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodDay()
{
    Counter myTestCounter;

    SetMockTime(2000,01,01,12,00,00);
    myTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "3", "DAY", false);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,18,30,00);
    myTestCounter.UpdateDuration("LOW");
    assert("6.5"==myTestCounter.GetDuration());

    SetMockTime(2000,01,04,21,00,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,05,05,15,00);
    myTestCounter.UpdateDuration("LOW");
    assert("8.3"==myTestCounter.GetDuration());

    SetMockTime(2000,01,05,15,15,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,05,16,30,00);
    myTestCounter.UpdateDuration("LOW");
    assert("9.5"==myTestCounter.GetDuration());

    SetMockTime(2000,01,18,01,00,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,18,04,00,00);
    myTestCounter.UpdateDuration("LOW");
    SetMockTime(2000,01,19,04,30,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,19,05,30,00);
    myTestCounter.UpdateDuration("LOW");
    assert("4.0"==myTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodMonth()
{
    Counter myTestCounter;

    SetMockTime(2000,01,01,12,00,00);
    myTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "4", "MONTH", false);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,12,00,00);
    myTestCounter.UpdateDuration("LOW");
    assert("24.0"==myTestCounter.GetDuration());

    SetMockTime(2000,06,10,15,00,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,06,11,20,00,00);
    myTestCounter.UpdateDuration("LOW");
    assert("29.0"==myTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodYear()
{
    Counter myTestCounter;

    SetMockTime(2000,01,01,12,00,00);
    myTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "1", "YEAR", false);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,12,00,00);
    myTestCounter.UpdateDuration("LOW");
    assert("24.0"==myTestCounter.GetDuration());

    SetMockTime(2001,06,10,15,00,00);
    myTestCounter.UpdateDuration("HIGH");
    SetMockTime(2001,06,11,20,00,00);
    myTestCounter.UpdateDuration("LOW");
    assert("29.0"==myTestCounter.GetDuration());

    return true;
}
