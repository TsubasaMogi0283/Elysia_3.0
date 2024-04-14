#pragma once
#include "NodeAnimation.h"
#include <map>
#include <string>

struct Animation {
	//アニメーション全体の尺
	float duration;
	//NodeAnimationの集合
	//Node名でひけるようにする
	std::map<std::string, NodeAnimation>nodeAnimations;

};

Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);
