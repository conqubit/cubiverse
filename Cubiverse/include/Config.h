#pragma once

struct Config {
	static const string Version;
	static const string Filename;

	struct Controls {
		static bool InvertMouse;
	};

	struct Key {
		static int Forward;
		static int Backward;
		static int Left;
		static int Right;
		static int NoClip;
		static int Up;
		static int Down;
		static int Run;
		static int Walk;
		static int Jump;
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