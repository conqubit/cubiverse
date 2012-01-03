#pragma once

class Entity {
public:
	int ID;

	virtual ~Entity() {
	}

	virtual Vector3D Pos() = 0;
	virtual Vector3D Vel() = 0;

protected:
	Entity() : ID(0) {
	}
};