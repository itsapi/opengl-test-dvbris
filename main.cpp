#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


#ifdef __APPLE__

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>

#else

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif


#include "file.cpp"
#include "strings.cpp"
#include "vectors.cpp"
#include "shaders.cpp"
#include "game.cpp"


const unsigned int WINDOW_WIDTH = 900;
const unsigned int WINDOW_HEIGHT = 900;

const float FPS = 60;


void
game_loop(SDL_Window *sdl_window)
{
  bool running = true;
  bool first_frame = true;

  while (running)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
          running = false;
        } break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
          if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            running = false;
          }
        } break;

        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
        {
        } break;
      }
    }

    running &= game(sdl_window, first_frame);

    usleep(1000000.0/FPS);
    first_frame = false;
  }
}


int
main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("Failed to init SDL.\n");
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);

  SDL_Window *sdl_window = SDL_CreateWindow("OpenGL Test Dvbris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, flags);
  if (!sdl_window)
  {
    printf("Failed to initialise SDL window.\n");
    exit(1);
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);
  if (!gl_context)
  {
    printf("Failed to create OpenGL context.\n");
    exit(1);
  }

  SDL_GL_SetSwapInterval(1);

#ifndef __APPLE__
  glewExperimental = GL_TRUE;

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
  {
    printf("Failed to init GLEW: \"%s\"\n", glewGetErrorString(glew_status));
    exit(1);
  }
#endif

  const unsigned char *opengl_version = glGetString(GL_VERSION);
  printf("OpenGL Version: %s\n", opengl_version);

  print_gl_errors();
  printf("OpenGL init finished.\n");

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glEnable(GL_DEPTH_TEST);

  game_loop(sdl_window);

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return 0;
}