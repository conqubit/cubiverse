#include "stdafx.h"

#include "Config.h"

const string Config::Version = "0.0.4";
const string Config::Filename = "settings.txt";

// default values should be defined here

int Config::Graphics::MultiSampling = 0;
bool Config::Graphics::VSync = false;
double Config::Graphics::FOV = 80.0;
int Config::Graphics::FrameRateLimit = 240;

bool Config::Controls::InvertMouse = false;

int Config::Key::Forward = 'W';
int Config::Key::Backward = 'S';
int Config::Key::Left = 'A';
int Config::Key::Right = 'D';
int Config::Key::Up = 'Q';
int Config::Key::Down = 'E';
int Config::Key::NoClip = 'Z';
int Config::Key::Run = GLFW_KEY_LSHIFT;
int Config::Key::Walk = GLFW_KEY_LCTRL;
int Config::Key::Jump = GLFW_KEY_SPACE;