#include "game.h"


bool
game(SDL_Window *sdl_window, bool first_frame)
{
  static int n_points,
             n_indices,
             n_instances;
  static GLuint vao,
                vertex_vbo,
                vertex_ibo,
                instance_vbo;
  static Uniforms uniforms;

  bool keep_running = true;

  if (first_frame)
  {
    ShaderAttributes points[] = {
      // front
      {.pos = {-1, -1,  1}, .colour = {1, 0, 0}},
      {.pos = { 1, -1,  1}, .colour = {0, 1, 0}},
      {.pos = { 1,  1,  1}, .colour = {0, 0, 1}},
      {.pos = {-1,  1,  1}, .colour = {1, 1, 1}},
      // back
      {.pos = {-1, -1, -1}, .colour = {1, 0, 0}},
      {.pos = { 1, -1, -1}, .colour = {0, 1, 0}},
      {.pos = { 1,  1, -1}, .colour = {0, 0, 1}},
      {.pos = {-1,  1, -1}, .colour = {1, 1, 1}},
    };
    n_points = sizeof(points) / sizeof(ShaderAttributes);

    GLushort indices[] = {
      // front
      0, 1, 2,
      2, 3, 0,
      // top
      1, 5, 6,
      6, 2, 1,
      // back
      7, 6, 5,
      5, 4, 7,
      // bottom
      4, 0, 3,
      3, 7, 4,
      // left
      4, 5, 1,
      1, 0, 4,
      // right
      3, 2, 6,
      6, 7, 3,
    };
    n_indices = sizeof(indices) / sizeof(GLushort);

    vec3 instances[2] = {{-0.5, 0, 0}, {0.5, 0, 0}};
    n_instances = sizeof(instances) / sizeof(vec3);

    setup_vao(&vao,
              &vertex_vbo, &vertex_ibo, &instance_vbo,
              points, n_points,
              indices, n_indices,
              instances, n_instances);

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

    glUseProgram(shader_program);

    bool uniforms_success = get_uniform_locations(&uniforms, shader_program);
    if (!uniforms_success)
    {
      keep_running = false;
      return keep_running;
    }

    print_gl_errors();
    printf("End of first frame setup.\n");
  }

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform1f(uniforms.uniform_float_scale, 0.3);
  glUniform1f(uniforms.uniform_float_theta_x, 1.0);
  glUniform1f(uniforms.uniform_float_theta_y, 45 * 0.001 * SDL_GetTicks() * M_PI / 180);
  glUniform1f(uniforms.uniform_float_theta_z, M_PI * 0.00);

  glDrawElementsInstanced(GL_TRIANGLES, n_indices, GL_UNSIGNED_SHORT, 0, n_instances);

  SDL_GL_SwapWindow(sdl_window);
  print_gl_errors();

  return keep_running;
}
