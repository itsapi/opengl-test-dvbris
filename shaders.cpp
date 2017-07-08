#include "shaders.h"


void
print_gl_error(GLenum error_code)
{
  switch (error_code)
  {
    case (GL_INVALID_ENUM):
    {
      printf("OpenGL error: GL_INVALID_ENUM\n");
    } break;
    case (GL_INVALID_VALUE):
    {
      printf("OpenGL error: GL_INVALID_VALUE\n");
    } break;
    case (GL_INVALID_OPERATION):
    {
      printf("OpenGL error: GL_INVALID_OPERATION\n");
    } break;
    case (GL_STACK_OVERFLOW):
    {
      printf("OpenGL error: GL_STACK_OVERFLOW\n");
    } break;
    case (GL_STACK_UNDERFLOW):
    {
      printf("OpenGL error: GL_STACK_UNDERFLOW\n");
    } break;
    case (GL_OUT_OF_MEMORY):
    {
      printf("OpenGL error: GL_OUT_OF_MEMORY\n");
    } break;
    case (GL_INVALID_FRAMEBUFFER_OPERATION):
    {
      printf("OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
    } break;
    case (GL_TABLE_TOO_LARGE):
    {
      printf("OpenGL error: GL_TABLE_TOO_LARGE\n");
    }
  }
}


void
print_gl_errors()
{
  GLenum error = glGetError();
  while (error != GL_NO_ERROR)
  {
    print_gl_error(error);

    error = glGetError();
  }
}


bool
compile_shader(const char shader_source[], int size, GLenum shader_type, GLuint *shader)
{
  bool success = true;

  *shader = glCreateShader(shader_type);

  glShaderSource(*shader, 1, &shader_source, &size);
  glCompileShader(*shader);

  GLint compile_success = 0;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &compile_success);
  if (!compile_success)
  {
    GLint log_size = 0;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &log_size);

    GLchar *log = (GLchar *)malloc(sizeof(GLchar) * log_size);
    glGetShaderInfoLog(*shader, log_size, NULL, log);

    printf("Shader compile error:\n%s\n", log);
    free(log);

    glDeleteShader(*shader);
    *shader = 0;
    success = false;
  }

  return success;
}


bool
load_shader(const char filename[], GLenum shader_type, GLuint *shader)
{
  bool success = true;
  *shader = 0;

  File file;
  if (open_file(filename, &file))
  {
    success = compile_shader(file.text, file.size, shader_type, shader);
  }
  else
  {
    success = false;
  }

  return success;
}


bool
create_shader_program(const char *filenames[], GLenum types[], int size, GLuint *shader_program)
{
  bool success = true;
  *shader_program = glCreateProgram();

  for (int i = 0; i < size; ++i)
  {
    GLuint shader;
    bool shader_success = load_shader(filenames[i], types[i], &shader);
    if (shader_success == 0)
    {
      success = false;
      break;
    }
    glAttachShader(*shader_program, shader);
  }

  if (success)
  {
    glLinkProgram(*shader_program);

    GLint is_linked = 0;
    glGetProgramiv(*shader_program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE)
    {
      GLint log_size = 0;
      glGetProgramiv(*shader_program, GL_INFO_LOG_LENGTH, &log_size);

      GLchar *log = (GLchar *)malloc(sizeof(GLchar) * log_size);
      glGetProgramInfoLog(*shader_program, log_size, NULL, log);

      printf("Shader link error:\n%s\n", log);
      free(log);

      glDeleteProgram(*shader_program);

      success = false;
    }
  }

  return success;
}


GLuint
create_buffer()
{
  GLuint result;

  int n_buffers = 1;
  glGenBuffers(n_buffers, &result);

  return result;
}


void
setup_vao(GLuint *vao)
{
  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);
}


void
setup_vertex_vbo_ibo(GLuint *vertex_vbo, ShaderAttributes *shader_attributes, int n_shader_attributes,
                     GLuint *vertex_ibo, GLushort *indices, int n_indices)
{
  // Vertex VBO

  glGenBuffers(1, vertex_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vertex_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ShaderAttributes) * n_shader_attributes, shader_attributes, GL_STATIC_DRAW);

  GLuint attribute_pos = 0;
  glVertexAttribPointer(attribute_pos, sizeof(vec3)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(ShaderAttributes), (const void *)offsetof(ShaderAttributes, pos));
  glEnableVertexAttribArray(attribute_pos);

  GLuint attribute_colour = 3;
  glVertexAttribPointer(attribute_colour, sizeof(vec3)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(ShaderAttributes), (const void *)offsetof(ShaderAttributes, colour));
  glEnableVertexAttribArray(attribute_colour);

  // Vertex IBO

  glGenBuffers(1, vertex_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vertex_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * n_indices, indices, GL_STATIC_DRAW);
}


void
setup_instances_vbo(GLuint *instance_vbo, mat4 *instances, int n_instances)
{
  glGenBuffers(1, instance_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *instance_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * n_instances, instances, GL_STATIC_DRAW);

  // Max attribute size is 4, so have to do mat4 as 4 vec4s
  GLuint attribute_instance_transform[4] = {6, 7, 8, 9};
  glEnableVertexAttribArray(attribute_instance_transform[0]);
  glVertexAttribPointer(attribute_instance_transform[0], sizeof(vec4)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)(sizeof(vec4)*0));
  glEnableVertexAttribArray(attribute_instance_transform[1]);
  glVertexAttribPointer(attribute_instance_transform[1], sizeof(vec4)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)(sizeof(vec4)*1));
  glEnableVertexAttribArray(attribute_instance_transform[2]);
  glVertexAttribPointer(attribute_instance_transform[2], sizeof(vec4)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)(sizeof(vec4)*2));
  glEnableVertexAttribArray(attribute_instance_transform[3]);
  glVertexAttribPointer(attribute_instance_transform[3], sizeof(vec4)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)(sizeof(vec4)*3));

  glVertexAttribDivisor(attribute_instance_transform[0], 1);
  glVertexAttribDivisor(attribute_instance_transform[1], 1);
  glVertexAttribDivisor(attribute_instance_transform[2], 1);
  glVertexAttribDivisor(attribute_instance_transform[3], 1);
}


void
update_instance(GLuint instance_vbo, int instance_n, mat4 *instance)
{
  glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(mat4)*instance_n, sizeof(mat4), instance);
}


bool
get_uniform_locations(Uniforms *uniforms, GLuint shader_program)
{
  bool success = true;

  uniforms->uniform_mat4_world_transform = glGetUniformLocation(shader_program, "world_transform");
  if (uniforms->uniform_mat4_world_transform == -1)
  {
    printf("Failed to find uniform mat4 world_transform");
    success &= false;
  }

  return success;
}