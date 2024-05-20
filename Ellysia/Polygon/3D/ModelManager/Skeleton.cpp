#include "Skeleton.h"
#include <Matrix4x4Calculation.h>

void Skeleton::Create(const Node& rootNode){
    root_ = CreateJoint(rootNode, {}, joints_);

    //名前とINdexのマッピングを行いアクセスしやすくする。
    for (const Joint& joint : joints_) {
        //emplace...push_backみたいなもの。新しく挿入する。
        jointMap_.emplace(joint.name, joint.index);
    }

}

void Skeleton::Update(){
    //全てのJointを更新。親が若いので通常ループで処理可能になっている。
    for (Joint& joint : joints_) {
        Vector3 newRotate = { joint.transform.rotate.x,joint.transform.rotate.y,joint.transform.rotate.z };
        joint.localMatrix = MakeAffineMatrix(joint.transform.scale, newRotate, joint.transform.translate);

        //親がいれば親の行列を掛ける
        if (joint.parent) {
            joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, joints_[*joint.parent].skeletonSpaceMatrix);
        }
        //親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する
        else {
            joint.skeletonSpaceMatrix = joint.localMatrix;
        }

    }


}
