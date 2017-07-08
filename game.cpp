#include "game.h"


bool
game(SDL_Window *sdl_window, bool first_frame)
{
  static GLuint vao,
                vertex_vbo,
                vertex_ibo,
                instance_vbo;
  static Uniforms uniforms;

  static Block block_instances[] = {
    {{1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1}, BLOCK_STONE},
    {{1, 0, 0, 2,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1}, BLOCK_DIRT},
    {{1, 0, 0, 0,
      0, 1, 0, 2,
      0, 0, 1, 0,
      0, 0, 0, 1}, BLOCK_GRASS},
    {{1, 0, 0, 2,
      0, 1, 0, 2,
      0, 0, 1, 0,
      0, 0, 0, 1}, BLOCK_GRASS},
    {{1, 0, 0, 2,
      0, 1, 0, 0,
      0, 0, 1, -2,
      0, 0, 0, 1}, BLOCK_GRASS}
  };

  static Vertex cube_vertices[] = {
    // front
    {.pos = {-1, -1,  1}},
    {.pos = { 1, -1,  1}},
    {.pos = { 1,  1,  1}},
    {.pos = {-1,  1,  1}},
    // back
    {.pos = {-1, -1, -1}},
    {.pos = { 1, -1, -1}},
    {.pos = { 1,  1, -1}},
    {.pos = {-1,  1, -1}},
  };

  static GLushort cube_indices[] = {
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

  static int n_block_instances = sizeof(block_instances) / sizeof(Block);
  static int n_cube_vertices = sizeof(cube_vertices) / sizeof(Vertex);
  static int n_cube_indices = sizeof(cube_indices) / sizeof(GLushort);


  bool keep_running = true;

  if (first_frame)
  {
    setup_vao(&vao);
    setup_vertex_vbo_ibo(&vertex_vbo, cube_vertices, n_cube_vertices,
                         &vertex_ibo, cube_indices, n_cube_indices);
    setup_block_instances_vbo(&instance_vbo, block_instances, n_block_instances);

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

    // Upload the block colours map to the uniform
    glUniform3fv(uniforms.uniform_vec3_block_type_colours, 3, BLOCK_TYPE_COLOURS[0].elem);

    print_gl_errors();
    printf("End of first frame setup.\n");
  }

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float seconds = 0.001 * SDL_GetTicks();

  // Update block instances



  // Update uniforms

  mat4 world_transform = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};

  vec3 offset = {0, 0, 0};
  multiply(&world_transform, (mat4){1, 0, 0, offset.x,
                                    0, 1, 0, offset.y,
                                    0, 0, 1, offset.z,
                                    0, 0, 0,        1});

  float scale = 0.1;
  multiply(&world_transform, (mat4){scale,     0,     0, 0,
                                        0, scale,     0, 0,
                                        0,     0, scale, 0,
                                        0,     0,     0, 1});

  float theta_z = -45 * M_PI / 180;
  float theta_y = 45 * seconds * M_PI / 180;
  float theta_x = 0;

  multiply(&world_transform, (mat4){1,            0,             0, 0,
                                    0, cos(theta_z), -sin(theta_z), 0,
                                    0, sin(theta_z),  cos(theta_z), 0,
                                    0,            0,             0, 1});

  multiply(&world_transform, (mat4){ cos(theta_y), 0, sin(theta_y), 0,
                                                0, 1,            0, 0,
                                    -sin(theta_y), 0, cos(theta_y), 0,
                                                0, 0,            0, 1});

  multiply(&world_transform, (mat4){cos(theta_x), -sin(theta_x), 0, 0,
                                    sin(theta_x),  cos(theta_x), 0, 0,
                                               0,             0, 1, 0,
                                               0,             0, 0, 1});

  glUniformMatrix4fv(uniforms.uniform_mat4_world_transform, 1, true, world_transform.es);

  glDrawElementsInstanced(GL_TRIANGLES, n_cube_indices, GL_UNSIGNED_SHORT, 0, n_block_instances);

  SDL_GL_SwapWindow(sdl_window);
  print_gl_errors();

  return keep_running;
}
