#ifndef _HIBISCUS_CONFIGURATION_HPP_
#define _HIBISCUS_CONFIGURATION_HPP_

#include "yaml-cpp/yaml.h"

namespace config {
struct resolution {
    int32_t width;
    int32_t height;
};

struct interface {
    const char* name;
    int32_t top;
    int32_t left;
    int32_t width;
    int32_t height;
};

// TODO: fix configuration path, does not work
class Configuration {
public:
    explicit Configuration();
    explicit Configuration(const char* config_path, const char* name);
    ~Configuration();

    void setConfigurationPath(const char* config_path, const char* name);
    void readConfiguration();

    interface getInterfaceValue(const char* interface_prop);
    resolution getResolutionValue();

    template <typename TypeValue>
    auto getValue(const char* property) -> TypeValue;

private:
    YAML::Node m_config;
    const char* m_configPath = nullptr;
    const char* m_configName = nullptr;
};

template <typename TypeValue>
auto Configuration::getValue(const char* property) -> TypeValue
{
    return m_config[property].as<TypeValue>();
}

} // namespace config

#endif // !_HIBISCUS_CONFIGURATION_HPP_