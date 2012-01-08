#include "stdafx.h"

#include "Window.h"
#include "Game.h"
#include "Input.h"
#include "world/World.h"
#include "graphics/WorldRenderer.h"

#include "player/Player.h"

#include "graphics/ModelFactory.h"
#include "player/Crosshair.h"
#include "player/BlockPickingOutline.h"

double eyeOffset = 1.7 - PHI;

Model* wireframe;

/*
#include <btBulletDynamicsCommon.h>

btCollisionShape* blockShape;
btRigidBody::btRigidBodyConstructionInfo* blockBodyInfo;

btRigidBody* body;
btMotionState* playerMotionState;
btDynamicsWorld* dynamicsWorld;*/


Crosshair crosshair;
PickOutline pickoutline;

void Player::Init() {
	bb = BoundingBoxD(-0.3, -0.3, 0, 0.3, 0.3, 1.7);
	height = 1.7;
	eyeHeight = height - eyeOffset;

	/*btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
 
	btCollisionShape* playerShape = new btCylinderShapeZ(btVector3(0.3, 0.3, height));
	playerShape->calculateLocalInertia(1.0, btVector3(0, 0, 0));

	playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(pos.x, pos.y, pos.z)));

	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(1.0, playerMotionState, playerShape);

	body = new btRigidBody(playerRigidBodyCI);

	body->setAngularFactor(0);

	dynamicsWorld->addRigidBody(body);*/
}

void Player::InitGraphics() {
	crosshair.InitGraphics();
	pickoutline.InitGraphics();

	wireframe = ModelFactory::CreateWireframeDebugBox(bb, ColorF(1, 0, 0, 1));
}

void Player::ShutdownGraphics() {
	crosshair.ShutdownGraphics();
	pickoutline.ShutdownGraphics();

	wireframe->Shutdown();
}

void Player::Shutdown() {
	ShutdownGraphics();
	delete this;
}

void Player::Render() {
	if (picked) {
		pickoutline.pos = pickedBlock;
		pickoutline.side = side;
		pickoutline.Render();
	}

	//glLineWidth(4);
	//wireframe->world = glm::translate(glm::mat4(), pos.ToGlmVec3()) * glm::mat4(orientation);
	//wireframe->Render();
}

void Player::DrawGUI() {
	crosshair.Render();
}

void Player::Tick() {
	DoInput();

	UpdateVelocity();
	DoCollision();
	UpdatePosition();

	DoJump();

	DoOrient();

	DoBlockPicking();

	counter += 1;
}

void Player::DoOrient() {
	//if (noclip) return;
	Vector3I newUp = Game::world->GetUp(pos);

	if (playerUp != newUp) {
		glm::dmat4 oldOrientation = orientation;

		Vector3I rotationAxis = playerUp.Cross(newUp);
		glm::dmat4 rot = glm::gtc::matrix_transform::rotate(glm::dmat4(), 90.0, glm::dvec3(rotationAxis.ToGlmVec3()));
		orientation = rot * orientation;

		if (Game::world->Intersects(BBox())) {
			orientation = oldOrientation;
		} else {
			playerUp = newUp;
		}
	}

	Vector3D newSmoothUp = Game::world->GetUpSmooth(pos);
	double angle = smoothUp.DegreesTo(newSmoothUp);
	if (abs(angle) > 0.001) {
		//glm::dmat4 oldOrientation = smoothOrientation;

		Vector3D rotationAxis = smoothUp.Cross(newSmoothUp);
		glm::dmat4 rot = glm::gtc::matrix_transform::rotate(glm::dmat4(), angle, glm::dvec3(rotationAxis.ToGlmVec3()));

		smoothOrientation = rot * smoothOrientation;
		smoothUp = newSmoothUp;
	}
}

void Player::UpdateVelocity() {
	if (!noclip) {
		if (inAir && Game::world->GetUp(pos) == playerUp) {
			vel -= (Game::world->GetUpSmooth(pos).ToDouble() * 0.0008);
		} else {
			vel -= (Game::world->GetUp(pos).ToDouble() * 0.0008);
		}
	}

	vel += ToWorldSmooth(kvec * (inAir && !noclip ? 0.015 : 0.10));

	Vector3D vel = FromWorld(this->vel);

	vel.x *= (inAir && !noclip ? 0.99 : 0.94);
	vel.y *= (inAir && !noclip ? 0.99 : 0.94);
	vel.z *= noclip ? 0.95 : 0.999;

	this->vel = ToWorld(vel);
}

void Player::DoBlockPicking() {
	PickBlock();

	if (!Window::HasFocus()) {
		return;
	}

	if (picked && !mouseStateLastTick) {
		if (Input::MouseRight()) {
			int old = Game::world->GetBlock(pickedBlock + side);
			Game::world->SetBlock(pickedBlock + side, Block::Test);
			if (BoundingBoxD::Block(pickedBlock + side).Intersects(BBox())) {
				Game::world->SetBlock(pickedBlock + side, old);
			} else {
				Game::worldRenderer->UpdateBlock(pickedBlock + side);
			}
		} else if (Input::MouseLeft()) {
			Game::world->SetBlock(pickedBlock, Block::Air);
			Game::worldRenderer->UpdateBlock(pickedBlock);
		}
		PickBlock();
		counter = 0;
	}

	mouseStateLastTick = Input::MouseLeft() || Input::MouseRight();

	if (counter > 50) {
		mouseStateLastTick = false;
	}
}

