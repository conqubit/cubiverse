#include "stdafx.h"

#include "Config.h"

struct Property {
	string name;
	string value;
		
	void Reset() {
		name.clear();
		value.clear();
	}
};

static std::fstream file;
static std::map<string, string> props;

template <typename T>
static string EncodeDefault(T val) {
	return str(val);
}

template <typename T>
static void DecodeDefault(const string& str, T& var) {
	std::istringstream ss(str);
	ss >> var;
}

static void DecodeUnsignedInt(const string& str, int& var) {
	DecodeDefault(str, var);
	if (var < 0) {
		var = 0;
	}
}

static string EncodeBool(bool val) {
	if (val) {
		return "true";
	} else {
		return "false";
	}
}

static void DecodeBool(const string& str, bool& var) {
	string s = Util::String::ToLowerCopy(str);
	if (str == "false" || str == "disabled" || str == "disable") {
		var = false;
	}
	else if (str == "true" || str == "enabled" || str == "enable") {
		var = true;
	}
}

static string EncodeKey(int key) {
	switch (key) {
	case GLFW_KEY_SPACE: return "SPACE";
	case GLFW_KEY_LCTRL: return "CTRL";
	case GLFW_KEY_LSHIFT: return "SHIFT";
	case GLFW_KEY_ENTER: return "ENTER";
	case GLFW_KEY_LALT: return "ALT";
	case GLFW_KEY_BACKSPACE: return "BACKSPACE";
	case GLFW_KEY_TAB: return "TAB";
	}

	return string(1, toupper(key));
}

static void DecodeKey(const string& str, int& var) {
	if (str.size() == 1) {
		var = toupper(str[0]);
		return;
	}

	string s = Util::String::ToUpperCopy(str);

	if (s == "SPACE") {
		var = GLFW_KEY_SPACE;
	}
	else if (s == "CTRL") {
		var = GLFW_KEY_LCTRL;
	}
	else if (s == "SHIFT") {
		var = GLFW_KEY_LSHIFT;
	}
	else if (s == "ENTER") {
		var = GLFW_KEY_ENTER;
	}
	else if (s == "ALT" || s == "ALTERNATE") {
		var = GLFW_KEY_LALT;
	}
	else if (s == "BACKSPACE" || s == "BACK SPACE" || s == "BACK") {
		var = GLFW_KEY_BACKSPACE;
	}
	else if (s == "TAB") {
		var = GLFW_KEY_TAB;
	}
}

static string comment;

static void Comment(const string& str) {
	comment = str;
}

template <typename T>
static void ProcessProperty(const string& name, T& variable,
							void (*Decode)(const string&, T&) = DecodeDefault<T>,
							string (*Encode)(T) = EncodeDefault<T>) {
	string s = Util::String::ToLowerCopy(name);
	if (props.count(s) == 1) {
		Decode(props[s], variable);
	} else {
		file << comment;
		file << name << " = " << Encode(variable) << '\n';
	}
	comment.clear();
}

static bool ReadNextProperty(Property& prop) {
	static enum {
		ReadingName, ReadingValue
	};

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
		file << "# Delete or clear this file for defaults\n";
		file << '\n';
	}

	file.seekg(0);

	Property prop;
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

	ProcessProperty("Graphics.MultiSampling", Graphics::MultiSampling, DecodeUnsignedInt);
	ProcessProperty("Graphics.VSync", Graphics::VSync, DecodeBool, EncodeBool);
	Comment("\n# Set to 0 for no limiting\n");
	ProcessProperty("Graphics.FrameRateLimit", Graphics::FrameRateLimit, DecodeUnsignedInt);

	Comment("\n");
	ProcessProperty("Controls.InvertMouse", Controls::InvertMouse, DecodeBool, EncodeBool);

	Comment("\n");
	ProcessProperty("Key.Forward", Key::Forward, DecodeKey, EncodeKey);
	ProcessProperty("Key.Left", Key::Left, DecodeKey, EncodeKey);
	ProcessProperty("Key.Backward", Key::Backward, DecodeKey, EncodeKey);
	ProcessProperty("Key.Right", Key::Right, DecodeKey, EncodeKey);
	ProcessProperty("Key.Up", Key::Up, DecodeKey, EncodeKey);
	ProcessProperty("Key.Down", Key::Down, DecodeKey, EncodeKey);
	ProcessProperty("Key.NoClip", Key::NoClip, DecodeKey, EncodeKey);
	ProcessProperty("Key.Run", Key::Run, DecodeKey, EncodeKey);
	ProcessProperty("Key.Walk", Key::Walk, DecodeKey, EncodeKey);
	ProcessProperty("Key.Jump", Key::Jump, DecodeKey, EncodeKey);

	file.close();

	props.clear();
}