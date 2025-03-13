#include "ConvertColor.h"

Vector4 Color::ColorAdapter(const uint32_t& color){
    Vector4 result = {
        //赤
       .x = ((color >> 24) & 0xFF) / 255.0f,
       //緑
       .y =((color >> 16) & 0xFF) / 255.0f,
       //青
       .z =((color >>8) & 0xFF) / 255.0f,
       //透明度
       .w = ((color) & 0xFF) / 255.0f 

    };

     return result;

}

