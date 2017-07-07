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

    mat4 instances[] = {
      {0.1, 0, 0, 0,
       0, 1,   0, 1,
       0, 0, 0.1, 0,
       0, 0,   0, 1},
      {1, 0,   0, 0,
       0, 0.1, 0, 0,
       0, 0,   1, 0,
       0, 0,   0, 1},
      {0.2, 0, 0, 0,
       0, 0.2, 0, 0,
       0, 0, 0.2, 0,
       0, 0,   0, 1}
    };
    n_instances = sizeof(instances) / sizeof(mat4);

    setup_vao(&vao,
              &vertex_vbo, &vertex_ibo, &instance_vbo,
              points, n_points,
              indices, n_indices,
              instances, n_instances);

    const int n_shaders = 2;
    const char *filenames[n_shaders] = {"vertex-shader.glvs", "fragment-shader.glfs"};
    GLenum shader_types[n_shaders] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

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

  float seconds = 0.001 * SDL_GetTicks();

  mat4 world_transform = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};

  vec3 offset = {0, 0.5 * sin(180 * seconds * M_PI / 180.0), 0};
  multiply(&world_transform, (mat4){1, 0, 0, offset.x,
                                    0, 1, 0, offset.y,
                                    0, 0, 1, offset.z,
                                    0, 0, 0,        1});

  float scale = 0.3;
  multiply(&world_transform, (mat4){scale,     0,     0, 0,
                                        0, scale,     0, 0,
                                        0,     0, scale, 0,
                                        0,     0,     0, 1});

  float theta_z = 1.0;
  float theta_y = 45 * seconds * M_PI / 180;
  float theta_x = M_PI * 0.00;

  multiply(&world_transform, (mat4){cos(theta_x), -sin(theta_x), 0, 0,
                                    sin(theta_x),  cos(theta_x), 0, 0,
                                               0,             0, 1, 0,
                                               0,             0, 0, 1});

  multiply(&world_transform, (mat4){ cos(theta_y), 0, sin(theta_y), 0,
                                                0, 1,            0, 0,
                                    -sin(theta_y), 0, cos(theta_y), 0,
                                                0, 0,            0, 1});

  multiply(&world_transform, (mat4){1,            0,             0, 0,
                                    0, cos(theta_z), -sin(theta_z), 0,
                                    0, sin(theta_z),  cos(theta_z), 0,
                                    0,            0,             0, 1});

  glUniformMatrix4fv(uniforms.uniform_mat4_world_transform, 1, true, world_transform.es);

  glDrawElementsInstanced(GL_TRIANGLES, n_indices, GL_UNSIGNED_SHORT, 0, n_instances);

  SDL_GL_SwapWindow(sdl_window);
  print_gl_errors();

  return keep_running;
}
