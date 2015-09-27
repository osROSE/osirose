#ifndef COMMON_CONFIG_H__
#define COMMON_CONFIG_H__
#pragma once
/*
	iROSE Configuration Reduex
	by Qix

	Because the last config system has been bugging me since I was about 14.

	Sorry, but you *will* need to update your config files. New format is YAML.

	This class is NOT written how I would normally write code; it is written
	in such a way as to easily replace the existing config code.
*/

#include <yaml-cpp/yaml.h>

#define Cnf RoseConfig::get()

class RoseConfig {
public:
	static YAML::Node &get();

private:
	static void ensure();

	static bool loaded;
	static YAML::Node config;
};


#endif
