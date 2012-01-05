#pragma once

struct Config {
	static const string Version;
	static const string File;

	struct Controls {
		static bool InvertMouse;
	};

	struct Graphics {
		static int MultiSampling;
		static int FrameRateLimit;
		static bool VSync;
	};

	struct Property {
		string name;
		string value;
		
		void Reset() {
			name.clear();
			value.clear();
		}
	};

	static void LoadConfigFile();
};