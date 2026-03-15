#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Leaf {

// 这里固定使用一个对称的正交投影体：Z 轴范围 [-1, 1]
// 这通常足够 2D 场景以及简单 UI 使用。
OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top)
    // glm::ortho 会根据 left/right/bottom/top 生成一个正交投影矩阵
    // 注意：这里没有把 near/far 作为参数暴露出去，而是简单地用 -1/1
    : mProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    // 初始视图矩阵设为单位矩阵，表示“相机在世界原点，且没有旋转”
    // [ 1  0  0  0 ]
    // [ 0  1  0  0 ]
    // [ 0  0  1  0 ]
    // [ 0  0  0  1 ]
    , mViewMatrix(1.0f) {
    // 初次构造时，直接用默认的 View 计算一次 VP 矩阵
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthographicCamera::RecalculateViewMatrix() {
    // 相机视图矩阵的几何含义总结：
    // --------------------------------------------------------
    // 1. transform 表示“相机自身在世界里的姿态”（相机空间 → 世界空间）：
    //      transform = T(position) * R(rotation)
    //    - position：相机往哪儿移动
    //    - rotation：相机朝哪个方向看（绕 Z 轴的旋转）
    //
    // 2. View 要做的是“把世界装换到相机眼中”（世界空间 → 相机空间），
    //    所以取 transform 的逆矩阵：
    //      View = inverse(transform)
    //
    // 3. 从直观效果看：
    //    - 当相机在游戏逻辑里向 +X 方向移动 10（position = (10,0,0)）时，
    //      View 会把整个世界向 -X 方向平移 10，看起来就像是相机在动。
    //    - 当相机绕 Z 轴逆时针旋转 +90° 时，
    //      View 会让整个世界绕 Z 轴顺时针转 -90°，
    //      看起来就像是相机转头去看另一个方向。
    //
    // 换句话说：在逻辑上是“相机在世界中移动/旋转”，
    // 在数学上实现为“用 View 矩阵让整个世界围绕相机反向移动/旋转”。
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), mPosition) *
        glm::rotate(glm::mat4(1.0f), glm::radians(mRotation),
                    glm::vec3(0, 0, 1));

    // 将“相机变换矩阵”的逆作为视图矩阵：
    //   世界空间 → 相机空间
    mViewMatrix = glm::inverse(transform);

    // 每次视图矩阵发生变化时，重新计算组合矩阵，
    // 这样对外只需要拿 ViewProjection 就能直接用于 Shader。
    mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}

}  // namespace Leaf
