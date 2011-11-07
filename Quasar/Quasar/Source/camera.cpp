#include "stdafx.h"
#include "system.h"
#include "camera.h"

FreeCamera::FreeCamera() {
    pos.set(XMVectorSet(0, 0, -5, 0));
    dir.set(XMVectorZero());
    yaw = 0;
    pitch = 0;
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::DoInput() {
    yaw += System::input->dmx/300.0;
    pitch -= System::input->dmy/300.0;

    if (pitch < -XM_PI/2 + 0.01) {
        pitch = -XM_PI/2 + 0.01;
    }
    if (pitch > XM_PI/2 - 0.01) {
        pitch = XM_PI/2 - 0.01;
    }

    dir.set(XMVector3Normalize(XMVectorSet(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch), 0.0)));
    XMVECTOR vel =    dir.get() * (float)(System::input->KeyPressed(DIK_W) - System::input->KeyPressed(DIK_S))
                   +  XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0, 1.0, 0.0, 0.0), dir.get()))
                   *  (float)(System::input->KeyPressed(DIK_D) - System::input->KeyPressed(DIK_A))
                   +  XMVectorSet(0.0, (float)(System::input->KeyPressed(DIK_Q) - System::input->KeyPressed(DIK_E)), 0.0, 0.0);
    pos.set(XMVector3Normalize(vel)/(System::input->KeyPressed(DIK_LCONTROL) ? 5000 : 100) + pos.get());
    //SetCursorPos(800, 450);
}

XMMATRIX FreeCamera::GetMat() {
    return XMMatrixLookToLH(pos.get(), dir.get(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}