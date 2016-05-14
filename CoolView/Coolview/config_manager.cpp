#include "config_manager.h"

#include <Qstring>

#include "util/ini/CVIniConfig.h"
#include "config/RunningConfig.h"

namespace detail
{

template <typename R, typename Arg1>
class ReturnVoid: public std::unary_function<Arg1, void>
{
public:
  typedef std::function<R(Arg1)> Func;
  ReturnVoid(Func &&func): func_(func) {}

  void operator()(Arg1 arg) { func_(arg); }

private:
  Func func_;
};

typedef std::function<void (int)> SetIntFunc;
typedef std::function<void (float)> SetFloatFunc;
typedef std::function<int(void)> GetIntFunc;
typedef std::function<float(void)> GetFloatFunc;

class SetVaraintFunc: public std::unary_function<QVariant, bool>
{
public:
  SetVaraintFunc(SetIntFunc &&func1)
    : set_int_func_(std::move(func1)) {};

  SetVaraintFunc(SetFloatFunc &&func1)
    : set_float_func_(std::move(func1)) {};

  bool operator()(QVariant var) 
  {
    if (set_int_func_ && var.canConvert<int>()) {
      set_int_func_(var.toInt());
    } else if (var.canConvert<float>()) {
      set_float_func_(var.toFloat());
    } else {
      return false;
    }
    return true;
  }

private:
  SetIntFunc set_int_func_;
  SetFloatFunc set_float_func_;
};

class GetVaraintFunc: public std::unary_function<void, QVariant>
{
public:
  GetVaraintFunc(GetIntFunc &&func)
    : get_int_func_(std::move(func)) {}

  GetVaraintFunc(GetFloatFunc &&func)
    : get_float_func_(std::move(func)) {}

  QVariant operator()() 
  {
    if (get_int_func_)
      return get_int_func_();
    else
      return get_float_func_();
  }

private:
  GetIntFunc get_int_func_;
  GetFloatFunc get_float_func_;
};

} // namespace detail

using namespace detail;
using namespace placeholders;

ConfigManager& ConfigManager::instance()
{
  static ConfigManager obj;
  return obj;
}

ConfigManager::ConfigManager()
{
  auto running_config = RunningConfig::Instance();

  // hardware monitor parameters
  {
    SetFloatFunc set_func = 
      std::bind(&RunningConfig::SetCpuTempThreshold, running_config, _1);
    GetFloatFunc get_func = 
      std::bind(&RunningConfig::GetCpuTempThreshold, running_config);
    func_pairs_.insert(
      "CpuTempThreshold",
      std::make_pair(
        SetVaraintFunc(std::move(set_func)),
        GetVaraintFunc(std::move(get_func))));
  }
  
  {
    SetIntFunc set_func = 
      std::bind(&RunningConfig::SetCpuTempAction, running_config, _1);
    GetIntFunc get_func = 
      std::bind(&RunningConfig::GetCpuTempAction, running_config);
    func_pairs_.insert(
      "CpuTempAction",
      std::make_pair(
        SetVaraintFunc(std::move(set_func)),
        GetVaraintFunc(std::move(get_func))));
  }

  {
    SetFloatFunc set_func = 
      std::bind(&RunningConfig::SetMainboardTempThreshold, running_config, _1);
    GetFloatFunc get_func = 
      std::bind(&RunningConfig::GetMainboardTempThreshold, running_config);
    func_pairs_.insert(
      "MainboardTempThreshold",
      std::make_pair(
        SetVaraintFunc(std::move(set_func)),
        GetVaraintFunc(std::move(get_func))));
  }

  {
    SetIntFunc set_func = 
      std::bind(&RunningConfig::SetMainboardTempAction, running_config, _1);
    GetIntFunc get_func = 
      std::bind(&RunningConfig::GetMainboardTempAction, running_config);
    func_pairs_.insert(
      "MainboardTempAction",
      std::make_pair(
        SetVaraintFunc(std::move(set_func)),
        GetVaraintFunc(std::move(get_func))));
  }
}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::Update( const ConfigDict &dict )
{
  bool need_update = false;
  for (auto it = dict.begin(); it != dict.end(); ++it) {
    auto func_it = func_pairs_.find(it.key());
    if (func_it != func_pairs_.end()) {
      if (func_it.value().second() != it.value()) {
        if (func_it.value().first(it.value()) && !need_update) {
          need_update = true;
        }
      }
    }
  }
  
  return need_update;
}

void ConfigManager::SaveToConfigFiles()
{
  RunningConfig::Instance()->saveConfig();
}

ConfigDict ConfigManager::Dump() const
{
  ConfigDict config;
  for (auto it = func_pairs_.begin(); it != func_pairs_.end(); ++it)
  {
    config.insert(it.key(), it.value().second());
  }
  return config;
}


