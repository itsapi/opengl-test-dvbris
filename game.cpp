#include "game.h"


bool
game(SDL_Window *sdl_window, bool first_frame)
{
  static int n_points;
  static GLuint vbo;

  bool keep_running = true;

  if (first_frame)
  {
    int n_shaders = 2;
    const char *filenames[n_shaders];
    GLenum shader_types[n_shaders];

    filenames[0] = "vertex-shader.glvs";
    shader_types[0] = GL_VERTEX_SHADER;

    filenames[1] = "fragment-shader.glfs";
    shader_types[1] = GL_FRAGMENT_SHADER;

    GLuint shader_program = 0;
    bool compile_success = create_shader_program(filenames, shader_types, n_shaders, &shader_program);
    if (!compile_success)
    {
      keep_running = false;
      return keep_running;
    }

    vbo = create_buffer();
    bind_shader_attributes(vbo, shader_program);

    bool link_success = link_shader_program(shader_program);
    if (!link_success)
    {
      keep_running = false;
      return keep_running;
    }

    glUseProgram(shader_program);

    ShaderAttributes points[] = {
      {.pos = {-0.45f,  0.45f}, .colour = {1.0f, 0.0f, 0.0f}},
      {.pos = { 0.45f,  0.45f}, .colour = {0.0f, 1.0f, 0.0f}},
      {.pos = { 0.45f, -0.45f}, .colour = {0.0f, 0.0f, 1.0f}},
      {.pos = {-0.45f, -0.45f}, .colour = {1.0f, 1.0f, 0.0f}}
    };

    n_points = sizeof(points) / sizeof(ShaderAttributes);
    load_coords_into_vbo(vbo, points, n_points);
  }

  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glDrawArrays(GL_QUADS, 0, n_points);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  SDL_GL_SwapWindow(sdl_window);

  print_gl_errors();

  return keep_running;
}
