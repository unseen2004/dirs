#ifndef GLAD_H
#define GLAD_H

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif

// OpenGL 3.3 constants
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif

// Function declarations for modern OpenGL
extern void (*glGenVertexArrays)(int n, unsigned int *arrays);
extern void (*glBindVertexArray)(unsigned int array);
extern void (*glDeleteVertexArrays)(int n, const unsigned int *arrays);
extern void (*glGenBuffers)(int n, unsigned int *buffers);
extern void (*glBindBuffer)(unsigned int target, unsigned int buffer);
extern void (*glDeleteBuffers)(int n, const unsigned int *buffers);
extern void (*glBufferData)(unsigned int target, long size, const void *data, unsigned int usage);
extern void (*glEnableVertexAttribArray)(unsigned int index);
extern void (*glVertexAttribPointer)(unsigned int index, int size, unsigned int type, unsigned char normalized, int stride, const void *pointer);
extern unsigned int (*glCreateShader)(unsigned int type);
extern void (*glShaderSource)(unsigned int shader, int count, const char* const *string, const int *length);
extern void (*glCompileShader)(unsigned int shader);
extern void (*glGetShaderiv)(unsigned int shader, unsigned int pname, int *params);
extern void (*glGetShaderInfoLog)(unsigned int shader, int bufSize, int *length, char *infoLog);
extern unsigned int (*glCreateProgram)(void);
extern void (*glAttachShader)(unsigned int program, unsigned int shader);
extern void (*glLinkProgram)(unsigned int program);
extern void (*glGetProgramiv)(unsigned int program, unsigned int pname, int *params);
extern void (*glGetProgramInfoLog)(unsigned int program, int bufSize, int *length, char *infoLog);
extern void (*glDeleteShader)(unsigned int shader);
extern void (*glDeleteProgram)(unsigned int program);
extern void (*glUseProgram)(unsigned int program);
extern int (*glGetUniformLocation)(unsigned int program, const char *name);
extern void (*glUniformMatrix4fv)(int location, int count, unsigned char transpose, const float *value);
extern void (*glUniform3fv)(int location, int count, const float *value);
extern void (*glUniform1f)(int location, float v0);
extern void (*glUniform1i)(int location, int v0);

// Basic OpenGL function loader
int gladLoadGL(void);

#ifdef __cplusplus
}
#endif

#endif