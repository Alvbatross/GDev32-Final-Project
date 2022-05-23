// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ---------------
// Function declarations
// ---------------

/**
 * @brief Creates a shader program based on the provided file paths for the vertex and fragment shaders.
 * @param[in] vertexShaderFilePath Vertex shader file path
 * @param[in] fragmentShaderFilePath Fragment shader file path
 * @return OpenGL handle to the created shader program
 */
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

/**
 * @brief Creates a shader based on the provided shader type and the path to the file containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderFilePath Path to the file containing the shader source
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath);

/**
 * @brief Creates a shader based on the provided shader type and the string containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderSource Shader source string
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource);

/**
 * @brief Function for handling the event when the size of the framebuffer changed.
 * @param[in] window Reference to the window
 * @param[in] width New width
 * @param[in] height New height
 */
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);

/**
 * Struct containing data about a vertex
 */
struct Vertex
{
	GLfloat x, y, z;	// Position
	GLubyte r, g, b;	// Color
	GLfloat nx, ny, nz; // Normal Vertices
};

glm::mat4 cubeMatrix = glm::mat4(1.0f);
glm::mat4 cubeMatrix2 = glm::mat4(1.0f);
glm::mat4 cubeMatrix3 = glm::mat4(1.0f);
glm::mat4 planeMatrix = glm::mat4(1.0f);
glm::mat4 camera;
glm::mat4 perspective;
/**
 * @brief Main function
 * @return An integer indicating whether the program ended successfully or not.
 * A value of 0 indicates the program ended succesfully, while a non-zero value indicates
 * something wrong happened during execution.
 */
