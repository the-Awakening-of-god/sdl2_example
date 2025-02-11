#include "setup.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <math.h>

SDL_Window *window = NULL;
SDL_GLContext context = NULL;
int game_is_running = FALSE;
// Vertex Array Object
GLuint gVertextArrayObject = 0;
GLuint gVertextBufferObject = 0;
GLuint gEBO;

// Program Object (for our shaders)
GLuint ShaderProgram = 0;

void CreateGraphicsPipeline() {
  char *VertexShaderSource = readFile("src/shaders/vertshader.vert");
  char *FragmentShaderSource = readFile("src/shaders/fragshader.frag");
  ShaderProgram = CreateShaderProgram(VertexShaderSource, FragmentShaderSource);
  free(VertexShaderSource);
  free(FragmentShaderSource);
}

int initialize_window() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return FALSE;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  int window_flags = SDL_WINDOW_OPENGL;

  window = SDL_CreateWindow("Some fuckin Bullshit", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            window_flags);

  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return FALSE;
  }

  context = SDL_GL_CreateContext(window);

  if (!context) {
    fprintf(stderr, "Error creating GL context");
    return FALSE;
  }

  SDL_GL_CreateContext(window);

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    return FALSE;
  }

  char *GL_version = (char *)glGetString(GL_VERSION);
  char *GL_vendor = (char *)glGetString(GL_VENDOR);
  char *GL_renderer = (char *)glGetString(GL_RENDERER);

  printf("Version: %s\n", GL_version);
  printf("Vendor: %s\n", GL_vendor);
  printf("Renderer: %s\n", GL_renderer);

  return TRUE;
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    game_is_running = FALSE;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      game_is_running = FALSE;
    break;
  default:
    break;
  }
}

void PreDraw() {

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(ShaderProgram);
}

void VertextSpecification() {

  // Lives on Cpu
  const GLfloat vertextPosition[] = {
      // first triangle
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, // top left
  };
  const GLuint indices[] = {0, 1, 3, 1, 2, 3};
  // We are setting things up on the GPU
  glGenVertexArrays(1, &gVertextArrayObject);
  glBindVertexArray(gVertextArrayObject);

  // Start generation our Vertex Buffer Object

  glGenBuffers(1, &gVertextBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertextBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertextPosition), vertextPosition,
               GL_STATIC_DRAW);
  // Element Array Buffer
  glGenBuffers(1, &gEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glBindVertexArray(0);
}

void render() {
  glBindVertexArray(gVertextArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertextBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);

  double timeValue = SDL_GetTicks() / 1000.f;
  double greenValue = (sin(timeValue) / 2.0f) + 0.5f;
  int vertexColorLocation = glGetUniformLocation(ShaderProgram, "ourColor");
  glUseProgram(ShaderProgram);
  glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void destroy_window() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
  game_is_running = initialize_window();

  VertextSpecification();

  CreateGraphicsPipeline();

  while (game_is_running) {
    process_input();
    PreDraw();
    render();

    SDL_GL_SwapWindow(window);
  }
  destroy_window();

  return 0;
}
