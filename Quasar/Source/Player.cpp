#include "stdafx.h"

#include "System.h"
#include "Window.h"
#include "Player.h"

#include "graphics/ModelFactory.h"

Model* x0;
Model* x1;
Model* Y0;
Model* Y1;
Model* z0;
Model* z1;

double eyeOffset = 1.7 - PHI;

Shader* shader;

void Player::Init(Vector3D p) {

    orientation[0] = glm::vec4(1, 0, 0, 0);
    orientation[1] = glm::vec4(0, 1, 0, 0);
    orientation[2] = glm::vec4(0, 0, 1, 0);

    playerUp = Vector3I(0, 0, 1);

    pos = p;

    bb = BoundingBox(-0.3, -0.3, 0, 0.3, 0.3, 1.7);
    height = 1.7;
    eyeHeight = height - eyeOffset;

    shader = new Shader();
    shader->Init("res/color.c.v.glsl", "res/color.c.f.glsl");

    // Block picking outline.
    ModelFactory mf = ModelFactory();
    mf.AddAttribute("position", 3);
    mf.AddAttribute("color", 4);

    mf.shader = shader;
    mf.topology = GL_LINE_LOOP;
    ColorF c(0, 0, 0, 0.5);

    double d = 0.0025;

    mf.Next().Set("position", 0 - d, 1 - d, 0 + d).Set("color", c);
    mf.Next().Set("position", 0 - d, 1 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 - d, 0 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 - d, 0 + d, 0 + d).Set("color", c);
    x0 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 1 + d, 0 + d, 0 + d).Set("color", c);
    mf.Next().Set("position", 1 + d, 0 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 + d, 1 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 + d, 1 - d, 0 + d).Set("color", c);
    x1 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 0 + d, 0 - d, 0 + d).Set("color", c);
    mf.Next().Set("position", 0 + d, 0 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 - d, 0 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 - d, 0 - d, 0 + d).Set("color", c);
    Y0 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 1 - d, 1 + d, 0 + d).Set("color", c);
    mf.Next().Set("position", 1 - d, 1 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 + d, 0 + d).Set("color", c);
    Y1 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 1 - d, 0 + d, 0 - d).Set("color", c);
    mf.Next().Set("position", 1 - d, 1 - d, 0 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 - d, 0 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 0 + d, 0 - d).Set("color", c);
    z0 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 0 + d, 0 + d, 1 + d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 - d, 1 + d).Set("color", c);
    mf.Next().Set("position", 1 - d, 1 - d, 1 + d).Set("color", c);
    mf.Next().Set("position", 1 - d, 0 + d, 1 + d).Set("color", c);
    z1 = mf.Create();

    // Crappy cursor.
    mf.Clear();
    mf.topology = GL_LINES;

    mf.Next().Set("position", 0, 0, 0).Set("color", 1, 1, 1, 1);
    mf.Next().Set("position", -0.02, -0.02, 0).Set("color", 1, 1, 1, 1);
    mf.Next().Set("position", 0, 0, 0).Set("color", 1, 1, 1, 1);
    mf.Next().Set("position", 0.02, -0.02, 0).Set("color", 1, 1, 1, 1);
    
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
        glLineWidth(3);
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
    }
    glLineWidth(2);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
    cursor->Render();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1);
}

void Player::Tick() {
    DoInput();

    UpdateVelocity();
    DoCollision();
    UpdatePosition();

    DoJump();

    DoOrient();

    DoBlockPicking();

    counter++;
}

void Player::DoOrient() {
    Vector3I newUp = System::world->GetUp(pos + ToWorld(Vector3D(0, 0, 0.3)));
    if (playerUp == newUp) {
        return;
    }

    glm::dmat4 oldOrientation = orientation;

    Vector3I rotationAxis = playerUp.Cross(newUp);
    glm::dmat4 rot = glm::gtc::matrix_transform::rotate(glm::dmat4(), 90.0, glm::dvec3(rotationAxis.ToGlmVec3()));
    orientation = rot * orientation;

    if (System::world->Intersects(BBox())) {
        orientation = oldOrientation;
    } else {
        playerUp = newUp;
    }
}

