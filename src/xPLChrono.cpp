#include <fstream>
#include "SimpleFolders/SimpleFolders.h"
#include "Plateforms/Plateforms.h"
#include "xPLLib/Schemas/SchemaSensor.h"
#include "xPLChrono.h"

using namespace std;

xPLChrono::xPLChrono() : m_AdvanceConfig(&m_xPLDevice), m_Sensors(&m_xPLDevice)
{
    m_Log = m_xPLDevice.GetLogHandle();

    m_AdvanceConfig.AddFormat("configname", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("source", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("unit", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("resetperiod", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("resetunit", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("savevalue", xPL::AdvanceConfig::ParamType::BOOLEAN, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.SetCallBack(this);

    m_xPLDevice.AddExtension(&m_AdvanceConfig);
    m_xPLDevice.AddExtension(&m_Sensors);
    m_xPLDevice.AddExtension(this);

    m_xPLDevice.Initialisation("fragxpl", "chrono", "default");
    m_xPLDevice.SetAppName("xPL Chrono", "1.0.0.0");
    m_bServicePause = false;
    m_bServiceStop = false;
}

xPLChrono::~xPLChrono()
{
}

bool xPLChrono::MsgAnswer(xPL::SchemaObject& msg)
{
    map<string, Counter>::iterator it;
    map<string, Counter>::iterator itEnd;
    string source;
    string device;
    string state;


    device = msg.GetValue("device");
    if(device=="") return true;

    source = msg.GetSource()+":"+device;
    if(m_DeviceCount[source]<1) return true;

    state = msg.GetValue("current");

    itEnd = m_Counters.end();
    for(it=m_Counters.begin(); it!=itEnd; ++it)
    {
        if(it->second.GetSource()!=source) continue;
        if(it->second.UpdateDuration(state))
            m_Sensors.ModifyMessage(it->first, it->second.GetDuration());
    }

    return true;
}

void xPLChrono::RequestValue(const string& longDevice)
{
    xPL::SchemaSensorRequest sensorRequest;
    string module;
    string shortDevice;
    size_t pos;


    pos = longDevice.find(":");
    module = longDevice.substr(0, pos);
    shortDevice = longDevice.substr(pos+1);
    sensorRequest.SetDeviceName(shortDevice);
    m_xPLDevice.SendMessage(&sensorRequest, module);
}

void xPLChrono::ConfigChange(const string& configName)
{
    std::map<std::string, std::string>* config;
    std::map<std::string, Counter>::iterator it;
    string source;
    string unit;
    string razPeriod;
    string razUnit;
    string strBool;
    bool saveValue;
    Counter counter;


    config = m_AdvanceConfig.GetConfig(configName);

    source  = (*config)["source"];
    unit    = (*config)["unit"];
    razPeriod  = (*config)["resetperiod"];
    razUnit    = (*config)["resetunit"];
    strBool    = (*config)["savevalue"];
    saveValue  = (strBool=="1") ? true : false;

    it = m_Counters.find(configName);

    if(it==m_Counters.end())
    {
        LOG_VERBOSE(m_Log) << "New counter " << configName << ":" << source;
        counter.Set(source, unit, razPeriod, razUnit, saveValue);
        m_Counters[configName] = counter;
        m_DeviceCount[source]++;
        m_Sensors.AddSensorMessage(configName, xPL::SchemaSensorTypeUtility::count, 0);
    }
    else
    {
        LOG_VERBOSE(m_Log) << "Modify counter " << configName << ":" << source;
        m_DeviceCount[it->second.GetSource()]--;
        it->second.Set(source, unit, razPeriod, razUnit, saveValue);
        m_DeviceCount[source]++;
    }

    if(source=="")
        LOG_WARNING(m_Log) << "Counter " << configName << ": No source, Counter ignored";
    else
        RequestValue(source);
}

void xPLChrono::ConfigDelete(const string& configName)
{
    std::map<std::string, Counter>::iterator it;
    std::string source;

    it = m_Counters.find(configName);
    if(it==m_Counters.end()) return;
    LOG_VERBOSE(m_Log) << "Delete counter "<<configName;
    source = it->second.GetSource();
    m_Counters.erase(it);
    m_DeviceCount[source]--;
    m_Sensors.RemoveMessage(configName);
    if(m_DeviceCount[source]<0) m_DeviceCount[source] = 0;
}

void xPLChrono::ConfigsDelete()
{
    LOG_VERBOSE(m_Log) << "Delete all filters";
    m_Counters.clear();
    m_DeviceCount.clear();
    m_Sensors.RemoveAllMessages();
}

void xPLChrono::AdvanceConfigure()
{
    int i;
    int nb;

    LOG_ENTER;

    ConfigsDelete();
    nb = m_AdvanceConfig.GetNbConfig();

    for(i=0; i<nb; i++)
    {
        std::map<std::string, std::string>* config;
        string configName;
        string source;
        string unit;
        string razPeriod;
        string razUnit;
        string strBool;
        bool saveValue;
        Counter counter;


        config = m_AdvanceConfig.GetConfig(i);

        configName = (*config)["configname"];
        source     = (*config)["source"];
        unit       = (*config)["unit"];
        razPeriod  = (*config)["resetperiod"];
        razUnit    = (*config)["resetunit"];
        strBool    = (*config)["savevalue"];
        saveValue  = (strBool=="1") ? true : false;

        if(source=="")
        {
            LOG_WARNING(m_Log) << "Counter " << configName << ": No source, Counter ignored";
            continue;
        }

        LOG_VERBOSE(m_Log) << "New Counter " << configName << ":" << source << " unit " << unit << ", razPeriod " << razPeriod << " " <<razUnit;
        counter.Set(source, unit, razPeriod, razUnit, saveValue);
        m_Counters[configName] = counter;
        m_DeviceCount[source]++;

        m_Sensors.AddSensorMessage(configName, xPL::SchemaSensorTypeUtility::count, 0);
        RequestValue(source);
    }
    LoadData();

	LOG_EXIT_OK;
}

string xPLChrono::GetDataFileName()
{
    SimpleFolders   folder;
    string          fileName;

    try
    {
        fileName = folder.AddFile(m_xPLDevice.GetConfigFolder(), "xPLChrono.data");
    }
    catch(char const* msg)
    {
        LOG_ERROR(m_Log) << "Unable to get user folder : "<< msg;
        fileName = "xPLChrono.data";
    }
    return fileName;
}

void xPLChrono::SaveData()
{
    map<string, Counter>::iterator it;
    map<string, Counter>::iterator itEnd = m_Counters.end();
    string fileName = GetDataFileName();
    fstream dataFile;


    dataFile.open(fileName, fstream::out);
    if(!dataFile.is_open())
    {
        LOG_ERROR(m_Log) << "Unable to open the datafile "<< fileName;
        return;
    }

    for(it = m_Counters.begin(); it != itEnd; ++it)
    {
        if(!it->second.IsValueToSave()) continue;
        dataFile << it->first << " " << it->second.GetInternalDuration() << " " << time(nullptr);
    }
    dataFile.close();
}

void xPLChrono::LoadData()
{
    string fileName = GetDataFileName();
    fstream dataFile;
    string configName;
    unsigned int duration;
    map<string, Counter>::iterator it;
    time_t lastTime;


    dataFile.open(fileName, fstream::in);
    if(!dataFile.is_open())
    {
        LOG_ERROR(m_Log) << "Unable to open the datafile "<< fileName;
        return;
    }

    while(dataFile >> configName)
    {
        dataFile >> duration;
        dataFile >> lastTime;
        it = m_Counters.find(configName);
        if(it==m_Counters.end()) continue;
        it->second.SetSavedDuration(duration, lastTime);
        m_Sensors.ModifyMessage(configName, it->second.GetDuration());
    }
}

int xPLChrono::ServiceStart(int argc, char* argv[])
{
    m_bServiceStop = false;
    if(argc > 1) m_xPLDevice.SetConfigFileName(argv[1]);
    m_xPLDevice.Open();

    while(!m_bServiceStop)
    {
        if(m_bServicePause)
            Plateforms::delay(500);
        else
            m_xPLDevice.WaitRecv(500);
    }

    m_xPLDevice.Close();
    SaveData();

    return 0;
}

void xPLChrono::ServicePause(bool bPause)
{
    m_bServicePause = bPause;
}

void xPLChrono::ServiceStop()
{
    m_bServiceStop = true;
}

