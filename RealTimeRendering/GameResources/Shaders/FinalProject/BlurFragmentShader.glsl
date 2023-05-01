#version 330 core

uniform sampler2D TextureSampler;

in vec2 TexCoords;
in vec2 VertexPos;
out vec4 Color;

uniform float textureSize;

uniform float screenWidth;
uniform float screenHeight;

uniform int blur;
uniform int highlight;
uniform float highlight_mul;

float offset_x = 1.0 / screenWidth;
float offset_y = 1.0 / screenHeight;

float stepSize = 1 / textureSize;

vec2 offsets[25] = vec2[](
    vec2(-offset_x * 2.0, offset_y * 2.0),  vec2(-offset_x , offset_y * 2.0),   vec2(0.0, offset_y * 2.0),      vec2(offset_x, offset_y * 2.0),     vec2(offset_x * 2.0, offset_y * 2.0),
    vec2(-offset_x * 2.0, offset_y),        vec2(-offset_x, offset_y),          vec2(0.0, offset_y),            vec2(offset_x, offset_y),           vec2(offset_x * 2.0, offset_y),
    vec2(-offset_x * 2.0, 0.0),             vec2(-offset_x, 0.0),               vec2(0.0, 0.0),                 vec2(offset_x, 0.0),                vec2(offset_x * 2.0, 0.0), 
    vec2(-offset_x * 2.0, -offset_y),       vec2(-offset_x, -offset_y),         vec2(0.0, -offset_y),           vec2(offset_x, -offset_y) ,         vec2(offset_x * 2.0, -offset_y),
    vec2(-offset_x * 2.0, -offset_y * 2.0), vec2(-offset_x, -offset_y * 2.0),   vec2(0.0, -offset_y * 2.0),     vec2(offset_x, -offset_y * 2.0) ,   vec2(offset_x * 2.0, -offset_y * 2.0)
);
float kernel[25] = float[](
    1.0,    4.0,    7.0,    4.0,    1.0,
    4.0,    16.0,   26.0,   16.0,   4.0,
    7.0,    26.0,   41.0,   26.0,   4.0,
    4.0,    16.0,   26.0,   16.0,   4.0,
    1.0,    4.0,    7.0,    4.0,    1.0
);
float highlight_kernel[25] = float[](
    -1.0,   -1.0,   -1.0,   -1.0,   -1.0,
    -1.0,   -1.0,   -1.0,   -1.0,   -1.0,
    -1.0,   -1.0,   24.0,    -1.0,   -1.0,
    -1.0,   -1.0,   -1.0,   -1.0,   -1.0,
    -1.0,   -1.0,   -1.0,   -1.0,   -1.0
);
void main() {
    float weights[7] = float[7](
		0.006,
		0.061,
		0.242,
		0.383,
		0.242,
		0.061,
		0.006
		);
    vec2 coords = TexCoords - vec2(0.0, stepSize * 3.0);
    vec4 sum = vec4(0.0);
    for(int i = 0; i < 7; i++) {
        vec4 tap = texture2D(TextureSampler, coords);
		sum += weights[i] * tap;
		coords += vec2(0.0, stepSize * 1.0);
    }
    Color = sum;
    // float kernel_sum = 0.0;
    // for(int i = 0; i < 25; i++) {
    //     kernel_sum += kernel[i];
    // }
    // for(int i = 0; i < 25; i++) {
    //     kernel[i] /= kernel_sum;
    // }
    // if(blur == 1) {
    //     vec3 color = vec3(0.0f);
    //     for(int i = 0; i < 25; i++) {
    //         if(!((TexCoords.s > (0.5 - 0.15) && TexCoords.s < (0.5 + 0.15)) && (TexCoords.t > (0.5 - 0.15) && TexCoords.t < (0.5 + 0.15)))) {
    //             color += vec3(texture(TextureSampler, TexCoords.st + offsets[i])) * kernel[i];
    //         }
    //         else {
    //             color = vec3(texture(TextureSampler, TexCoords.st));
                
    //         }
    //     }
    //     Color = vec4(color, 1.0);
    // }
    // else if(highlight == 1) {
    //     vec3 color = vec3(0.0f);
    //     for(int i = 0; i < 25; i++) {
    //         color += vec3(texture(TextureSampler, TexCoords.st + offsets[i])) * (highlight_kernel[i] * highlight_mul);
    //     }
    //     Color = vec4(color, 1.0);
    // }
    // else {
    //     Color = texture(TextureSampler, TexCoords);
    // }
    // vec4 texColor = texture(TextureSampler, TexCoords);
    // float avg = (texColor.x + texColor.y + texColor.z) / 3;
    // Color = vec4(avg, avg, avg, 1.0);
    //Color = vec4(1.0, 1.0, 1.0, 1.0);
    //Color = vec4(TexCoords, 0.0, 1.0);
}