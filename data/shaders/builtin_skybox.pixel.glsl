#version 450

layout(location = 0) out vec4 pixel;

layout(binding = 0) uniform sampler2D tex;

void main(){
  pixel = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
