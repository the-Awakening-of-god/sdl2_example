#include "constants.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <stdio.h>

SDL_Window *window = NULL;
SDL_GLContext context = NULL;
int game_is_running = FALSE;
// Vertex Array Object
GLuint gVertextArrayObject = 0;
GLuint gVertextBufferObject = 0;

const char *gVertexShaderSource =
    "#version 410 core\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
    "}\n";

const char *gFragmentShaderSource = "#version 410 core\n"
                                    "out vec4 color;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
                                    "}\n";

// Program Object (for our shaders)
GLuint gGraphicsPipelineShaderProgram = 0;

GLuint CompileShader(GLuint type, const char *source) {
  GLuint shaderObject = 0;
  if (type == GL_VERTEX_SHADER) {
    shaderObject = glCreateShader(GL_VERTEX_SHADER);
  } else if (type == GL_FRAGMENT_SHADER) {
    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  }

  glShaderSource(shaderObject, 1, &source, NULL);
  glCompileShader(shaderObject);

  return shaderObject;
}

GLuint CreateShaderProgram(const char *vertexshadersource,
                           const char *fragmentshadersource) {

  GLuint programObject = glCreateProgram();

  GLuint myVertextShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
  GLuint myFragmentShader =
      CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

  glAttachShader(programObject, myVertextShader);

  glAttachShader(programObject, myFragmentShader);

  glLinkProgram(programObject);

  // Validate our program
  glValidateProgram(programObject);

  return programObject;
}

void CreateGraphicsPipeline() {

  gGraphicsPipelineShaderProgram =
      CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}
void VertextSpecification() {

  // Lives on Cpu
  const GLfloat vertextPosition[] = {
      -0.8f, -0.8f, 0.0f, // Vertex 1
      0.8f,  -0.8f, .0f,  // Vertex 2
      0.0f,  0.8f,  0.0f, // Vertex 3
  };
  // We are setting things up on the GPU
  glGenVertexArrays(1, &gVertextArrayObject);
  glBindVertexArray(gVertextArrayObject);

  // Start generation our Vertex Buffer Object

  glGenBuffers(1, &gVertextBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertextBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertextPosition), vertextPosition,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
}

int initialize_window(void) {
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
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glClearColor(1.f, 1.f, 0.f, 1.f);

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUseProgram(gGraphicsPipelineShaderProgram);
}

void render() {

  glBindVertexArray(gVertextArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertextBufferObject);
  glDrawArrays(GL_TRIANGLES, 0, 3);
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
