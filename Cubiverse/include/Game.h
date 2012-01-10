#pragma once

#include "player/Player.h"

class World;
class WorldRenderer;

class Game {
public:
	static const int TickMS = 5;

	static Player*		  player;
	static World*		   world;
	static WorldRenderer*   worldRenderer;

	static bool Init();
	static void Start();
	static void Shutdown();

	static void Update();
	static void Tick();

	static void Render();
	static void DrawGUI();
	static void KeyCallback(int key, int action);
};