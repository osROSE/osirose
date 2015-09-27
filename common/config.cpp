#include "config.h"
#include "log.h"

YAML::Node RoseConfig::config;
bool RoseConfig::loaded = false;

YAML::Node &RoseConfig::get() {
	RoseConfig::ensure();
	return RoseConfig::config;
}

void RoseConfig::ensure() {
	if (!RoseConfig::loaded) {
		RoseConfig::loaded = true;
		try {
			RoseConfig::config = YAML::LoadFile("seven-hearts.yml");
			Log(msg_type::MSG_INFO, "Loaded configuration successfully");
		} catch (YAML::BadFile &e) {
			Log(msg_type::MSG_ERROR, "Could not load seven-hearts.yml!");
		}
	}
}
