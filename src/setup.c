#include <glad/glad.h>
#include <setup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readFile(const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    printf("Failed to find file\n");
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);

  /* Read File for Content */
  char *buffer = malloc(sizeof(char) * (size + 1));
  if (buffer == NULL) {
    fclose(fp);
    return NULL;
  }
  size_t read_size = fread(buffer, 1, size, fp);
  if (read_size != size) {
    fclose(fp);
    free(buffer);
    return NULL;
  }

  buffer[size] = '\0';
  fclose(fp);
  return buffer;
}

GLuint CompileShader(GLuint type, const char *source) {
  GLuint shaderObject = 0;
  if (type == GL_VERTEX_SHADER) {
    shaderObject = glCreateShader(GL_VERTEX_SHADER);
  } else if (type == GL_FRAGMENT_SHADER) {
    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  }

  glShaderSource(shaderObject, 1, &source, NULL);
  glCompileShader(shaderObject);
  int success;
  char infoLog[512];
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
  }
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
