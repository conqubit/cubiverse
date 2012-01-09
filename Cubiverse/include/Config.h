#pragma once

struct Config {
	static const string Version;
	static const string Filename;

	struct Controls {
		static bool InvertMouse;
	};

	struct Key {
		static char Forward;
		static char Backward;
		static char Left;
		static char Right;
		static char NoClip;
		static char Up;
		static char Down;
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