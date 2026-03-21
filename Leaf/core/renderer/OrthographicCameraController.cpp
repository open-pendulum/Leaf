
#include "OrthographicCameraController.h"

#include "Input.h"
#include "KeyCodes.h"

namespace Leaf {

OrthographicCameraController::OrthographicCameraController(float aspectRatio,
                                                           bool rotation) :
    mAspectRatio(aspectRatio),
    mCamera(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel,
            mZoomLevel),
    mRotation(rotation) {
}

void OrthographicCameraController::OnUpdate(Timestep ts) {
    if (Input::IsKeyPressed(LEAF_KEY_A)) {
        mCameraPosition.x -=
            cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        mCameraPosition.y -=
            sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
    } else if (Input::IsKeyPressed(LEAF_KEY_D)) {
        mCameraPosition.x +=
            cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        mCameraPosition.y +=
            sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
    }

    if (Input::IsKeyPressed(LEAF_KEY_W)) {
        mCameraPosition.x +=
            -sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        mCameraPosition.y +=
            cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
    } else if (Input::IsKeyPressed(LEAF_KEY_S)) {
        mCameraPosition.x -=
            -sin(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
        mCameraPosition.y -=
            cos(glm::radians(mCameraRotation)) * mCameraTranslationSpeed * ts;
    }

    if (mRotation) {
        if (Input::IsKeyPressed(LEAF_KEY_Q)) {
            mCameraRotation += mCameraRotationSpeed * ts;
        }
        if (Input::IsKeyPressed(LEAF_KEY_E)) {
            mCameraRotation -= mCameraRotationSpeed * ts;
        }

        if (mCameraRotation > 180.0f) {
            mCameraRotation -= 360.0f;
        } else if (mCameraRotation <= -180.0f) {
            mCameraRotation += 360.0f;
        }

        mCamera.SetRotation(mCameraRotation);
    }

    mCamera.SetPosition(mCameraPosition);

    mCameraTranslationSpeed = mZoomLevel;
}

void OrthographicCameraController::OnEvent(Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(
        LEAF_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(
        LEAF_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
    mZoomLevel -= e.GetYOffset() * 0.25f;
    mZoomLevel = std::max(mZoomLevel, 0.25f);
    mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel,
                          -mZoomLevel, mZoomLevel);
    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e) {
    mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    mCamera.SetProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel,
                          -mZoomLevel, mZoomLevel);
    return false;
}

}  // namespace Leaf
