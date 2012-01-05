#include "stdafx.h"

#include "Config.h"

// Defaults should be defined here.

const string Config::Version = "0.0.3";
const string Config::File = "cubiverse.cfg";

bool Config::Controls::InvertMouse = false;

int Config::Graphics::MultiSampling = 0;
int Config::Graphics::FrameRateLimit = 240;
bool Config::Graphics::VSync = false;



static std::fstream file;
static std::map<string, string> props;

template <typename T>
static string GetString(const T& val) {
	return str(val);
}

template <>
static string GetString(const bool& val) {
	if (val) {
		return "true";
	} else {
		return "false";
	}
}

template <typename T>
static T GetValue(const string& str) {
	std::istringstream ss(str);
	T ret;
	ss >> ret;
	return ret;
}

template <>
static bool GetValue(const string& str) {
	if (str == "false" || str == "disabled") {
		return false;
	}
	if (str == "true" || str == "enabled") {
		return true;
	}
}


template <typename T>
static void SetVariable(T& variable, const string& val) {
	variable = GetValue<T>(val);
}

template <typename T>
static void SetProperty(const string& name, T& variable) {
	if (props.count(name) == 1) {
		SetVariable(variable, props[name]);
	} else {
		//if (file.peek() != '\n') {
		//	file << std::endl;
		//}
		file << name << '=' << GetString(variable) << std::endl;
	}
}

static enum {
	ReadingName, ReadingValue
};

static bool ReadNextProperty(Config::Property& prop) {
	if (!file.good()) {
		return false;
	}

	prop.Reset();
	int state = ReadingName;

	bool escape = false;

	char c;
	while (file.get(c), !file.eof()) {
		if (c == '#' || c == '\n') {
			// shoot to the end of line or file if comment
			if (c == '#') {
				while (file.get(c), file.good() && c != '\n');
			}
			if (state == ReadingValue) {
				return true;
			} else if (file.good()) {
				prop.Reset();
				state = ReadingName;
				continue;
			} else {
				return false;
			}
		}
		switch (state) {
		case ReadingName:
			if (!isspace(c)) {
				if (c == '=') {
					state = ReadingValue;
				} else {
					prop.name += tolower(c);
				}
			}
			break;
		case ReadingValue:
			prop.value += c;
			break;
		}
	}

	if (state == ReadingValue) {
		return true;
	} else {
		return false;
	}
}

void Config::LoadConfigFile() {
	file.open(File, std::fstream::in | std::fstream::out | std::fstream::app);

	file.seekg(0, std::fstream::end);  
	if ((int)file.tellg() == 0) {
		file << "# Cubiverse configuration file" << std::endl << std::endl;
	}
	file.seekg(0, std::fstream::beg);  

	Config::Property prop;
	while (ReadNextProperty(prop)) {
		if (prop.name.size() == 0) continue;
		props.insert(std::make_pair(prop.name, prop.value));
	}

	file.close();
	file.open(File, std::fstream::in | std::fstream::out | std::fstream::app);

	SetProperty("controls.invertmouse", Controls::InvertMouse);
	SetProperty("graphics.multisampling", Graphics::MultiSampling);
	SetProperty("graphics.frameratelimit", Graphics::FrameRateLimit);
	SetProperty("graphics.vsync", Graphics::VSync);

	props.clear();
	file.close();
}