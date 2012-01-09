#include "stdafx.h"

#include "Config.h"

// Defaults should be defined here.

const string Config::Version = "0.0.4";
const string Config::Filename = "settings.txt";

int Config::Graphics::MultiSampling = 0;
int Config::Graphics::FrameRateLimit = 240;
bool Config::Graphics::VSync = false;

bool Config::Controls::InvertMouse = false;

char Config::Key::Forward = 'W';
char Config::Key::Backward = 'S';
char Config::Key::Left = 'A';
char Config::Key::Right = 'D';
char Config::Key::Up = 'Q';
char Config::Key::Down = 'E';
char Config::Key::NoClip = 'Z';

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
	if (Util::String::EqualsIgnoreCase(str, "false") ||
		Util::String::EqualsIgnoreCase(str, "disabled") ) {
		return false;
	}
	if (Util::String::EqualsIgnoreCase(str, "true") ||
		Util::String::EqualsIgnoreCase(str, "enabled") ) {
		return true;
	}
	return false;
}

template <>
static char GetValue(const string& str) {
	return toupper(str[0]);
}

template <typename T>
static void SetVariable(T& variable, const string& val) {
	variable = GetValue<T>(val);
}

template <typename T>
static void SetProperty(const string& name, T& variable) {
	string s = Util::String::ToLowerCopy(name);
	if (props.count(s) == 1) {
		SetVariable(variable, props[s]);
	} else {
		file << name << " = " << GetString(variable) << '\n';
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

	char c;
	while (file.get(c), file.good()) {
		// shoot to the end of line or file if comment
		if (c == '#') {
			while (file.get(c), file.good() && c != '\n');
		}
		if (c == '\n') {
			switch (state) {
			case ReadingName:
				prop.Reset();
				continue;
			case ReadingValue:
				return true;
			}
		}
		switch (state) {
		case ReadingName:
			if (c == '=') {
				state = ReadingValue;
			} else {
				prop.name += c;
			}
			break;
		case ReadingValue:
			prop.value += c;
			break;
		}
	}

	switch (state) {
	case ReadingName:
		return false;
	case ReadingValue:
		return true;
	}
}

void Config::LoadConfigFile() {
	file.open(Filename, std::ios::in | std::ios::out | std::ios::app | std::ios::ate);

	if ((int)file.tellg() == 0) {
		file << "# Cubiverse configuration file\n";
		file << '\n';
	}

	file.seekg(0);

	Config::Property prop;
	while (ReadNextProperty(prop)) {
		Util::String::ToLower(prop.name);
		Util::String::Trim(prop.name);
		Util::String::Trim(prop.value);
		if (prop.name.size() == 0) continue;
		props.insert(std::make_pair(prop.name, prop.value));
	}

	file.clear();

	file.seekg(-1, std::ios::end);
	char lastChar = file.peek();
	file.seekp(0);

	if (lastChar != '\n') {
		file.put('\n');
	}

	SetProperty("Graphics.MultiSampling", Graphics::MultiSampling);
	SetProperty("Graphics.FrameRateLimit", Graphics::FrameRateLimit);
	SetProperty("Graphics.VSync", Graphics::VSync);

	SetProperty("Controls.InvertMouse", Controls::InvertMouse);

	SetProperty("Key.Forward", Key::Forward);
	SetProperty("Key.Left", Key::Left);
	SetProperty("Key.Backward", Key::Backward);
	SetProperty("Key.Right", Key::Right);
	SetProperty("Key.Up", Key::Up);
	SetProperty("Key.Down", Key::Down);
	SetProperty("Key.NoClip", Key::NoClip);

	file.close();

	props.clear();
}