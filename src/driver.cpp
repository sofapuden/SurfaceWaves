#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>

GLFWwindow* window = nullptr;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = nullptr;
const char *fragmentShaderSource = nullptr;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main(){
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Hello, World!", nullptr, nullptr);
	if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	else{
		std::cout << "Succesfully created GLFW window" << std::endl;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
	else{
		std::cout << "Succesfully initialize GLEW" << std::endl;
	}

	std::ifstream t("./Shaders/vertex.glsl");
	t.seekg(0,std::ios::end);
	size_t size = t.tellg();
	std::string vertexbuffer(size,' ');
	t.seekg(0);
	t.read(&vertexbuffer[0],size);
	t.close();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	vertexShaderSource = vertexbuffer.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	t.open("./Shaders/fragment.glsl");
	t.seekg(0,std::ios::end);
	size = t.tellg();
	std::string fragmentbuffer(size,' ');
	t.seekg(0);
	t.read(&fragmentbuffer[0],size);
	t.close();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaderSource = fragmentbuffer.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	float vertices[] = {
		-0.5f, -0.4f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.1f
	};
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	while(!glfwWindowShouldClose(window)){

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}	
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
	
	glfwDestroyWindow(window);
	glfwTerminate();
}