void Player::UpdateVelocity() {
    if (!noclip) {
        vel -= System::world->GetUp(pos + ToWorld(Vector3D(0, 0, 0.3))).ToDouble() * 0.0008;
    }

    Vector3D vel = FromWorld(this->vel);

    vel += kvec * (inAir && !noclip ? 0.02 : 0.10);
    vel.x *= (inAir && !noclip ? 0.99 : 0.94);
    vel.y *= (inAir && !noclip ? 0.99 : 0.94);
    vel.z *= noclip ? 0.95 : 0.999;

    this->vel = ToWorld(vel);
}

void Player::DoBlockPicking() {
    PickBlock();

    if (!Window::focus || Window::temp) {
        return;
    }

    if (picked && !mouseStateLastTick) {
        if (Input::MouseRight()) {
            int old = System::world->GetBlock(pickedBlock + side);
            System::world->SetBlock(pickedBlock + side, Block::Test);
            if (BoundingBox::Block(pickedBlock + side).Intersects(BBox())) {
                System::world->SetBlock(pickedBlock + side, old);
            } else {
                System::worldRenderer->UpdateBlock(pickedBlock + side);
            }
        } else if (Input::MouseLeft()) {
            System::world->SetBlock(pickedBlock, Block::Air);
            System::worldRenderer->UpdateBlock(pickedBlock);
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
    Vector3D dir = ToWorld(this->dir);
    while ((Eye() - p).LengthSquared() < 5 * 5) {
        if (System::world->GetBlock(b) != Block::Air) {
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
    if (noclip) return;
    if (!inAir && Input::KeyPressed(Key::Space)) {
        vel += ToWorld(Vector3D(0, 0, .05));
        inAir = true;
    }
}

void Player::DoCollision() {
    if (noclip) return;
    Vector3I up = System::world->GetUp(pos);
    bool X, Y, Z;
    X = Y = Z = true;

    Vector3D vel = FromWorld(this->vel);

    if (System::world->Intersects(BBox().Offset(ToWorld(vel.X())))) {
        vel.x = 0;
        X = false;
    }
    if (System::world->Intersects(BBox().Offset(ToWorld(vel.Y())))) {
        vel.y = 0;
        Y = false;
    }
    if (System::world->Intersects(BBox().Offset(ToWorld(vel.Z())))) {
        inAir = vel.z > 0;
        vel.z = 0;
        Z = false;
    } else {
        inAir = true;
    }

    if (X && Y && System::world->Intersects(BBox().Offset(ToWorld(vel.XY())))) {
        if (vel.x < vel.y) {
            vel.x = 0;
        } else {
            vel.y = 0;
        }
    }
    if (X && Z && System::world->Intersects(BBox().Offset(ToWorld(vel.XZ())))) {
        if (vel.x < vel.z) {
            vel.x = 0;
        } else {
            vel.z = 0;
        }
    }
    if (Y && Z && System::world->Intersects(BBox().Offset(ToWorld(vel.YZ())))) {
        if (vel.y < vel.z) {
            vel.y = 0;
        } else {
            vel.z = 0;
        }
    }
    if (X && Y && Z && System::world->Intersects(BBox().Offset(this->vel))) {
        if (vel.x < vel.y && vel.x < vel.z) {
            vel.x = 0;
        } else if (vel.y < vel.z) {
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

    cameraUp = right.Cross(dir);

    kvec = kdir * (Input::KeyPressed(Key::W) - Input::KeyPressed(Key::S))
           + right * (Input::KeyPressed(Key::D) - Input::KeyPressed(Key::A));

    if (noclip) {
        kvec += Vector3D::AXIS_Z * ((Input::KeyPressed(Key::Q) || Input::KeyPressed(Key::Space)) - Input::KeyPressed(Key::E));
    }

    kvec = kvec.Normalize(1.0 / (Input::KeyPressed(Key::LControl) ? (noclip ? 300.0 : 200.0) : (Input::KeyPressed(Key::LShift) ? (noclip ? 5.0 : 25.0) : (noclip ? 35.0 : 50.0))));
}