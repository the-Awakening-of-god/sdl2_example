#include <SDL2/SDL.h>
#include <glad/glad.h>
#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLuint CompileShader(GLuint type, const char *source);
GLuint CreateShaderProgram(const char *vertexshadersource,
                           const char *fragmentshadersource);

char *readFile(const char *fileName);
