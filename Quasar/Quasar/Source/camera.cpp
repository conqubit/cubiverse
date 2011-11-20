#include "stdafx.h"

#include "system.h"
#include "camera.h"

FreeCamera::FreeCamera() :
    pos(),
    dir(),
    yaw(),
    pitch() {
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::DoInput() {
    yaw += System::input->dmx/300.0;
    pitch -= System::input->dmy/300.0;

    if (pitch < -PI_2 + 0.01) {
        pitch = -PI_2 + 0.01;
    }
    if (pitch > PI_2 - 0.01) {
        pitch = PI_2 - 0.01;
    }

    dir = Vector3D(sin(yaw) * cos(pitch), sin(pitch), cos(yaw) * cos(pitch)).Normalize();

    Vector3D vel = dir * (System::input->KeyPressed(DIK_W) - System::input->KeyPressed(DIK_S))
                 + Vector3D::AXIS_Y.Cross(dir).Normalize()
                 * (System::input->KeyPressed(DIK_D) - System::input->KeyPressed(DIK_A))
                 + Vector3D(0, (System::input->KeyPressed(DIK_Q) - System::input->KeyPressed(DIK_E)), 0);

    pos += vel.Normalize() / (System::input->KeyPressed(DIK_LCONTROL) ? 100 : 10);

    SetCursorPos(800, 450);
}

XMMATRIX FreeCamera::GetMat() {
    return XMMatrixLookToLH(pos.ToXMVector(), dir.ToXMVector(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}