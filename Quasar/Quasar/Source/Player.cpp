#include "stdafx.h"

#include "System.h"
#include "Player.h"

#include "graphics/ModelFactory.h"

void Player::Init() {
    pos = Vector3D(32, 32, 70);
    bb = BoundingBox(-0.4, -0.4, 0, 0.4, 0.4, 1.7);

    // Block picking outline.
    ModelFactory mf = ModelFactory();
    mf.shader = System::worldRenderer->shader;
    mf.topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    float d = 0.005;
    mf.vertices.Add(Vertex(Vector3F(-d, -d, -d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(1+d, -d, -d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(1+d, 1+d, -d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(0, 1+d, -d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(-d, -d, 1+d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(1, -d, 1+d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(1+d, 1+d, 1+d), XMFLOAT4(0, 0, 0, 1)));
    mf.vertices.Add(Vertex(Vector3F(-d, 1+d, 1+d), XMFLOAT4(0, 0, 0, 1)));
    mf.AddSegment(0, 1);
    mf.AddSegment(1, 2);
    mf.AddSegment(2, 3);
    mf.AddSegment(3, 0);
    mf.AddSegment(4, 5);
    mf.AddSegment(5, 6);
    mf.AddSegment(6, 7);
    mf.AddSegment(7, 4);
    mf.AddSegment(0, 4);
    mf.AddSegment(1, 5);
    mf.AddSegment(2, 6);
    mf.AddSegment(3, 7);
    selectedBlock = mf.Create();

    // Crappy cursor.
    ModelFactory mf2 = ModelFactory();
    mf2.shader = System::worldRenderer->shader;
    mf2.vertices.Add(Vertex(Vector3F(-0.002, -0.004, 0.1), XMFLOAT4(0, 1, 0, 0)));
    mf2.vertices.Add(Vertex(Vector3F(0, 0, 0.1), XMFLOAT4(0, 0.6, 0, 0.75)));
    mf2.vertices.Add(Vertex(Vector3F(0.002, -0.004, 0.1), XMFLOAT4(0, 1, 0, 0)));
    mf2.AddTriangle(0, 1, 2);
    cursor = mf2.Create();
}

void Player::Shutdown() {
}

void Player::Render() {
    if (picked) {
        selectedBlock->mat.set(XMMatrixTranslation(pickedBlock.x, pickedBlock.y, pickedBlock.z));
        selectedBlock->Render();
    }
    XMVECTOR det = XMMatrixDeterminant(System::graphics->viewMat.get());
    cursor->mat.set(XMMatrixInverse(&det, System::graphics->viewMat.get()));
    cursor->Render();
}

void Player::Tick() {
    DoMouseKeyboardInput();
    DoJump();

    UpdateVelocity();
    DoCollision();
    UpdatePosition();

    DoBlockPicking();

    counter++;
}

void Player::UpdateVelocity() {
    vel.z -= 0.0008;
    vel += kvec * (inAir ? 0.02 : 0.10);
    vel.x *= (inAir ? 0.99 : 0.94);
    vel.y *= (inAir ? 0.99 : 0.94);
    vel.z *= 0.999;
}

void Player::DoBlockPicking() {
    PickBlock();

    if (picked && !mouseStateLastTick) {
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

    mouseStateLastTick = System::input->MouseLeft() || System::input->MouseRight();

    if (counter > 50) {
        mouseStateLastTick = false;
    }
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

void Player::DoJump() {
    if (!inAir && System::input->KeyPressed(DIK_SPACE)) {
        vel.z += 0.05;
        inAir = true;
    }
}

void Player::DoCollision() {
    bool X = true, Y = true, Z = true;
    if (System::world->Intersects(bb.Offset(pos + vel.X()))) {
        vel.x = 0;
        X = false;
    }
    if (System::world->Intersects(bb.Offset(pos + vel.Y()))) {
        vel.y = 0;
        Y = false;
    }
    if (System::world->Intersects(bb.Offset(pos + vel.Z()))) {
        inAir = vel.z > 0;
        vel.z = 0;
        Z = false;
    } else {
        inAir = true;
    }

    if (X && Y && System::world->Intersects(bb.Offset(pos + vel.XY()))) {
        if (vel.x < vel.y) {
            vel.x = 0;
        } else {
            vel.y = 0;
        }
    }
    if (X && Z && System::world->Intersects(bb.Offset(pos + vel.XZ()))) {
        if (vel.x < vel.z) {
            vel.x = 0;
        } else {
            vel.z = 0;
        }
    }
    if (Y && Z && System::world->Intersects(bb.Offset(pos + vel.YZ()))) {
        if (vel.y < vel.z) {
            vel.y = 0;
        } else {
            vel.z = 0;
        }
    }
    if (X && Y && Z && System::world->Intersects(bb.Offset(pos + vel))) {
        if (vel.x < vel.y && vel.x < vel.z) {
            vel.x = 0;
        } else if (vel.y < vel.z) {
            vel.y = 0;
        } else {
            vel.z = 0;
        }
    }
}

void Player::DoMouseKeyboardInput() {
    yaw -= System::input->dx / 300.0;
    pitch -= System::input->dy / 300.0;

    if (pitch < -PI_2) {
        pitch = -PI_2;
    }
    if (pitch > PI_2) {
        pitch = PI_2;
    }

    dir = Vector3D(sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch)).Normalize();
    Vector3D kdir = Vector3D(sin(yaw), cos(yaw), 0).Normalize();
    kvec = kdir * (System::input->KeyPressed(DIK_W) - System::input->KeyPressed(DIK_S))
                    + Vector3D::AXIS_Z.Cross(kdir).Normalize() *
                    (System::input->KeyPressed(DIK_D) - System::input->KeyPressed(DIK_A))
                    + Vector3D::AXIS_Z * (System::input->KeyPressed(DIK_Q) - System::input->KeyPressed(DIK_E));

    kvec = kvec.Normalize(1.0 / (System::input->KeyPressed(DIK_LCONTROL) ? 200 : (System::input->KeyPressed(DIK_LSHIFT) ? 25 : 50)));
}