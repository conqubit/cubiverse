#include "stdafx.h"

#include "System.h"
#include "Player.h"

#include "graphics/ModelFactory.h"

Model* x0;
Model* x1;
Model* Y0;
Model* Y1;
Model* z0;
Model* z1;

void Player::Init() {
    pos = Vector3D(System::world->width.x / 2.0, System::world->width.y / 2.0, System::world->width.z / 2.0 + 16.0);
    bb = BoundingBox(-0.3, -0.3, 0, 0.3, 0.3, 1.7);

    // Block picking outline.
    ModelFactory mf = ModelFactory();
    mf.shader = System::worldRenderer->shader;
    mf.topology = GL_QUADS;
    mf.colorState = ColorF(0, 0, 0, 0.75);

    double d = 0.0025;

    mf.AddVertex(0 - d, 0 + d, 0 + d);
    mf.AddVertex(0 - d, 0 + d, 1 - d);
    mf.AddVertex(0 - d, 1 - d, 1 - d);
    mf.AddVertex(0 - d, 1 - d, 0 + d);
    x0 = mf.Create();

    mf.Clear();
    mf.AddVertex(1 + d, 1 - d, 0 + d);
    mf.AddVertex(1 + d, 1 - d, 1 - d);
    mf.AddVertex(1 + d, 0 + d, 1 - d);
    mf.AddVertex(1 + d, 0 + d, 0 + d);
    x1 = mf.Create();

    mf.Clear();
    mf.AddVertex(1 - d, 0 - d, 0 + d);
    mf.AddVertex(1 - d, 0 - d, 1 - d);
    mf.AddVertex(0 + d, 0 - d, 1 - d);
    mf.AddVertex(0 + d, 0 - d, 0 + d);
    Y0 = mf.Create();

    mf.Clear();
    mf.AddVertex(0 + d, 1 + d, 0 + d);
    mf.AddVertex(0 + d, 1 + d, 1 - d);
    mf.AddVertex(1 - d, 1 + d, 1 - d);
    mf.AddVertex(1 - d, 1 + d, 0 + d);
    Y1 = mf.Create();

    mf.Clear();
    mf.AddVertex(0 + d, 0 + d, 0 - d);
    mf.AddVertex(0 + d, 1 - d, 0 - d);
    mf.AddVertex(1 - d, 1 - d, 0 - d);
    mf.AddVertex(1 - d, 0 + d, 0 - d);
    z0 = mf.Create();

    mf.Clear();
    mf.AddVertex(1 - d, 0 + d, 1 + d);
    mf.AddVertex(1 - d, 1 - d, 1 + d);
    mf.AddVertex(0 + d, 1 - d, 1 + d);
    mf.AddVertex(0 + d, 0 + d, 1 + d);
    z1 = mf.Create();

    /*
    double p = 1 + d;
    double n = -d;
    mf.AddVertex(Vertex(Vector3F(n, n, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(p, n, n), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(p, n, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(p, p, n), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(p, p, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(n, p, n), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(n, p, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(n, n, n), ColorF(0, 0, 0)));
    //
    mf.AddVertex(Vertex(Vector3F(n, n, p), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(p, n, p), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(p, n, p), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(p, p, p), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(p, p, p), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(n, p, p), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(n, p, p), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(n, n, p), ColorF(0, 0, 0)));
    //
    mf.AddVertex(Vertex(Vector3F(n, n, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(n, n, p), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(p, n, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(p, n, p), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(p, p, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(p, p, p), ColorF(0, 0, 0)));

    mf.AddVertex(Vertex(Vector3F(n, p, n), ColorF(0, 0, 0)));
    mf.AddVertex(Vertex(Vector3F(n, p, p), ColorF(0, 0, 0)));
    */

    //selectedBlock = mf.Create();

    // Crappy cursor.
    mf = ModelFactory();
    mf.topology = GL_TRIANGLES;
    mf.shader = System::worldRenderer->shader;
    mf.AddVertex(Vector3F(-0.02, -0.04, 0), ColorF(0, 1, 0, 0));
    mf.AddVertex(Vector3F(0.02, -0.04, 0), ColorF(0, 1, 0, 0));
    mf.AddVertex(Vector3F(0, 0, 0), ColorF(0, 0.6, 0, 0.75));

    cursor = mf.Create();
    cursor->temp = true;
}

