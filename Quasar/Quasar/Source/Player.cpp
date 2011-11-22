#include "stdafx.h"

#include "System.h"
#include "Player.h"

Player::Player() :
pos(), vel(), dir(),
yaw(), pitch(),
inAir(), 
pickedBlock(), picked(), lastMouse(),
counter() {
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
    PickBlock();

    if (picked && !lastMouse) {
        if (System::input->MouseRight()) {
            int old = System::world->GetBlock(pickedBlock + side);
            System::world->SetBlock(pickedBlock + side, Block::Stone);
            if (System::world->Intersects(bb.Offset(pos))) {
                System::world->SetBlock(pickedBlock + side, old);
            } else {
                System::worldRenderer->UpdateBlock(pickedBlock + side);
            }
        } else if (System::input->MouseLeft()) {
            System::world->SetBlock(pickedBlock, Block::Air);
            System::worldRenderer->UpdateBlock(pickedBlock);
        }
        counter = 0;
    }

    lastMouse = System::input->MouseLeft() || System::input->MouseRight();

    if (counter > 50) {
        lastMouse = false;
    }

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
        inAir = vel.z > 0;
        vel.z = 0;
    } else {
        inAir = true;
    }
    pos += vel;
    counter++;
}

void Player::PickBlock() {
    Vector3D p = Eye();
    Vector3I b = p.Floor();
    while ((Eye() - p).LengthSquared() < 5 * 5) {
        if (System::world->GetBlock(b) == Block::Stone) {
            pickedBlock = b;
            picked = true;
            return;
        }
        double xPlaneDist = abs((p.x - (b.x + (dir.x > 0 ? 1 : 0))) / dir.x);
        double yPlaneDist = abs((p.y - (b.y + (dir.y > 0 ? 1 : 0))) / dir.y);
        double zPlaneDist = abs((p.z - (b.z + (dir.z > 0 ? 1 : 0))) / dir.z);
        if (xPlaneDist < yPlaneDist && xPlaneDist < zPlaneDist) {
            b.x += sign(dir.x);
            p += dir * xPlaneDist;
            side = -Vector3I::AXIS_X * sign(dir.x);
        }
        else if (yPlaneDist < zPlaneDist) {
            b.y += sign(dir.y);
            p += dir * yPlaneDist;
            side = -Vector3I::AXIS_Y * sign(dir.y);
        }
        else {
            b.z += sign(dir.z);
            p += dir * zPlaneDist;
            side = -Vector3I::AXIS_Z * sign(dir.z);
        }
    }
    picked = false;
    return;
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