int main()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return 1;
	}

	// Tell GLFW that we prefer to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW that we prefer to use the modern OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tell GLFW to create a window
	int windowWidth = 800;
	int windowHeight = 800;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Lab 3 - Shadow Mapping", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(window);

	// Register the callback function that handles when the framebuffer size has changed
	glfwSetFramebufferSizeCallback(window, FramebufferSizeChangedCallback);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---
	
	// Set up the data for each vertex of the triangle
	Vertex cube[36];
	//             Position				Color				UV
	//Front Face
	cube[0] = { -0.5f, -0.5f, 0.5f,		100, 255, 255};	// Lower-left
	cube[1] = { 0.5f, -0.5f, 0.5f,		100, 255, 255};	// Lower-right
	cube[2] = { 0.5f, 0.5f, 0.5f,		100, 255, 255};	// Upper-right

	cube[3] = { 0.5f, 0.5f, 0.5f,		100, 255, 255};	// Upper-right
	cube[4] = { -0.5f, 0.5f, 0.5f,		100, 255, 255};	// Upper-left
	cube[5] = { -0.5f, -0.5f, 0.5f,		100, 255, 255};	// Lower-left

	//Right Face
	cube[6] = { 0.5f, -0.5f, 0.5f,		100, 255, 255};	// Lower-left
	cube[7] = { 0.5f, -0.5f, -0.5f,	    100, 255, 255};	// Lower-right
	cube[8] = { 0.5f, 0.5f, -0.5f,		100, 255, 255};	// Upper-right

	cube[9] = { 0.5f, 0.5f, -0.5f,		100, 255, 255};	// Upper-right
	cube[10] = { 0.5f, 0.5f, 0.5f,		100, 255, 255};	// Upper-left
	cube[11] = { 0.5f, -0.5f, 0.5f,		100, 255, 255};	// Lower-left

	//Back Face
	cube[12] = { 0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-left
	cube[13] = { -0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-right
	cube[14] = { -0.5f, 0.5f, -0.5f,	100, 255, 255};	// Upper-right

	cube[15] = { -0.5f, 0.5f, -0.5f,	100, 255, 255};	// Upper-right
	cube[16] = { 0.5f, 0.5f, -0.5f,		100, 255, 255};	// Upper-left
	cube[17] = { 0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-left

	//Left Face
	cube[18] = { -0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-left
	cube[19] = { -0.5f, -0.5f, 0.5f,	100, 255, 255};	// Lower-right
	cube[20] = { -0.5f, 0.5f, 0.5f,		100, 255, 255};	// Upper-right

	cube[21] = { -0.5f, 0.5f, 0.5f,		100, 255, 255};	// Upper-right
	cube[22] = { -0.5f, 0.5f, -0.5f,	100, 255, 255};	// Upper-left
	cube[23] = { -0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-left

	//Top Face
	cube[24] = { -0.5f, 0.5f, 0.5f,		100, 255, 255};	// Lower-left
	cube[25] = { 0.5f, 0.5f, 0.5f,		100, 255, 255};	// Lower-right
	cube[26] = { 0.5f, 0.5f, -0.5f,		100, 255, 255};	// Upper-right

	cube[27] = { 0.5f, 0.5f, -0.5f,		100, 255, 255};	// Upper-right
	cube[28] = { -0.5f, 0.5f, -0.5f,	100, 255, 255};	// Upper-left
	cube[29] = { -0.5f, 0.5f, 0.5f,		100, 255, 255};	// Lower-left

	//Bottom Face
	cube[30] = { 0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-left
	cube[31] = { 0.5f, -0.5f, 0.5f,		100, 255, 255};	// Lower-right
	cube[32] = { -0.5f, -0.5f, 0.5f,	100, 255, 255};	// Upper-right

	cube[33] = { -0.5f, -0.5f, 0.5f,	100, 255, 255};	// Upper-right
	cube[34] = { -0.5f, -0.5f, -0.5f,	100, 255, 255};	// Upper-left
	cube[35] = { 0.5f, -0.5f, -0.5f,	100, 255, 255};	// Lower-left


	//set normal for cube

	for (int i = 0; i < 36; i += 3) {
		GLfloat x0 = cube[i].x;
		GLfloat y0 = cube[i].y;
		GLfloat z0 = cube[i].z;

		GLfloat x1 = cube[i + 1].x;
		GLfloat y1 = cube[i + 1].y;
		GLfloat z1 = cube[i + 1].z;

		GLfloat x2 = cube[i + 2].x;
		GLfloat y2 = cube[i + 2].y;
		GLfloat z2 = cube[i + 2].z;

		GLfloat vx0 = x2 - x0;
		GLfloat vy0 = y2 - y0;
		GLfloat vz0 = z2 - z0;

		GLfloat vx1 = x1 - x0;
		GLfloat vy1 = y1 - y0;
		GLfloat vz1 = z1 - z0;

		glm::vec3 cubeNormal = glm::cross(glm::vec3(vx1, vy1, vz1), glm::vec3(vx0, vy0, vz0));

		cube[i].nx = cubeNormal.x;
		cube[i].ny = cubeNormal.y;
		cube[i].nz = cubeNormal.z;
		cube[i + 1].nx = cubeNormal.x;
		cube[i + 1].ny = cubeNormal.y;
		cube[i + 1].nz = cubeNormal.z;
		cube[i + 2].nx = cubeNormal.x;
		cube[i + 2].ny = cubeNormal.y;
		cube[i + 2].nz = cubeNormal.z;
	}

	Vertex plane[6];
	//Plane face
	plane[0] = { -0.5f, 0.f, 0.5f,		160,  82,  45,  0.0f, 1.0f, 0.0f };	// Lower-left
	plane[1] = { 0.5f, 0.f, 0.5f,		160,  82,  45,  0.0f, 1.0f, 0.0f };	// Lower-right
	plane[2] = { 0.5f, 0.f, -0.5f,		160,  82,  45,  0.0f, 1.0f, 0.0f };	// Upper-right

	plane[3] = { 0.5f, 0.f, -0.5f,		160,  82,  45,  0.0f, 1.0f, 0.0f };	// Upper-right
	plane[4] = { -0.5f, 0.f, -0.5f,	    160,  82,  45,  0.0f, 1.0f, 0.0f };	// Upper-left
	plane[5] = { -0.5f, 0.f, 0.5f,		160,  82,  45,  0.0f, 1.0f, 0.0f };	// Lower-left

	
	// Create a vertex buffer object (VBO), and upload our vertices data to the VBO
	GLuint cubeVbo;
	glGenBuffers(1, &cubeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint planeVbo;
	glGenBuffers(1, &planeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, planeVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	

	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.
	GLuint cubeVao;
	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	
	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

	// Vertex attribute 2 - Normal coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));
	glBindVertexArray(0);


	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.
	GLuint planeVao;
	glGenVertexArrays(1, &planeVao);
	glBindVertexArray(planeVao);
	glBindBuffer(GL_ARRAY_BUFFER, planeVbo);

	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

	// Vertex attribute 2 - Normal coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));
	glBindVertexArray(0);
	


	// FRAMEBUFFERS
	//
	//
	GLuint fbo;
	glGenFramebuffers(1, &fbo);

	GLuint fboTex;
	glGenTextures(1, &fboTex);
	glBindTexture(GL_TEXTURE_2D, fboTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)\
	{
		std::cout << "Error! Framebuffer not complete!" << std::endl;
	}

	// Create a shader program
	GLuint program = CreateShaderProgram("main.vsh", "main.fsh");

	GLuint depthshaders = CreateShaderProgram("depthShader.vsh", "depthShader.fsh");

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);

	cubeMatrix = glm::scale(cubeMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	cubeMatrix = glm::translate(cubeMatrix, glm::vec3(0.0f, 0.6f, 0.0f));

	cubeMatrix2 = glm::scale(cubeMatrix2, glm::vec3(0.8f, 0.8f, 0.8f));
	cubeMatrix2 = glm::translate(cubeMatrix2, glm::vec3(4.f, 0.5f, 0.f));

	cubeMatrix3 = glm::scale(cubeMatrix3, glm::vec3(1.0f, 1.0f, 1.0f));
	cubeMatrix3 = glm::translate(cubeMatrix3, glm::vec3(0.0f, 0.5f, 3.5f));
	
	planeMatrix = glm::scale(planeMatrix, glm::vec3(10.0f, 10.0f, 10.0f));

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat angleX = M_PI;
	GLfloat angleY = 0.0f;
	GLfloat camSpeed = 1.0f;
	GLfloat mouseSpeed = 0.5f;

	GLfloat prevTime = glfwGetTime();

	GLdouble xpos, ypos;

	int forwardInput = glfwGetKey(window, GLFW_KEY_W);
	int backwardInput = glfwGetKey(window, GLFW_KEY_S);
	int leftInput = glfwGetKey(window, GLFW_KEY_A);
	int rightInput = glfwGetKey(window, GLFW_KEY_D);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat time = glfwGetTime();
		GLfloat deltaTime = time - prevTime;
		prevTime = time;

		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, (double)windowWidth / 2, (double)windowHeight / 2);

		angleX += mouseSpeed * deltaTime * float(windowWidth / 2 - xpos);
		angleY += mouseSpeed * deltaTime * float(windowHeight / 2 - ypos);

		glm::vec3 cameraTarget(cos(angleY) * sin(angleX), sin(angleY), cos(angleY) * cos(angleX));
		glm::vec3 right(sin(angleX - M_PI / 2.0f), 0.0f, cos(angleX - M_PI / 2.0f));
		glm::vec3 cameraUp = glm::cross(right, cameraTarget);


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraPosition += camSpeed * cameraTarget * deltaTime;
			std::cout << cameraPosition.z << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraPosition -= camSpeed * cameraTarget * deltaTime;
			std::cout << cameraPosition.z << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraPosition -= camSpeed * right * deltaTime;
			std::cout << cameraPosition.x << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraPosition += camSpeed * right * deltaTime;
			std::cout << cameraPosition.x << std::endl;
		}
		
		// FIRST PASS
		// 
		// 
		//
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1024, 1024);
		glUseProgram(depthshaders);
		

		GLint dirLightProjectionUniformLocation = glGetUniformLocation(depthshaders, "lightProjection");
		GLint dirLightViewUniformLocation = glGetUniformLocation(depthshaders, "lightView");
		GLint dirModelMatrix = glGetUniformLocation(depthshaders, "modelMatrix");

		glm::vec3 lightPosition = glm::vec3(5.0f, 5.0f, 5.0f);
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.0f);
		glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		glUniformMatrix4fv(dirLightProjectionUniformLocation, 1, GL_FALSE, glm::value_ptr(lightProjection));
		glUniformMatrix4fv(dirLightViewUniformLocation, 1, GL_FALSE, glm::value_ptr(lightView));
		


		// PLANE
		//
		//
		glBindVertexArray(planeVao);

		GLint planeUniformLocation = glGetUniformLocation(depthshaders, "modelMatrix");
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(planeMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		// CUBE
		// 
		//
		// Use the vertex array object that we created
		glBindVertexArray(cubeVao);

		GLint cubeUniformLocation = glGetUniformLocation(depthshaders, "modelMatrix");

		glUniformMatrix4fv(cubeUniformLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniformMatrix4fv(cubeUniformLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix2));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniformMatrix4fv(cubeUniformLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix3));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		//
		// SECOND PASS 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboTex);
		

		// Use the shader program that we created
		glUseProgram(program);
		

		GLint shadowMapTexUniformLocation = glGetUniformLocation(program, "shadowMap");
		glUniform1i(shadowMapTexUniformLocation, 0);
		

		// PLANE 
		//
		glBindVertexArray(planeVao);

		planeUniformLocation = glGetUniformLocation(program, "modelMatrix");
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(planeMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		// CUBE
		// 
		//
		// Use the vertex array object that we created
		glBindVertexArray(cubeVao);

		cubeUniformLocation = glGetUniformLocation(program, "modelMatrix");
		

		

		glUniformMatrix4fv(cubeUniformLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniformMatrix4fv(cubeUniformLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix2));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniformMatrix4fv(cubeUniformLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix3));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		GLint cameraPositionUniformLocation = glGetUniformLocation(program, "cameraPosition");
		GLint objectSpecUniformLocation = glGetUniformLocation(program, "objectSpec");
		GLint objectShineUniformLocation = glGetUniformLocation(program, "objectShine");
		GLint dirLightDirUniformLocation = glGetUniformLocation(program, "directionalLightDirection");
		
		GLint dirLightAmbientUniformLocation = glGetUniformLocation(program, "lightAmbient");
		GLint dirLightDiffuseUniformLocation = glGetUniformLocation(program, "lightDiffuse");
		GLint dirLightSpecularUniformLocation = glGetUniformLocation(program, "lightSpecular");

		GLint dirLightProjectionUniformLocation2 = glGetUniformLocation(program, "lightProjection");
		GLint dirLightViewUniformLocation2 = glGetUniformLocation(program, "lightView");
		
		glUniform3fv(cameraPositionUniformLocation, 1, glm::value_ptr(cameraPosition));
		glUniform3f(objectSpecUniformLocation, 1.0f, 1.0f, 1.0f);
		glUniform1f(objectShineUniformLocation, 100.f);
		glUniform3f(dirLightDirUniformLocation, -0.25f, -1.0f, -0.25f);

		glUniform3f(dirLightAmbientUniformLocation, 1.f, 0.f, 0.f);
		glUniform3f(dirLightDiffuseUniformLocation, 0.f, 1.f, 0.f);
		glUniform3f(dirLightSpecularUniformLocation, 0.f, 0.f, 1.f);

		glUniformMatrix4fv(dirLightProjectionUniformLocation2, 1, GL_FALSE, glm::value_ptr(lightProjection));
		glUniformMatrix4fv(dirLightViewUniformLocation2, 1, GL_FALSE, glm::value_ptr(lightView));
		
		// Camera computations
		camera = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, cameraUp);
		perspective = glm::perspective(glm::radians(90.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);

		GLint camUniformLocation = glGetUniformLocation(program, "camera");
		GLint perspectiveUniformLocation = glGetUniformLocation(program, "perspective");
		glUniformMatrix4fv(camUniformLocation, 1, GL_FALSE, glm::value_ptr(camera));
		glUniformMatrix4fv(perspectiveUniformLocation, 1, GL_FALSE, glm::value_ptr(perspective));

		glEnable(GL_DEPTH_TEST);

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(window);

		// Tell GLFW to process window events (e.g., input events, window closed events, etc.)
		glfwPollEvents();
	}

	// --- Cleanup ---

	// Make sure to delete the shader program
	glDeleteProgram(program);

	// Delete the VBO that contains our vertices
	glDeleteBuffers(1, &cubeVbo);
	glDeleteBuffers(1, &planeVbo);

	// Delete the vertex array object
	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &planeVao);

	// Remember to tell GLFW to clean itself up before exiting the application
	glfwTerminate();

	return 0;
}

/**
 * @brief Creates a shader program based on the provided file paths for the vertex and fragment shaders.
 * @param[in] vertexShaderFilePath Vertex shader file path
 * @param[in] fragmentShaderFilePath Fragment shader file path
 * @return OpenGL handle to the created shader program
 */
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	GLuint vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilePath);
	GLuint fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	// Check shader program link status
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetProgramInfoLog(program, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "program link error: " << infoLog << std::endl;
	}

	return program;
}

