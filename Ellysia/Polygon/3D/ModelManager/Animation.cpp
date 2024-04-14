#include "Animation.h"
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName){
    Animation animation = {};
    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + fileName;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
   
    //アニメーションが無い場合止める
    assert(scene->mNumAnimations != 0);
    //最初のアニメーションだけを採用
    aiAnimation* animationAssimp = scene->mAnimations[0];

    //mTicksPerSecond...周波数
    //mDuration...mTicksPerSecondで指定された周波数における長さ
    animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);



    //NodeAnimationを解析する
    //assimpでは個々のNodeAnimationをChannelと呼んでいるのでChannelを回してNodeAnimationの情報をとってくる
    for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
        aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
        NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
        //Translate
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
            KeyFrameVector3 keyFrame = {};
            //秒変換
            keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
            //右手->左手
            keyFrame.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
            nodeAnimation.translate.keyFrames.push_back(keyFrame);
        }
        //RotateはmNunRotateionKeys/mRotateKeys
        //RotateはQuaternionで、右手->左手に変換するために、YとZを反転させる必要がある。
         
        
        
        //Rotate
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; keyIndex) {
            aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
            KeyFrameQuaternion keyFrame = {};
            //秒変換
            keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
            //右手->左手
            //KeyFrame.value={rotate.x,-rotate.y,-rotate.z,rotate.w};
            keyFrame.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };
            nodeAnimation.rotate.keyFrames.push_back(keyFrame);
        }

        //ScaleはmNumScalingKeys/mScalingKeysで取得出来るので同時に行う
        //Scaleはそのままで良い

        //Scale
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
            KeyFrameVector3 keyFrame = {};
            //秒変換
            keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
            keyFrame.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z};
            nodeAnimation.scale.keyFrames.push_back(keyFrame);
        }

    }


    return animation;
}
