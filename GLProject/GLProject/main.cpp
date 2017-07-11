#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

std::string LoadFileToString(const char* filePath)
{
	std::string fileData = "";
	std::ifstream stream(filePath, std::ios::in);

	if(stream.is_open())
	{
		std::string line = "";

		while(getline(stream, line))
		{
			fileData += "\n" + line;
		}

		stream.close();
	}

	return fileData;
}

GLuint LoadShaders(const char* vertShaderPath, const char* fragShaderPath)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertShaderSource = LoadFileToString(vertShaderPath);
	std::string fragShaderSource = LoadFileToString(fragShaderPath);

	const char* rawVertShaderSource = vertShaderSource.c_str();
	const char* rawFragShaderSource = fragShaderSource.c_str();

	glShaderSource(vertShader, 1, &rawVertShaderSource, NULL);
	glShaderSource(fragShader, 1, &rawFragShaderSource, NULL);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	return program;
}

int main(){

	if(glfwInit() == false)
	{
		fprintf(stderr, "GLFW failed to initialize.");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "My OpenGL", NULL, NULL);

	if(!window)
	{
		fprintf(stderr, "window failed to create");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glewExperimental = true;

	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW failed to initialize");
		glfwTerminate();
		return -1;
	}

	// Generate VAO
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	static const GLfloat verts[] = {
		// X, Y, Z
		-1.0f, -1.0f, 0.0f, //bottom left
		1.0f, -1.0f, 0.0f, // bottom right
		0.0f, 1.0f, 0.0f, // top center
	};


	GLuint program = LoadShaders("shader.vertshader", "shader.fragshader");


	// Generate VBO
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while(glfwWindowShouldClose(window) == false);

	//getchar();

	return 0;
}