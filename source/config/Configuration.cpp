#include "Configuration.hpp"
#include "logger/Logger.hpp"

#include <exception>

namespace sl::config
{
	Configuration::Configuration()
	{
	}

	Configuration::Configuration(const char* config_path, const char* name)
		: m_configPath{ config_path }
		, m_configName{ name }
	{
	}

	Configuration::~Configuration()
	{
	}

	void Configuration::setConfigurationPath(const char* config_path, const char* name)
	{
		m_configPath = config_path;
		m_configName = name;
	}

	void Configuration::readConfiguration()
	{
		/*
		std::string configPath{m_configPath};
		if (configPath[configPath.size() - 1] != '/')
		{
			configPath.append("/");
		}
		configPath.append(m_configName);
		*/

		YAML::Node node = YAML::LoadFile(m_configName);
		if (!node["starline"])
		{
			throw std::exception("There is no property starline in configuration file");
		}
		m_config = node["starline"];
	}

	interface Configuration::getInterfaceValue(const char* interface_prop)
	{
		const size_t sizeInterface = 4u;
		auto iface = m_config["interface"];
		for (auto i = 0u; i < sizeInterface; ++i)
		{
			auto name = iface[i]["name"].as<std::string>();
			if (name == interface_prop)
			{
				return interface {
					interface_prop,
					iface[i]["top"].as<int32_t>(),
					iface[i]["left"].as<int32_t>(),
					iface[i]["width"].as<int32_t>(),
					iface[i]["height"].as<int32_t>()
				};
			}
		}
		return interface{};
	}

	resolution Configuration::getResolutionValue()
	{
		return resolution{ 
			m_config["resolution"]["width"].as<int32_t>(), 
			m_config["resolution"]["height"].as<int32_t>() 
		};
	}

} // namespace sl::config