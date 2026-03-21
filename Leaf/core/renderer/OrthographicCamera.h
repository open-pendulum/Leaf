#pragma once

#include <glm/glm.hpp>

namespace Leaf {

// 正交相机（Orthographic Camera）
// --------------------------------
// 1. 用一个固定大小的“盒子”来观察世界：left/right/bottom/top
//    在这个盒子里的物体不会因为远近而产生透视缩放，适合 2D 游戏、UI。
// 2. 内部维护三种矩阵：
//    - 投影矩阵：只由视口(left/right/bottom/top) + 近远平面决定
//    - 视图矩阵：由相机的位置、旋转决定（世界 → 相机空间）
//    - 视图投影矩阵：Projection * View，渲染时直接传给 Shader 使用。
class OrthographicCamera {
public:
    // 使用世界空间中的一个轴对齐盒子来初始化相机的可视区域：
    // - X 方向范围 [left, right]
    // - Y 方向范围 [bottom, top]
    // - Z 方向范围固定为 [-1, 1]（在当前实现中写死在 cpp 内）
    explicit OrthographicCamera(float left, float right, float bottom,
                                float top);

    // 获取/设置相机的位置（世界空间）
    [[nodiscard]] const glm::vec3 &GetPosition() const {
        return mPosition;
    }

    void SetProjection(float left, float right, float bottom, float top);

    // 修改位置后会重新计算视图矩阵，从而影响最终的 ViewProjection
    void SetPosition(const glm::vec3 &position) {
        mPosition = position;
        RecalculateViewMatrix();
    }

    // 获取/设置相机绕 Z 轴的旋转角度（单位：度）
    [[nodiscard]] float GetRotation() const {
        return mRotation;
    }

    // 注意：这里采用“相机坐标系”的约定，
    // 实际上是把相机的旋转转换为场景的反向旋转，具体逻辑在
    // RecalculateViewMatrix 中。
    void SetRotation(float rotation) {
        mRotation = rotation;
        RecalculateViewMatrix();
    }

    // 只读访问三个矩阵，用于渲染管线（例如传入 Shader）
    [[nodiscard]] const glm::mat4 &GetProjectionMatrix() const {
        return mProjectionMatrix;
    }

    [[nodiscard]] const glm::mat4 &GetViewMatrix() const {
        return mViewMatrix;
    }

    [[nodiscard]] const glm::mat4 &GetViewProjectionMatrix() const {
        return mViewProjectionMatrix;
    }

private:
    // 根据当前 position & rotation 重新计算 View 与 ViewProjection
    // - Projection 不在这里改动（本类当前没有“修改投影参数”的接口）
    void RecalculateViewMatrix();

private:
    // 正交投影矩阵（由构造函数固定下来）
    glm::mat4 mProjectionMatrix;
    // 视图矩阵：世界空间 → 相机空间
    glm::mat4 mViewMatrix;
    // 组合矩阵：Projection * View，渲染时最常用
    glm::mat4 mViewProjectionMatrix {};

    // 相机在世界空间中的位置
    glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
    // 相机绕 Z 轴的旋转角度（单位：度）
    float mRotation = 0.0f;
};

}  // namespace Leaf