void Player::PickBlock() {
	Vector3D p = Eye();
	Vector3I b = p.Floor();
	Vector3D dir = ToWorldSmooth(this->dir);
	while ((Eye() - p).LengthSquared() < 5 * 5) {
		if (Block::Solid(Game::world->GetBlock(b))) {
			pickedBlock = b;
			picked = true;
			return;
		}
		double xPlaneDist = abs((p.x - (b.x + (dir.x > 0 ? 1 : 0))) / dir.x);
		double yPlaneDist = abs((p.y - (b.y + (dir.y > 0 ? 1 : 0))) / dir.y);
		double zPlaneDist = abs((p.z - (b.z + (dir.z > 0 ? 1 : 0))) / dir.z);
		if (xPlaneDist < yPlaneDist && xPlaneDist < zPlaneDist) {
			b.x += SIGN(dir.x);
			p += dir * xPlaneDist;
			side = -Vector3I::AxisX * SIGN(dir.x);
		}
		else if (yPlaneDist < zPlaneDist) {
			b.y += SIGN(dir.y);
			p += dir * yPlaneDist;
			side = -Vector3I::AxisY * SIGN(dir.y);
		}
		else {
			b.z += SIGN(dir.z);
			p += dir * zPlaneDist;
			side = -Vector3I::AxisZ * SIGN(dir.z);
		}
	}
	picked = false;
	return;
}

void Player::DoJump() {
	if (noclip) return;
	if (!inAir && Input::KeyPressed(DIK_SPACE)) {
		vel += ToWorldSmooth(Vector3D(0, 0, .05));
		inAir = true;
	}
}

void Player::DoCollision() {
	if (noclip) return;
	Vector3I up = playerUp;//System::world->GetUp(pos);
	bool X, Y, Z;
	X = Y = Z = true;

	Vector3D vel = FromWorld(this->vel);

	if (Game::world->Intersects(BBox().Offset(ToWorld(vel.X())))) {
		vel.x = 0;
		X = false;
	}
	if (Game::world->Intersects(BBox().Offset(ToWorld(vel.Y())))) {
		vel.y = 0;
		Y = false;
	}
	if (Game::world->Intersects(BBox().Offset(ToWorld(vel.Z())))) {
		inAir = vel.z > 0;
		vel.z = 0;
		Z = false;
	} else {
		inAir = true;
	}

	Vector3D avel = vel.Abs();


	if (X && Y && Z && Game::world->Intersects(BBox().Offset(this->vel))) {
		if (avel.x < avel.y && avel.x < avel.z) {
			vel.x = 0;
		} else if (avel.y < avel.z) {
			vel.y = 0;
		} else {
			vel.z = 0;
		}
	} else if (X && Y && Game::world->Intersects(BBox().Offset(ToWorld(vel.XY())))) {
		if (avel.x < avel.y) {
			vel.x = 0;
		} else {
			vel.y = 0;
		}
	} else if (X && Z && Game::world->Intersects(BBox().Offset(ToWorld(vel.XZ())))) {
		if (avel.x < avel.z) {
			vel.x = 0;
		} else {
			vel.z = 0;
		}
	} else if (Y && Z && Game::world->Intersects(BBox().Offset(ToWorld(vel.YZ())))) {
		if (avel.y < avel.z) {
			vel.y = 0;
		} else {
			vel.z = 0;
		}
	}

	this->vel = ToWorld(vel);
}

double minPitch = -PI_2;
double maxPitch =  PI_2;

void Player::DoInput() {
	yaw += Input::DeltaMx() / 300.0;
	pitch -= Input::DeltaMy() / 300.0 * (Config::Controls::InvertMouse ? -1 : 1);

	if (pitch < minPitch) {
		pitch = minPitch;
	}
	if (pitch > maxPitch) {
		pitch = maxPitch;
	}

	dir = Vector3D(sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch)).Normalize();

	Vector3D kdir = Vector3D(sin(yaw), cos(yaw), 0).Normalize();

	Vector3D right = kdir.Cross(Vector3D::AxisZ).Normalize();

	cameraUp = right.Cross(dir);

	kvec = kdir * (Input::KeyPressed(DIK_W) - Input::KeyPressed(DIK_S))
		   + right * (Input::KeyPressed(DIK_D) - Input::KeyPressed(DIK_A));

	if (noclip) {
		kvec += Vector3D::AxisZ * ((Input::KeyPressed(DIK_Q) || Input::KeyPressed(DIK_SPACE)) - Input::KeyPressed(DIK_E));
	}

	kvec = kvec.Normalize(1.0 / (Input::KeyPressed(DIK_LCONTROL) ? (noclip ? 300.0 : 200.0) : (Input::KeyPressed(DIK_LSHIFT) ? (noclip ? 5.0 : 25.0) : (noclip ? 35.0 : 50.0))));
}

Vector3D Player::Eye() {
	return pos + ToWorldSmooth(Vector3D(0, 0, eyeHeight));
}