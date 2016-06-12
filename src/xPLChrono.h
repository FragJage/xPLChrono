#ifndef XPLCHRONO_H
#define XPLCHRONO_H

#include <map>
#include "Service/Service.h"
#include "xPLLib/xPLDevCfg.h"
#include "xPLLib/Extensions/AdvanceConfig.h"
#include "xPLLib/Extensions/Sensors.h"
#include "Counter.h"


class xPLChrono : public Service::IService, public xPL::AdvanceConfig::ICallBack, public xPL::xPLDevice::IExtension //,public xPL::BasicConfig::IConfigure
{
    public:
        xPLChrono();
        ~xPLChrono();

        bool MsgAnswer(xPL::SchemaObject& msg);
        void AdvanceConfigure();
        void ConfigChange(const std::string& name);
        void ConfigDelete(const std::string& name);
        void ConfigsDelete();

		int ServiceStart(int argc, char* argv[]);
		void ServicePause(bool bPause);
		void ServiceStop();

    private:
        void RequestValue(const std::string& longDevice);
        std::string GetDataFileName();
        void SaveData();
        void LoadData();
        xPL::xPLDevCfg m_xPLDevice;
        xPL::AdvanceConfig m_AdvanceConfig;
        xPL::Sensors m_Sensors;
        bool m_bServicePause;
        bool m_bServiceStop;
        SimpleLog* m_Log;
        std::map<std::string, Counter> m_Counters;
        std::map<std::string, int> m_DeviceCount;
};
#endif // XPLCHRONO_H
