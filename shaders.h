#ifndef SHADERS_H_DEF
#define SHADERS_H_DEF

struct ShaderAttributes
{
  vec3 pos;
  vec3 colour;
};


struct Uniforms
{
  GLuint uniform_mat4_world_transform;
};

#endif