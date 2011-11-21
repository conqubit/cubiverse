#include "stdafx.h"

#include "System.h"
#include "Player.h"

Player::Player() : pos(), vel(), dir(), yaw(), pitch(), inAir() {
}

Player::~Player() {
}

void Player::Init() {
    pos = Vector3D(32, 32, 70);
    bb = BoundingBox(-0.4, -0.4, 0, 0.4, 0.4, 1.7);
}

void Player::Shutdown() {
}

void Player::Tick() {
    if (!inAir && System::input->KeyPressed(DIK_SPACE)) {
        vel.z += 0.05;
        inAir = true;
    }
    vel.z -= 0.0008;
    vel += MovementVector() * (inAir ? 0.02 : 0.15);
    vel.x *= (inAir ? 0.99 : 0.9);
    vel.y *= (inAir ? 0.99 : 0.9);
    vel.z *= 0.999;
    if (System::world->Intersects(bb.Offset(pos + vel.OnlyX()))) {
        vel.x = 0;
    }
    if (System::world->Intersects(bb.Offset(pos + vel.OnlyY()))) {
        vel.y = 0;
    }
    if (System::world->Intersects(bb.Offset(pos + vel.OnlyZ()))) {
        vel.z = 0;
        inAir = false;
    } else {
        inAir = true;
    }
    pos += vel;
}

Vector3D Player::MovementVector() {
    yaw -= System::input->dx / 300.0;
    pitch -= System::input->dy / 300.0;

    if (pitch < -PI_2 + 0.01) {
        pitch = -PI_2 + 0.01;
    }
    if (pitch > PI_2 - 0.01) {
        pitch = PI_2 - 0.01;
    }

    dir = Vector3D(sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch)).Normalize();
    Vector3D kdir = Vector3D(sin(yaw), cos(yaw), 0).Normalize();
    Vector3D kvec = kdir * (System::input->KeyPressed(DIK_W) - System::input->KeyPressed(DIK_S))
                    + Vector3D::AXIS_Z.Cross(kdir).Normalize() *
                    (System::input->KeyPressed(DIK_D) - System::input->KeyPressed(DIK_A))
                    + Vector3D::AXIS_Z * (System::input->KeyPressed(DIK_Q) - System::input->KeyPressed(DIK_E));

    return kvec.Normalize(1.0 / (System::input->KeyPressed(DIK_LCONTROL) ? 200 : (System::input->KeyPressed(DIK_LSHIFT) ? 25 : 50)));
}

XMMATRIX Player::GetMat() {
    return XMMatrixLookToLH((pos + Vector3D(0, 0, 5.0 / 3.0)).ToXMVector(), dir.ToXMVector(), Vector3F::AXIS_Z.ToXMVector());
}