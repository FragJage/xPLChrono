#ifndef FRAGCOUNTER_H
#define FRAGCOUNTER_H

#include<time.h>
#include<string>

class Counter
{
    public:
        enum ePeriod {PERIOD_NONE, PERIOD_MINUTE, PERIOD_HOUR, PERIOD_DAY, PERIOD_MONTH, PERIOD_YEAR};
        enum eUnit {UNIT_SECONDE, UNIT_MINUTE, UNIT_HOUR};

		Counter();
		Counter(const std::string& source, const std::string& unit, const std::string& razPeriod, const std::string& razUnit, bool saveValue);
        ~Counter();

        void Set(const std::string& source, const std::string& unit, const std::string& razPeriod, const std::string& razUnit, bool saveValue);
        std::string GetSource();
        std::string GetDuration();
        unsigned int GetInternalDuration();
        void SetInternalDuration(unsigned int duration);
        bool UpdateDuration(const std::string& state);
        bool IsValueToSave();

    private:
        void InitRazPeriod();
        void NextRazPeriod();
        double FloorDuration(int duration);
        ePeriod ToPeriod(const std::string& period);
        eUnit ToUnit(const std::string& unit);

        std::string m_source;
        eUnit m_unit;
        unsigned int m_duration;

        time_t m_lastTime;
        bool m_stateHIGH;
        bool m_saveValue;

        int m_razPeriod;
        ePeriod m_razUnit;
        time_t m_razTime;

};

#endif // FRAGCOUNTER_H
