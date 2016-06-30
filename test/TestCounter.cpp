#include "TestCounter.h"

using namespace std;

TestCounter::TestCounter() : TestClass("Counter", this)
{
	addTest("Set", &TestCounter::Set);
	addTest("GetSource", &TestCounter::GetSource);
	addTest("Duration", &TestCounter::Duration);
	addTest("IsWithoutReset", &TestCounter::IsWithoutReset);
	addTest("UpdateDuration", &TestCounter::UpdateDuration);
	addTest("RazPeriodHour", &TestCounter::RazPeriodHour);
	addTest("RazPeriodDay", &TestCounter::RazPeriodDay);
	addTest("RazPeriodMonth", &TestCounter::RazPeriodMonth);
	addTest("RazPeriodYear", &TestCounter::RazPeriodYear);
}

TestCounter::~TestCounter()
{
}

bool TestCounter::Set()
{
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    return true;
}

bool TestCounter::GetSource()
{
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    string source = MyTestCounter.GetSource();

    assert("fragxpl-owfs.default:lamp"==source);
    return true;
}

bool TestCounter::Duration()
{
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    MyTestCounter.SetInternalDuration(185);
    assert(185==MyTestCounter.GetInternalDuration());
    assert("185"==MyTestCounter.GetDuration());

    MyTestCounter.Set("fragxpl-owfs.default:lamp", "MINUTE", "20", "MINUTE", false);
    MyTestCounter.SetInternalDuration(210);
    assert("3"==MyTestCounter.GetDuration());

    MyTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "20", "MINUTE", false);
    MyTestCounter.SetInternalDuration(3600+3600+1800);
    assert("2.5"==MyTestCounter.GetDuration());

    return true;
}

bool TestCounter::IsWithoutReset()
{
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "", "", true);
    assert(true==MyTestCounter.IsValueToSave());
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    assert(false==MyTestCounter.IsValueToSave());
    return true;
}

bool TestCounter::UpdateDuration()
{
    SetMockTime(2000,01,01,12,00,00);
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "SECONDE", "2", "MINUTE", false);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,01,35);
    MyTestCounter.UpdateDuration("LOW");
    assert("95"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,01,12,02,10);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,02,53);
    MyTestCounter.UpdateDuration("LOW");
    assert("43"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,01,12,03,10);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,03,37);
    MyTestCounter.UpdateDuration("LOW");
    assert("70"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,01,12,13,10);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,13,37);
    MyTestCounter.UpdateDuration("LOW");
    SetMockTime(2000,01,01,12,13,39);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,12,13,42);
    MyTestCounter.UpdateDuration("LOW");
    assert("30"==MyTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodHour()
{
    SetMockTime(2000,01,01,12,00,00);
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "MINUTE", "2", "HOUR", false);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,13,58,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("118"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,01,14,12,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,15,03,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("51"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,01,15,12,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,15,21,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("60"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,02,01,01,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,01,15,00);
    MyTestCounter.UpdateDuration("LOW");
    SetMockTime(2000,01,02,01,33,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,01,41,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("22"==MyTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodDay()
{
    SetMockTime(2000,01,01,12,00,00);
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "3", "DAY", false);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,01,18,30,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("6.5"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,04,21,00,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,05,05,15,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("8.3"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,05,15,15,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,05,16,30,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("9.5"==MyTestCounter.GetDuration());

    SetMockTime(2000,01,18,01,00,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,18,04,00,00);
    MyTestCounter.UpdateDuration("LOW");
    SetMockTime(2000,01,19,04,30,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,19,05,30,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("4.0"==MyTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodMonth()
{
    SetMockTime(2000,01,01,12,00,00);
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "4", "MONTH", false);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,12,00,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("24.0"==MyTestCounter.GetDuration());

    SetMockTime(2000,06,10,15,00,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,06,11,20,00,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("29.0"==MyTestCounter.GetDuration());

    return true;
}

bool TestCounter::RazPeriodYear()
{
    SetMockTime(2000,01,01,12,00,00);
    MyTestCounter.Set("fragxpl-owfs.default:lamp", "HOUR", "1", "YEAR", false);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2000,01,02,12,00,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("24.0"==MyTestCounter.GetDuration());

    SetMockTime(2001,06,10,15,00,00);
    MyTestCounter.UpdateDuration("HIGH");
    SetMockTime(2001,06,11,20,00,00);
    MyTestCounter.UpdateDuration("LOW");
    assert("29.0"==MyTestCounter.GetDuration());

    return true;
}
