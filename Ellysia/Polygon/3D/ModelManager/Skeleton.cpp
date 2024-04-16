#include "Skeleton.h"
#include <Matrix4x4Calculation.h>

Skeleton CreateSkeleton(const Node& rootNode){
    Skeleton skeleton = {};
    skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

    //名前とINdexのマッピングを行いアクセスしやすくする。
    for (const Joint& joint : skeleton.joints) {
        //emplace...push_backみたいなもの。新しく挿入する。
        skeleton.jointMap.emplace(joint.name, joint.index);
    }

    return skeleton;
}

void SkeletonUpdate(Skeleton& skeleton){
    //全てのJointを更新。親が若いので通常ループで処理可能になっている。
    for (Joint& joint : skeleton.joints) {
        Vector3 newRotate = { joint.transform.rotate.x,joint.transform.rotate.y,joint.transform.rotate.z };
        joint.localMatrix = MakeAffineMatrix(joint.transform.scale, newRotate, joint.transform.translate);

        //親がいれば親の行列を掛ける
        if (joint.parent) {
            joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, skeleton.joints[*joint.parent].skeletonSpaceMatrix);
        }
        //親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する
        else {
            joint.skeletonSpaceMatrix = joint.localMatrix;
        }

    }


}
