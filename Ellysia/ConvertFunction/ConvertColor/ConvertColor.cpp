#include "ConvertColor.h"

Vector4 ColorAdapter(const uint32_t& color){
    Vector4 ResultColor = {
        //赤
       ((color >> 24) & 0xFF) / 255.0f,
       //緑
       ((color >> 16) & 0xFF) / 255.0f,
       //青
       ((color >>8) & 0xFF) / 255.0f,
       //透明度
       ((color) & 0xFF) / 255.0f 

    };

     return ResultColor;

}

