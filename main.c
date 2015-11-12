#include "gl_core_3_3.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <stdio.h>

struct renderinfo
{
	int ScrW;
	int ScrH;

	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_GLContext Context;

	GLuint VertShader;
	GLuint FragShader;
	GLuint Program;

	GLuint VBO;
	GLuint EBO;

	struct
	{
		GLint Position;
	} Attrib;

	struct
	{
		GLint Ticks;
	} Uniform;
};

struct renderinfo re;

GLuint R_CreateBuffer(GLenum type, void const *data, GLsizei size)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(type, buffer);
	glBufferData(type, size, data, GL_STATIC_DRAW);
	return buffer;
}

GLuint R_CreateShader(GLenum type, char const *src)
{
	char log[512];
	GLint success;
	GLuint shader;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("Shader compile failed: %s\n", log);
		exit(1);
	}

	return shader;
}

char const *FXT_ReadFile(char const *fname)
{
	FILE *fp;
	char *buf;
	size_t size;

	fp = fopen(fname, "r");
	buf = malloc(4096);
	size = fread(buf, 1, 4096, fp);
	buf[size] = '\0';

	return realloc(buf, size + 1);
}

void R_InitGL()
{
	GLfloat const vertices[] = {
		-1.0, -1.0,
		1.0,  -1.0,
		-1.0,  1.0,
		1.0,   1.0
	};

	GLushort const indices[] = { 0, 1, 2, 3 };

	// Init viewport.
	glViewport(0, 0, re.ScrW, re.ScrH);
	glClearColor(0.2, 0.3, 0.3, 1.0);

	// Init buffers.
	re.VBO = R_CreateBuffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
	re.EBO = R_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices));

	// Init shaders.
	re.VertShader = R_CreateShader(GL_VERTEX_SHADER, FXT_ReadFile("main.vp.glsl"));
	re.FragShader = R_CreateShader(GL_FRAGMENT_SHADER, FXT_ReadFile("main.fp.glsl"));

	re.Program = glCreateProgram();
	glAttachShader(re.Program, re.VertShader);
	glAttachShader(re.Program, re.FragShader);
	glLinkProgram(re.Program);

	// Init uniform and attribute locations.

	re.Uniform.Ticks = glGetUniformLocation(re.Program, "Ticks");
	re.Attrib.Position = glGetAttribLocation(re.Program, "Position");
}

void R_Quit()
{
	// Delete GL bound stuff.
	glDeleteShader(re.VertShader);
	glDeleteShader(re.FragShader);
	glDeleteProgram(re.Program);
	glDeleteBuffers(1, &re.VBO);

	// Delete SDL bound stuff.
	SDL_DestroyWindow(re.Window);
	SDL_Quit();
}

void R_Init()
{
	re.ScrW = 640;
	re.ScrH = 480;

	// Init GL info.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Init window.
	SDL_CreateWindowAndRenderer(re.ScrW, re.ScrH, SDL_WINDOW_SHOWN, &re.Window, &re.Renderer);

	// Init GL context.
	re.Context = SDL_GL_CreateContext(re.Window);
	ogl_LoadFunctions();
	SDL_GL_SetSwapInterval(1);

	R_InitGL();

	atexit(R_Quit);
}

void R_Main()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind shader.
	glUseProgram(re.Program);

	// Send uniforms.
	glUniform1i(re.Uniform.Ticks, SDL_GetTicks());

	// Bind vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, re.VBO);
	glVertexAttribPointer(re.Attrib.Position, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, NULL);
	glEnableVertexAttribArray(re.Attrib.Position);

	// Bind element buffer, draw.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, re.EBO);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL);

	// Disable vertex buffer.
	glDisableVertexAttribArray(re.Attrib.Position);

	// Swap video buffer (send to screen).
	SDL_GL_SwapWindow(re.Window);
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
	SDL_Event ev;

	SDL_Init(SDL_INIT_EVERYTHING);
	R_Init();

	for(;;)
	{
		while(SDL_PollEvent(&ev) != 0)
		{
			if(ev.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		R_Main();
	}

	return 0;
}