/**
 * @brief Creates a shader based on the provided shader type and the path to the file containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderFilePath Path to the file containing the shader source
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath)
{
	std::ifstream shaderFile(shaderFilePath);
	if (shaderFile.fail())
	{
		std::cerr << "Unable to open shader file: " << shaderFilePath << std::endl;
		return 0;
	}

	std::string shaderSource;
	std::string temp;
	while (std::getline(shaderFile, temp))
	{
		shaderSource += temp + "\n";
	}
	shaderFile.close();

	return CreateShaderFromSource(shaderType, shaderSource);
}

/**
 * @brief Creates a shader based on the provided shader type and the string containing the shader source.
 * @param[in] shaderType Shader type
 * @param[in] shaderSource Shader source string
 * @return OpenGL handle to the created shader
 */
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource)
{
	GLuint shader = glCreateShader(shaderType);

	const char* shaderSourceCStr = shaderSource.c_str();
	GLint shaderSourceLen = static_cast<GLint>(shaderSource.length());
	glShaderSource(shader, 1, &shaderSourceCStr, &shaderSourceLen);
	glCompileShader(shader);

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}

/**
 * @brief Function for handling the event when the size of the framebuffer changed.
 * @param[in] window Reference to the window
 * @param[in] width New width
 * @param[in] height New height
 */
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}
