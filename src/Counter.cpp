#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "Counter.h"

using namespace std;

Counter::Counter()
{
}

Counter::Counter(const string& source, const string& unit, const string& razPeriod, const string& razUnit, bool saveValue)
{
    Set(source, unit, razPeriod, razUnit, saveValue);
}

Counter::~Counter()
{
}

void Counter::Set(const string& source, const string& unit, const string& razPeriod, const string& razUnit, bool saveValue)
{
    m_source = source;
    m_unit = ToUnit(unit);

    m_razPeriod = atoi(razPeriod.c_str());
    m_razUnit = ToPeriod(razUnit);
    m_razTime = time(nullptr);
    InitRazPeriod();

    m_duration = 0;
    m_lastTime = time(nullptr);
    m_stateHIGH = false;
    m_saveValue = saveValue;
}

string Counter::GetSource()
{
    return m_source;
}

double Counter::FloorDuration(int duration)
{
    double value=0;

    switch(m_unit)
    {
        case eUnit::UNIT_SECONDE :
            value = duration;
            break;
        case eUnit::UNIT_MINUTE :
            value = duration/60;
            break;
        case eUnit::UNIT_HOUR :
            value = (duration/3600.0) + 0.05;
            value = floor(value*10)/10;
            break;
    }

	return value;
}

string Counter::GetDuration()
{
    double value;
    ostringstream s;


    value = FloorDuration(m_duration);
    if(m_unit==eUnit::UNIT_HOUR)
        s << fixed << setprecision(1) << value;
    else
        s << fixed << setprecision(0) << value;

    return s.str();
}

unsigned int Counter::GetInternalDuration()
{
    return m_duration;
}

void Counter::SetInternalDuration(unsigned int duration)
{
    m_duration = duration;
}

bool Counter::UpdateDuration(const string& state)
{
    double oldDuration = FloorDuration(m_duration);
    bool newState;
    time_t newTime = time(nullptr);


    if((m_razPeriod!=ePeriod::PERIOD_NONE)&&(difftime(newTime, m_razTime)>0))
    {
        m_duration = 0;
        while(difftime(newTime, m_razTime)>0)
            NextRazPeriod();
    }

    if(state=="HIGH")
        newState = true;
    else
        newState = false;

    if(m_stateHIGH==true) m_duration += difftime(newTime, m_lastTime);

    m_lastTime = newTime;
    m_stateHIGH = newState;

    return (oldDuration!=FloorDuration(m_duration));
}

void Counter::NextRazPeriod()
{
	struct tm *t;


	switch(m_razUnit)
	{
		case ePeriod::PERIOD_MINUTE:
			m_razTime += m_razPeriod*60;
			break;
		case ePeriod::PERIOD_HOUR:
			m_razTime += m_razPeriod*60*60;
			break;
		case ePeriod::PERIOD_DAY:
			m_razTime += m_razPeriod*60*60*24;
			break;
		case ePeriod::PERIOD_MONTH:
			t = localtime(&m_razTime);
			t->tm_mon += m_razPeriod;
			if(t->tm_mon>12)
			{
				t->tm_mon -= 12;
				t->tm_year++;
			}
			m_razTime = mktime(t);
			break;
		case ePeriod::PERIOD_YEAR:
			t = localtime(&m_razTime);
			t->tm_year += m_razPeriod;
			m_razTime = mktime(t);
			break;
        case ePeriod::PERIOD_NONE:
            return;
	}
}

void Counter::InitRazPeriod()
{
	struct tm *t;
	float n;


	t = localtime(&m_razTime);

	switch(m_razUnit)
	{
		case ePeriod::PERIOD_MINUTE:
			t->tm_sec = 0;
			n = (float) t->tm_min;
			n/= m_razPeriod;
			n++;
			t->tm_min = m_razPeriod*(int)n;
			break;
		case ePeriod::PERIOD_HOUR:
			t->tm_sec = 0;
			n = (float) t->tm_hour;
			n/= m_razPeriod;
			n++;
			t->tm_hour = m_razPeriod*(int)n;
			break;
		case ePeriod::PERIOD_DAY:
			t->tm_sec = 0;
			n = (float) t->tm_yday;
			n/= m_razPeriod;
			n++;
			t->tm_yday = m_razPeriod*(int)n;
			if(t->tm_yday>365)
			{
				t->tm_yday -= 365;
				t->tm_year++;
			}
			break;
		case ePeriod::PERIOD_MONTH:
			t->tm_sec = 0;
			t->tm_mday= 0;
			n = (float) t->tm_mon;
			n/= m_razPeriod;
			n++;
			t->tm_mon = m_razPeriod*(int)n;
			if(t->tm_mon>12)
			{
				t->tm_mon -= 12;
				t->tm_year++;
			}
			break;
		case ePeriod::PERIOD_YEAR:
			t->tm_sec = 0;
			t->tm_mday= 0;
			t->tm_mon = 0;
			n = (float) t->tm_year;
			n/= m_razPeriod;
			n++;
			t->tm_year = m_razPeriod*(int)n;
			break;
        case ePeriod::PERIOD_NONE:
            return;
	}

	m_razTime = mktime(t);
}

bool Counter::IsValueToSave()
{
    return m_saveValue;
}

Counter::ePeriod Counter::ToPeriod(const string& period)
{
    if(period == "MINUTE") return ePeriod::PERIOD_MINUTE;
    if(period == "HOUR") return ePeriod::PERIOD_HOUR;
    if(period == "DAY") return ePeriod::PERIOD_DAY;
    if(period == "MONTH") return ePeriod::PERIOD_MONTH;
    if(period == "YEAR") return ePeriod::PERIOD_YEAR;
    return ePeriod::PERIOD_NONE;
}

Counter::eUnit Counter::ToUnit(const string& unit)
{
    if(unit == "SECONDE") return eUnit::UNIT_SECONDE;
    if(unit == "MINUTE") return eUnit::UNIT_MINUTE;
    return eUnit::UNIT_HOUR;
}
