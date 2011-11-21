#include "stdafx.h"

#include "System.h"
#include "Camera.h"

FreeCamera::FreeCamera() :
    pos(), dir(), yaw(), pitch() {
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::DoInput() {
    yaw -= System::input->dx / 300.0;
    pitch -= System::input->dy / 300.0;

    if (pitch < -PI_2 + 0.01) {
        pitch = -PI_2 + 0.01;
    }
    if (pitch > PI_2 - 0.01) {
        pitch = PI_2 - 0.01;
    }

    dir = Vector3D(sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch)).Normalize();

    Vector3D vel = dir * (System::input->KeyPressed(DIK_W) - System::input->KeyPressed(DIK_S))
                   + Vector3D::AXIS_Z.Cross(dir).Normalize() *
                   (System::input->KeyPressed(DIK_D) - System::input->KeyPressed(DIK_A))
                   + Vector3D::AXIS_Z * (System::input->KeyPressed(DIK_Q) - System::input->KeyPressed(DIK_E));

    pos += vel.Normalize(1.0 / (System::input->KeyPressed(DIK_LCONTROL) ? 100 : (System::input->KeyPressed(DIK_LSHIFT) ? 1 : 10)));
}

XMMATRIX FreeCamera::GetMat() {
    return XMMatrixLookToLH(pos.ToXMVector(), dir.ToXMVector(), Vector3F::AXIS_Z.ToXMVector());
}