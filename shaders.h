#ifndef SHADERS_H_DEF
#define SHADERS_H_DEF

struct Vertex
{
  vec3 pos;
};


struct Uniforms
{
  GLuint uniform_mat4_world_transform;
  GLuint uniform_vec3_block_type_colours;
};

#endif