void Player::Shutdown() {
    cursor->Shutdown();
    x0->Shutdown();
    x1->Shutdown();
    Y0->Shutdown();
    Y1->Shutdown();
    z0->Shutdown();
    z1->Shutdown();
}

void Player::Render() {
    if (picked) {
        glLineWidth(2);
        glPolygonMode(GL_FRONT, GL_LINE);
        glm::mat4 m = glm::translate(glm::mat4(), glm::vec3(pickedBlock.x, pickedBlock.y, pickedBlock.z));
        if (side.x == -1) {
            x0->world = m;
            x0->Render();
        }
        if (side.x == 1) {
            x1->world = m;
            x1->Render();
        }
        if (side.y == -1) {
            Y0->world = m;
            Y0->Render();
        }
        if (side.y == 1) {
            Y1->world = m;
            Y1->Render();
        }
        if (side.z == -1) {
            z0->world = m;
            z0->Render();
        }
        if (side.z == 1) {
            z1->world = m;
            z1->Render();
        }
        glPolygonMode(GL_FRONT, GL_FILL);
        glLineWidth(1);
    }
    cursor->Render();
}

void Player::Tick() {
    DoInput();
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

    if (!System::focus || System::temp) {
        return;
    }

    if (picked && !mouseStateLastTick) {
        if (Input::MouseRight()) {
            int old = System::world->GetBlock(pickedBlock + side);
            System::world->SetBlock(pickedBlock + side, Block::Stone);
            if (BoundingBox::Block(pickedBlock + side).Intersects(bb.Offset(pos))) {
                System::world->SetBlock(pickedBlock + side, old);
            } else {
                System::worldRenderer->UpdateBlock(pickedBlock + side);
            }
        } else if (Input::MouseLeft()) {
            System::world->SetBlock(pickedBlock, Block::Air);
            System::worldRenderer->UpdateBlock(pickedBlock);
        }
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
            b.x += SIGN(dir.x);
            p += dir * xPlaneDist;
            side = -Vector3I::AXIS_X * SIGN(dir.x);
        }
        else if (yPlaneDist < zPlaneDist) {
            b.y += SIGN(dir.y);
            p += dir * yPlaneDist;
            side = -Vector3I::AXIS_Y * SIGN(dir.y);
        }
        else {
            b.z += SIGN(dir.z);
            p += dir * zPlaneDist;
            side = -Vector3I::AXIS_Z * SIGN(dir.z);
        }
    }
    picked = false;
    return;
}

void Player::DoJump() {
    if (!System::focus) {
        return;
    }
    if (!inAir && Input::KeyPressed(Key::Space)) {
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

double minPitch = -PI_2;
double maxPitch =  PI_2;

void Player::DoInput() {
    if (!System::focus) {
        return;
    }
    yaw += Input::DeltaMx() / 300.0;
    pitch -= Input::DeltaMy() / 300.0;

    if (pitch < minPitch) {
        pitch = minPitch;
    }
    if (pitch > maxPitch) {
        pitch = maxPitch;
    }

    dir = Vector3D(sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch)).Normalize();

    Vector3D kdir = Vector3D(sin(yaw), cos(yaw), 0).Normalize();

    Vector3D right = kdir.Cross(Vector3D::AXIS_Z).Normalize();

    up = right.Cross(dir);

    kvec = kdir * (Input::KeyPressed(Key::W) - Input::KeyPressed(Key::S))
           + right * (Input::KeyPressed(Key::D) - Input::KeyPressed(Key::A))
           + Vector3D::AXIS_Z * (Input::KeyPressed(Key::Q) - Input::KeyPressed(Key::E));

    kvec = kvec.Normalize(1.0 / (Input::KeyPressed(Key::LControl) ? 200 : (Input::KeyPressed(Key::LShift) ? 25 : 50)));
}