#include "glad.h"
#include <GL/gl.h>
#include <dlfcn.h>
#include <stdio.h>

// Function pointers
void (*glGenVertexArrays)(int n, unsigned int *arrays) = NULL;
void (*glBindVertexArray)(unsigned int array) = NULL;
void (*glDeleteVertexArrays)(int n, const unsigned int *arrays) = NULL;
void (*glGenBuffers)(int n, unsigned int *buffers) = NULL;
void (*glBindBuffer)(unsigned int target, unsigned int buffer) = NULL;
void (*glDeleteBuffers)(int n, const unsigned int *buffers) = NULL;
void (*glBufferData)(unsigned int target, long size, const void *data, unsigned int usage) = NULL;
void (*glEnableVertexAttribArray)(unsigned int index) = NULL;
void (*glVertexAttribPointer)(unsigned int index, int size, unsigned int type, unsigned char normalized, int stride, const void *pointer) = NULL;
unsigned int (*glCreateShader)(unsigned int type) = NULL;
void (*glShaderSource)(unsigned int shader, int count, const char* const *string, const int *length) = NULL;
void (*glCompileShader)(unsigned int shader) = NULL;
void (*glGetShaderiv)(unsigned int shader, unsigned int pname, int *params) = NULL;
void (*glGetShaderInfoLog)(unsigned int shader, int bufSize, int *length, char *infoLog) = NULL;
unsigned int (*glCreateProgram)(void) = NULL;
void (*glAttachShader)(unsigned int program, unsigned int shader) = NULL;
void (*glLinkProgram)(unsigned int program) = NULL;
void (*glGetProgramiv)(unsigned int program, unsigned int pname, int *params) = NULL;
void (*glGetProgramInfoLog)(unsigned int program, int bufSize, int *length, char *infoLog) = NULL;
void (*glDeleteShader)(unsigned int shader) = NULL;
void (*glDeleteProgram)(unsigned int program) = NULL;
void (*glUseProgram)(unsigned int program) = NULL;
int (*glGetUniformLocation)(unsigned int program, const char *name) = NULL;
void (*glUniformMatrix4fv)(int location, int count, unsigned char transpose, const float *value) = NULL;
void (*glUniform3fv)(int location, int count, const float *value) = NULL;
void (*glUniform1f)(int location, float v0) = NULL;
void (*glUniform1i)(int location, int v0) = NULL;

static void* get_proc_address(const char* name) {
    static void* libgl = NULL;
    if (!libgl) {
        libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
        if (!libgl) {
            libgl = dlopen("libGL.so", RTLD_LAZY | RTLD_GLOBAL);
        }
    }
    if (libgl) {
        return dlsym(libgl, name);
    }
    return NULL;
}

int gladLoadGL(void) {
    // Load function pointers
    glGenVertexArrays = (void (*)(int, unsigned int*))get_proc_address("glGenVertexArrays");
    glBindVertexArray = (void (*)(unsigned int))get_proc_address("glBindVertexArray");
    glDeleteVertexArrays = (void (*)(int, const unsigned int*))get_proc_address("glDeleteVertexArrays");
    glGenBuffers = (void (*)(int, unsigned int*))get_proc_address("glGenBuffers");
    glBindBuffer = (void (*)(unsigned int, unsigned int))get_proc_address("glBindBuffer");
    glDeleteBuffers = (void (*)(int, const unsigned int*))get_proc_address("glDeleteBuffers");
    glBufferData = (void (*)(unsigned int, long, const void*, unsigned int))get_proc_address("glBufferData");
    glEnableVertexAttribArray = (void (*)(unsigned int))get_proc_address("glEnableVertexAttribArray");
    glVertexAttribPointer = (void (*)(unsigned int, int, unsigned int, unsigned char, int, const void*))get_proc_address("glVertexAttribPointer");
    glCreateShader = (unsigned int (*)(unsigned int))get_proc_address("glCreateShader");
    glShaderSource = (void (*)(unsigned int, int, const char* const*, const int*))get_proc_address("glShaderSource");
    glCompileShader = (void (*)(unsigned int))get_proc_address("glCompileShader");
    glGetShaderiv = (void (*)(unsigned int, unsigned int, int*))get_proc_address("glGetShaderiv");
    glGetShaderInfoLog = (void (*)(unsigned int, int, int*, char*))get_proc_address("glGetShaderInfoLog");
    glCreateProgram = (unsigned int (*)(void))get_proc_address("glCreateProgram");
    glAttachShader = (void (*)(unsigned int, unsigned int))get_proc_address("glAttachShader");
    glLinkProgram = (void (*)(unsigned int))get_proc_address("glLinkProgram");
    glGetProgramiv = (void (*)(unsigned int, unsigned int, int*))get_proc_address("glGetProgramiv");
    glGetProgramInfoLog = (void (*)(unsigned int, int, int*, char*))get_proc_address("glGetProgramInfoLog");
    glDeleteShader = (void (*)(unsigned int))get_proc_address("glDeleteShader");
    glDeleteProgram = (void (*)(unsigned int))get_proc_address("glDeleteProgram");
    glUseProgram = (void (*)(unsigned int))get_proc_address("glUseProgram");
    glGetUniformLocation = (int (*)(unsigned int, const char*))get_proc_address("glGetUniformLocation");
    glUniformMatrix4fv = (void (*)(int, int, unsigned char, const float*))get_proc_address("glUniformMatrix4fv");
    glUniform3fv = (void (*)(int, int, const float*))get_proc_address("glUniform3fv");
    glUniform1f = (void (*)(int, float))get_proc_address("glUniform1f");
    glUniform1i = (void (*)(int, int))get_proc_address("glUniform1i");
    
    // Check if we got the essential functions
    if (!glGenVertexArrays || !glCreateShader || !glCreateProgram) {
        printf("Failed to load essential OpenGL functions\n");
        return 0;
    }
    
    return 1;
}