#pragma once
#include "Material.h"
#include "DirectXSetup.h"
#include "Vector4.h"

class CreateMaterial{
public:

	void Initialize();

	void SetInformation(Vector4 setColor, bool enableLighting);

	void GraphicsCommand();

	
	

private:
	//頂点リソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	
	


};
