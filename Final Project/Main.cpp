// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <irrklang/irrKlang.h>

using namespace irrklang;

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
	GLfloat u, v;		// UV coordinates
	GLfloat nx, ny, nz; // Normal Vertices
};

glm::mat4 floorTile01 = glm::mat4(1.0f);

glm::mat4 wallTileR01 = glm::mat4(1.0f); glm::mat4 wallTileR02 = glm::mat4(1.0f); glm::mat4 wallTileR03 = glm::mat4(1.0f);
glm::mat4 wallTileR04 = glm::mat4(1.0f); glm::mat4 wallTileR05 = glm::mat4(1.0f); glm::mat4 wallTileR06 = glm::mat4(1.0f);
glm::mat4 wallTileR07 = glm::mat4(1.0f); glm::mat4 wallTileR08 = glm::mat4(1.0f); glm::mat4 wallTileR09 = glm::mat4(1.0f);
glm::mat4 wallTileR10 = glm::mat4(1.0f); glm::mat4 wallTileR11 = glm::mat4(1.0f); glm::mat4 wallTileR12 = glm::mat4(1.0f);
glm::mat4 wallTileR13 = glm::mat4(1.0f); glm::mat4 wallTileR14 = glm::mat4(1.0f); glm::mat4 wallTileR15 = glm::mat4(1.0f);
glm::mat4 wallTileR16 = glm::mat4(1.0f); glm::mat4 wallTileR17 = glm::mat4(1.0f); glm::mat4 wallTileR18 = glm::mat4(1.0f);
glm::mat4 wallTileR19 = glm::mat4(1.0f); glm::mat4 wallTileR20 = glm::mat4(1.0f); glm::mat4 wallTileR21 = glm::mat4(1.0f);
glm::mat4 wallTileR22 = glm::mat4(1.0f); glm::mat4 wallTileR23 = glm::mat4(1.0f); glm::mat4 wallTileR24 = glm::mat4(1.0f);
glm::mat4 wallTileR25 = glm::mat4(1.0f); glm::mat4 wallTileR26 = glm::mat4(1.0f); glm::mat4 wallTileR27 = glm::mat4(1.0f);
glm::mat4 wallTileR28 = glm::mat4(1.0f); glm::mat4 wallTileR29 = glm::mat4(1.0f); glm::mat4 wallTileR30 = glm::mat4(1.0f);
glm::mat4 wallTileR31 = glm::mat4(1.0f); glm::mat4 wallTileR32 = glm::mat4(1.0f); glm::mat4 wallTileR33 = glm::mat4(1.0f);
glm::mat4 wallTileR34 = glm::mat4(1.0f); glm::mat4 wallTileR35 = glm::mat4(1.0f); glm::mat4 wallTileR36 = glm::mat4(1.0f);
glm::mat4 wallTileR37 = glm::mat4(1.0f); glm::mat4 wallTileR38 = glm::mat4(1.0f); glm::mat4 wallTileR39 = glm::mat4(1.0f);

glm::mat4 wallTileL01 = glm::mat4(1.0f); glm::mat4 wallTileL02 = glm::mat4(1.0f); glm::mat4 wallTileL03 = glm::mat4(1.0f); 
glm::mat4 wallTileL04 = glm::mat4(1.0f); glm::mat4 wallTileL05 = glm::mat4(1.0f); glm::mat4 wallTileL06 = glm::mat4(1.0f); 
glm::mat4 wallTileL07 = glm::mat4(1.0f); glm::mat4 wallTileL08 = glm::mat4(1.0f); glm::mat4 wallTileL09 = glm::mat4(1.0f); 
glm::mat4 wallTileL10 = glm::mat4(1.0f); glm::mat4 wallTileL11 = glm::mat4(1.0f); glm::mat4 wallTileL12 = glm::mat4(1.0f); 
glm::mat4 wallTileL13 = glm::mat4(1.0f); glm::mat4 wallTileL14 = glm::mat4(1.0f); glm::mat4 wallTileL15 = glm::mat4(1.0f); 
glm::mat4 wallTileL16 = glm::mat4(1.0f); glm::mat4 wallTileL17 = glm::mat4(1.0f); glm::mat4 wallTileL18 = glm::mat4(1.0f); 
glm::mat4 wallTileL19 = glm::mat4(1.0f); glm::mat4 wallTileL20 = glm::mat4(1.0f); glm::mat4 wallTileL21 = glm::mat4(1.0f); 
glm::mat4 wallTileL22 = glm::mat4(1.0f); glm::mat4 wallTileL23 = glm::mat4(1.0f); glm::mat4 wallTileL24 = glm::mat4(1.0f); 
glm::mat4 wallTileL25 = glm::mat4(1.0f); glm::mat4 wallTileL26 = glm::mat4(1.0f); glm::mat4 wallTileL27 = glm::mat4(1.0f); 
glm::mat4 wallTileL28 = glm::mat4(1.0f); glm::mat4 wallTileL29 = glm::mat4(1.0f); glm::mat4 wallTileL30 = glm::mat4(1.0f); 
glm::mat4 wallTileL31 = glm::mat4(1.0f); glm::mat4 wallTileL32 = glm::mat4(1.0f); glm::mat4 wallTileL33 = glm::mat4(1.0f); 
glm::mat4 wallTileL34 = glm::mat4(1.0f); glm::mat4 wallTileL35 = glm::mat4(1.0f); glm::mat4 wallTileL36 = glm::mat4(1.0f); 
glm::mat4 wallTileL37 = glm::mat4(1.0f); glm::mat4 wallTileL38 = glm::mat4(1.0f); glm::mat4 wallTileL39 = glm::mat4(1.0f); 
glm::mat4 wallTileL40 = glm::mat4(1.0f); glm::mat4 wallTileL41 = glm::mat4(1.0f); glm::mat4 wallTileL42 = glm::mat4(1.0f);
glm::mat4 wallTileL43 = glm::mat4(1.0f); glm::mat4 wallTileL44 = glm::mat4(1.0f); glm::mat4 wallTileL45 = glm::mat4(1.0f);
glm::mat4 wallTileL46 = glm::mat4(1.0f); glm::mat4 wallTileL47 = glm::mat4(1.0f); glm::mat4 wallTileL48 = glm::mat4(1.0f);

glm::mat4 camera;
glm::mat4 perspective;

ISoundEngine* SoundEngine = createIrrKlangDevice();
ISoundEngine* sfxEngine = createIrrKlangDevice();
ISound* sfx;

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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Final Project - Horror game oh no", nullptr, nullptr);
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
	cube[0] = { -0.5f, -0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-left
	cube[1] = { 0.5f, -0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-right
	cube[2] = { 0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right

	cube[3] = { 0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right
	cube[4] = { -0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-left
	cube[5] = { -0.5f, -0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-left

	//Right Face
	cube[6] = { 0.5f, -0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-left
	cube[7] = { 0.5f, -0.5f, -0.5f,	    100, 255, 255,   0.0f, 0.0f,};	// Lower-right
	cube[8] = { 0.5f, 0.5f, -0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right

	cube[9] = { 0.5f, 0.5f, -0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right
	cube[10] = { 0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-left
	cube[11] = { 0.5f, -0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-left

	//Back Face
	cube[12] = { 0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-left
	cube[13] = { -0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-right
	cube[14] = { -0.5f, 0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-right

	cube[15] = { -0.5f, 0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-right
	cube[16] = { 0.5f, 0.5f, -0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-left
	cube[17] = { 0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-left

	//Left Face
	cube[18] = { -0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-left
	cube[19] = { -0.5f, -0.5f, 0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-right
	cube[20] = { -0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right

	cube[21] = { -0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right
	cube[22] = { -0.5f, 0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-left
	cube[23] = { -0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-left

	//Top Face
	cube[24] = { -0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-left
	cube[25] = { 0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-right
	cube[26] = { 0.5f, 0.5f, -0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right

	cube[27] = { 0.5f, 0.5f, -0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Upper-right
	cube[28] = { -0.5f, 0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-left
	cube[29] = { -0.5f, 0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-left

	//Bottom Face
	cube[30] = { 0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-left
	cube[31] = { 0.5f, -0.5f, 0.5f,		100, 255, 255,   0.0f, 0.0f,};	// Lower-right
	cube[32] = { -0.5f, -0.5f, 0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-right

	cube[33] = { -0.5f, -0.5f, 0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-right
	cube[34] = { -0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Upper-left
	cube[35] = { 0.5f, -0.5f, -0.5f,	100, 255, 255,   0.0f, 0.0f,};	// Lower-left


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
	plane[0] = { -0.5f, 0.f, 0.5f,		255,  255,  255,    0.0f, 1.0f,    0.0f, 1.0f, 0.0f };	// Lower-left
	plane[1] = { 0.5f, 0.f, 0.5f,		255,  255,  255,    0.0f, 0.0f,    0.0f, 1.0f, 0.0f };	// Lower-right
	plane[2] = { 0.5f, 0.f, -0.5f,		255,  255,  255,    1.0f, 0.0f,    0.0f, 1.0f, 0.0f };	// Upper-right

	plane[3] = { 0.5f, 0.f, -0.5f,		255,  255,  255,    1.0f, 0.0f,    0.0f, 1.0f, 0.0f };	// Upper-right
	plane[4] = { -0.5f, 0.f, -0.5f,	    255,  255,  255,    1.0f, 1.0f,    0.0f, 1.0f, 0.0f };	// Upper-left
	plane[5] = { -0.5f, 0.f, 0.5f,		255,  255,  255,    0.0f, 1.0f,    0.0f, 1.0f, 0.0f };	// Lower-left

	Vertex floor[6];
	floor[0] = { -0.5f, 0.f, 0.5f,		255,  255,  255,    0.0f, 10.0f,    0.0f, 1.0f, 0.0f };	// Lower-left
	floor[1] = { 0.5f, 0.f, 0.5f,		255,  255,  255,    0.0f, 0.0f,    0.0f, 1.0f, 0.0f };	// Lower-right
	floor[2] = { 0.5f, 0.f, -0.5f,		255,  255,  255,    10.0f, 0.0f,    0.0f, 1.0f, 0.0f };	// Upper-right

	floor[3] = { 0.5f, 0.f, -0.5f,		255,  255,  255,    10.0f, 0.0f,    0.0f, 1.0f, 0.0f };	// Upper-right
	floor[4] = { -0.5f, 0.f, -0.5f,	    255,  255,  255,    10.0f, 10.0f,    0.0f, 1.0f, 0.0f };	// Upper-left
	floor[5] = { -0.5f, 0.f, 0.5f,		255,  255,  255,    0.0f, 10.0f,    0.0f, 1.0f, 0.0f };	// Lower-left


	Vertex skybox[36];
	skybox[0] = { -1.0f,  1.0f, -1.0f };
	skybox[1] = { -1.0f, -1.0f, -1.0f };
	skybox[2] = { 1.0f, -1.0f, -1.0f };
	skybox[3] = { 1.0f, -1.0f, -1.0f };
	skybox[4] = { 1.0f,  1.0f, -1.0f };
	skybox[5] = { -1.0f,  1.0f, -1.0f };

	skybox[6] = { -1.0f, -1.0f,  1.0f };
	skybox[7] = { -1.0f, -1.0f, -1.0f };
	skybox[8] = { -1.0f,  1.0f, -1.0f };
	skybox[9] = { -1.0f,  1.0f, -1.0f };
	skybox[10] = { -1.0f,  1.0f,  1.0f };
	skybox[11] = { -1.0f, -1.0f,  1.0f };

	skybox[12] = { 1.0f, -1.0f, -1.0f };
	skybox[13] = { 1.0f, -1.0f,  1.0f };
	skybox[14] = { 1.0f,  1.0f,  1.0f };
	skybox[15] = { 1.0f,  1.0f,  1.0f };
	skybox[16] = { 1.0f,  1.0f, -1.0f };
	skybox[17] = { 1.0f, -1.0f, -1.0f };

	skybox[18] = { -1.0f, -1.0f,  1.0f };
	skybox[19] = { -1.0f,  1.0f,  1.0f };
	skybox[20] = { 1.0f,  1.0f,  1.0f };
	skybox[21] = { 1.0f,  1.0f,  1.0f };
	skybox[22] = { 1.0f, -1.0f,  1.0f };
	skybox[23] = { -1.0f, -1.0f,  1.0f };

	skybox[24] = { -1.0f,  1.0f, -1.0f };
	skybox[25] = { 1.0f,  1.0f, -1.0f };
	skybox[26] = { 1.0f,  1.0f,  1.0f };
	skybox[27] = { 1.0f,  1.0f,  1.0f };
	skybox[28] = { -1.0f,  1.0f,  1.0f };
	skybox[29] = { -1.0f,  1.0f, -1.0f };

	skybox[30] = { -1.0f, -1.0f, -1.0f };
	skybox[31] = { -1.0f, -1.0f,  1.0f };
	skybox[32] = { 1.0f, -1.0f, -1.0f };
	skybox[33] = { 1.0f, -1.0f, -1.0f };
	skybox[34] = { -1.0f, -1.0f,  1.0f };
	skybox[35] = { 1.0f, -1.0f,  1.0f };


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

	GLuint floorVbo;
	glGenBuffers(1, &floorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, floorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint skyboxVbo;
	glGenBuffers(1, &skyboxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), skybox, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.

	// cube
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

	// Vertex attribute 2 - UV coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));

	// Vertex attribute 3 - Normal coordinates
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));
	glBindVertexArray(0);


	// plane
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

	// Vertex attribute 2 - UV coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));

	// Vertex attribute 3 - Normal coordinates
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));
	
	glBindVertexArray(0);

	// floor
	GLuint floorVao;
	glGenVertexArrays(1, &floorVao);
	glBindVertexArray(floorVao);
	glBindBuffer(GL_ARRAY_BUFFER, floorVbo);

	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

	// Vertex attribute 2 - UV coordinate
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));

	// Vertex attribute 3 - Normal coordinates
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));

	glBindVertexArray(0);

	// Skybox
	GLuint skyboxVao;
	glGenVertexArrays(1, &skyboxVao);
	glBindVertexArray(skyboxVao);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
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

	GLuint skyboxshaders = CreateShaderProgram("skyboxShader.vsh", "skyboxShader.fsh");

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);


	// Create a variable that will contain the ID for our texture,
	// and use glGenTextures() to generate the texture itself
	GLuint wallTex;
	glGenTextures(1, &wallTex);

	// --- Load our image using stb_image ---

	// Im image-space (pixels), (0, 0) is the upper-left corner of the image
	// However, in u-v coordinates, (0, 0) is the lower-left corner of the image
	// This means that the image will appear upside-down when we use the image data as is
	// This function tells stbi to flip the image vertically so that it is not upside-down when we use it
	stbi_set_flip_vertically_on_load(true);

	// 'imageWidth' and imageHeight will contain the width and height of the loaded image respectively
	int imageWidth, imageHeight, numChannels;

	// Read the image data and store it in an unsigned char array
	unsigned char* imageData = stbi_load("BrickWallTex.jpg", &imageWidth, &imageHeight, &numChannels, 0);

	// Make sure that we actually loaded the image before uploading the data to the GPU
	if (imageData != nullptr)
	{
		// Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
		glBindTexture(GL_TEXTURE_2D, wallTex);

		// Set the filtering methods for magnification and minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Set the wrapping method for the s-axis (x-axis) and t-axis (y-axis)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload the image data to GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// If we set minification to use mipmaps, we can tell OpenGL to generate the mipmaps for us
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Once we have copied the data over to the GPU, we can delete
		// the data on the CPU side, since we won't be using it anymore
		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	// Create a variable that will contain the ID for our texture,
	// and use glGenTextures() to generate the texture itself
	GLuint floorTex;
	glGenTextures(1, &floorTex);

	// --- Load our image using stb_image ---

	// Im image-space (pixels), (0, 0) is the upper-left corner of the image
	// However, in u-v coordinates, (0, 0) is the lower-left corner of the image
	// This means that the image will appear upside-down when we use the image data as is
	// This function tells stbi to flip the image vertically so that it is not upside-down when we use it
	stbi_set_flip_vertically_on_load(true);


	// Read the image data and store it in an unsigned char array
	imageData = stbi_load("BrownTileTex.jpg", &imageWidth, &imageHeight, &numChannels, 0);

	// Make sure that we actually loaded the image before uploading the data to the GPU
	if (imageData != nullptr)
	{
		// Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
		glBindTexture(GL_TEXTURE_2D, floorTex);

		// Set the filtering methods for magnification and minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Set the wrapping method for the s-axis (x-axis) and t-axis (y-axis)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload the image data to GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// If we set minification to use mipmaps, we can tell OpenGL to generate the mipmaps for us
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Once we have copied the data over to the GPU, we can delete
		// the data on the CPU side, since we won't be using it anymore
		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	
	// Skybox textures code
	std::vector<std::string> skyboxFaces{
		"night2.jpg",
		"night2.jpg",
		"nightmoon.jpg",
		"night2.jpg",
		"night4.jpg",
		"night4.jpg"
	};

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	
	for (unsigned int i = 0; i < skyboxFaces.size(); i++)
	{
		//
		unsigned char* data = stbi_load(skyboxFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << skyboxFaces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	unsigned int skyboxTexture = textureID;

	//
	// Skybox code - end

	glEnable(GL_DEPTH_TEST);

	floorTile01 = glm::scale(floorTile01, glm::vec3(9.0f, 1.0f, 9.0f));

	wallTileL01 = glm::translate(wallTileL01, glm::vec3(-0.5f, 0.5f, 0.0f));
	wallTileL01 = glm::rotate(wallTileL01, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL02 = glm::translate(wallTileL02, glm::vec3(-0.5f, 0.5f, -1.0f));
	wallTileL02 = glm::rotate(wallTileL02, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL03 = glm::translate(wallTileL03, glm::vec3(-0.5f, 0.5f, -2.0f));
	wallTileL03 = glm::rotate(wallTileL03, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL04 = glm::translate(wallTileL04, glm::vec3(-0.5f, 0.5f, -3.0f));
	wallTileL04 = glm::rotate(wallTileL04, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL05 = glm::translate(wallTileL05, glm::vec3(-1.0f, 0.5f, -3.5f));
	wallTileL05 = glm::rotate(wallTileL05, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL05 = glm::rotate(wallTileL05, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL06 = glm::translate(wallTileL06, glm::vec3(-0.5f, 0.5f, 2.0f));
	wallTileL06 = glm::rotate(wallTileL06, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL07 = glm::translate(wallTileL07, glm::vec3(-0.5f, 0.5f, 3.0f));
	wallTileL07 = glm::rotate(wallTileL07, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL08 = glm::translate(wallTileL08, glm::vec3(-0.5f, 0.5f, 4.0f));
	wallTileL08 = glm::rotate(wallTileL08, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL09 = glm::translate(wallTileL09, glm::vec3(-0.5f, 0.5f, 5.0f));
	wallTileL09 = glm::rotate(wallTileL09, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));


    // North West Quadrant
	wallTileL10 = glm::translate(wallTileL10, glm::vec3(-2.0f, 0.5f, -3.5f));
	wallTileL10 = glm::rotate(wallTileL10, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL10 = glm::rotate(wallTileL10, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	wallTileL11 = glm::translate(wallTileL11, glm::vec3(-2.5f, 0.5f, -3.0f));
	wallTileL11 = glm::rotate(wallTileL11, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL12 = glm::translate(wallTileL12, glm::vec3(-2.5f, 0.5f, -2.0f));
	wallTileL12 = glm::rotate(wallTileL12, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL13 = glm::translate(wallTileL13, glm::vec3(-2.0f, 0.5f, -1.5f));
	wallTileL13 = glm::rotate(wallTileL13, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL13 = glm::rotate(wallTileL13, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL14 = glm::translate(wallTileL14, glm::vec3(-2.0f, 0.5f, -0.5f));
	wallTileL14 = glm::rotate(wallTileL14, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL14 = glm::rotate(wallTileL14, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	wallTileL15 = glm::translate(wallTileL15, glm::vec3(-1.5f, 0.5f, -1.0f));
	wallTileL15 = glm::rotate(wallTileL15, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL16 = glm::translate(wallTileL16, glm::vec3(-3.0f, 0.5f, -0.5f));
	wallTileL16 = glm::rotate(wallTileL16, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL16 = glm::rotate(wallTileL16, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL17 = glm::translate(wallTileL17, glm::vec3(-4.0f, 0.5f, -0.5f));
	wallTileL17 = glm::rotate(wallTileL17, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL17 = glm::rotate(wallTileL17, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL18 = glm::translate(wallTileL18, glm::vec3(-4.5f, 0.5f, -1.0f));
	wallTileL18 = glm::rotate(wallTileL18, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL19 = glm::translate(wallTileL19, glm::vec3(-4.0f, 0.5f, -1.5f));
	wallTileL19 = glm::rotate(wallTileL19, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL19 = glm::rotate(wallTileL19, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL20 = glm::translate(wallTileL20, glm::vec3(-3.5f, 0.5f, -2.0f));
	wallTileL20 = glm::rotate(wallTileL20, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL21 = glm::translate(wallTileL21, glm::vec3(-3.5f, 0.5f, -3.0f));
	wallTileL21 = glm::rotate(wallTileL21, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL22 = glm::translate(wallTileL22, glm::vec3(-3.5f, 0.5f, -4.0f));
	wallTileL22 = glm::rotate(wallTileL22, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL23 = glm::translate(wallTileL23, glm::vec3(-3.0f, 0.5f, -4.5f));
	wallTileL23 = glm::rotate(wallTileL23, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL23 = glm::rotate(wallTileL23, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL24 = glm::translate(wallTileL24, glm::vec3(-2.0f, 0.5f, -4.5f));
	wallTileL24 = glm::rotate(wallTileL24, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL24 = glm::rotate(wallTileL24, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL25 = glm::translate(wallTileL25, glm::vec3(-1.0f, 0.5f, -4.5f));
	wallTileL25 = glm::rotate(wallTileL25, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL25 = glm::rotate(wallTileL25, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL26 = glm::translate(wallTileL26, glm::vec3(0.0f, 0.5f, -4.5f));
	wallTileL26 = glm::rotate(wallTileL26, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL26 = glm::rotate(wallTileL26, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// END of North West Quadrant

	// South West Quadrant
	wallTileL27 = glm::translate(wallTileL27, glm::vec3(-1.0f, 0.5f, 0.5f));
	wallTileL27 = glm::rotate(wallTileL27, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL27 = glm::rotate(wallTileL27, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL28 = glm::translate(wallTileL28, glm::vec3(-2.0f, 0.5f, 0.5f));
	wallTileL28 = glm::rotate(wallTileL28, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL28 = glm::rotate(wallTileL28, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL29 = glm::translate(wallTileL29, glm::vec3(-2.5f, 0.5f, 1.0f));
	wallTileL29 = glm::rotate(wallTileL29, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL30 = glm::translate(wallTileL30, glm::vec3(-2.5f, 0.5f, 2.0f));
	wallTileL30 = glm::rotate(wallTileL30, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL31 = glm::translate(wallTileL31, glm::vec3(-2.5f, 0.5f, 3.0f));
	wallTileL31 = glm::rotate(wallTileL31, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL32 = glm::translate(wallTileL32, glm::vec3(-3.0f, 0.5f, 3.5f));
	wallTileL32 = glm::rotate(wallTileL32, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL32 = glm::rotate(wallTileL32, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL33 = glm::translate(wallTileL33, glm::vec3(-3.5f, 0.5f, 3.0f));
	wallTileL33 = glm::rotate(wallTileL33, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL34 = glm::translate(wallTileL34, glm::vec3(-3.5f, 0.5f, 2.0f));
	wallTileL34 = glm::rotate(wallTileL34, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL35 = glm::translate(wallTileL35, glm::vec3(-3.5f, 0.5f, 1.0f));
	wallTileL35 = glm::rotate(wallTileL35, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL36 = glm::translate(wallTileL36, glm::vec3(-4.0f, 0.5f, 0.5f));
	wallTileL36 = glm::rotate(wallTileL36, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL36 = glm::rotate(wallTileL36, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL37 = glm::translate(wallTileL37, glm::vec3(-4.5f, 0.5f, 1.0f));
	wallTileL37 = glm::rotate(wallTileL37, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL38 = glm::translate(wallTileL38, glm::vec3(-4.5f, 0.5f, 2.0f));
	wallTileL38 = glm::rotate(wallTileL38, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL39 = glm::translate(wallTileL39, glm::vec3(-4.5f, 0.5f, 3.0f));
	wallTileL39 = glm::rotate(wallTileL39, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL40 = glm::translate(wallTileL40, glm::vec3(-4.5f, 0.5f, 4.0f));
	wallTileL40 = glm::rotate(wallTileL40, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL41 = glm::translate(wallTileL41, glm::vec3(-4.0f, 0.5f, 4.5f));
	wallTileL41 = glm::rotate(wallTileL41, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL41 = glm::rotate(wallTileL41, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL42 = glm::translate(wallTileL42, glm::vec3(-3.0f, 0.5f, 4.5f));
	wallTileL42 = glm::rotate(wallTileL42, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL42 = glm::rotate(wallTileL42, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL43 = glm::translate(wallTileL43, glm::vec3(-2.0f, 0.5f, 4.5f));
	wallTileL43 = glm::rotate(wallTileL43, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL43 = glm::rotate(wallTileL43, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileL44 = glm::translate(wallTileL44, glm::vec3(-1.5f, 0.5f, 4.0f));
	wallTileL44 = glm::rotate(wallTileL44, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL45 = glm::translate(wallTileL45, glm::vec3(-1.5f, 0.5f, 3.0f));
	wallTileL45 = glm::rotate(wallTileL45, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL46 = glm::translate(wallTileL46, glm::vec3(-1.5f, 0.5f, 2.0f));
	wallTileL46 = glm::rotate(wallTileL46, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileL47 = glm::translate(wallTileL47, glm::vec3(-1.0f, 0.5f, 1.5f));
	wallTileL47 = glm::rotate(wallTileL47, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileL47 = glm::rotate(wallTileL47, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // End of South west quadrant and left side

	// Right side
	wallTileR01 = glm::translate(wallTileR01, glm::vec3(0.5f, 0.5f, 0.0f));
	wallTileR01 = glm::rotate(wallTileR01, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR02 = glm::translate(wallTileR02, glm::vec3(0.5f, 0.5f, -1.0f));
	wallTileR02 = glm::rotate(wallTileR02, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR03 = glm::translate(wallTileR03, glm::vec3(0.5f, 0.5f, -2.0f));
	wallTileR03 = glm::rotate(wallTileR03, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR04 = glm::translate(wallTileR04, glm::vec3(0.5f, 0.5f, -3.0f));
	wallTileR04 = glm::rotate(wallTileR04, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR05 = glm::translate(wallTileR05, glm::vec3(0.5f, 0.5f, -4.0f));
	wallTileR05 = glm::rotate(wallTileR05, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR06 = glm::translate(wallTileR06, glm::vec3(0.5f, 0.5f, 1.0f));
	wallTileR06 = glm::rotate(wallTileR06, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR07 = glm::translate(wallTileR07, glm::vec3(0.5f, 0.5f, 2.0f));
	wallTileR07 = glm::rotate(wallTileR07, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR08 = glm::translate(wallTileR08, glm::vec3(0.5f, 0.5f, 4.0f));
	wallTileR08 = glm::rotate(wallTileR08, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR09 = glm::translate(wallTileR09, glm::vec3(0.5f, 0.5f, 5.0f));
	wallTileR09 = glm::rotate(wallTileR09, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR10 = glm::translate(wallTileR10, glm::vec3(0.0f, 0.5f, 4.5f));
	wallTileR10 = glm::rotate(wallTileR10, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR10 = glm::rotate(wallTileR10, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR11 = glm::translate(wallTileR11, glm::vec3(1.0f, 0.5f, 2.5f));
	wallTileR11 = glm::rotate(wallTileR11, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR11 = glm::rotate(wallTileR11, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR12 = glm::translate(wallTileR12, glm::vec3(2.0f, 0.5f, 2.5f));
	wallTileR12 = glm::rotate(wallTileR12, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR12 = glm::rotate(wallTileR12, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR13 = glm::translate(wallTileR13, glm::vec3(3.0f, 0.5f, 2.5f));
	wallTileR13 = glm::rotate(wallTileR13, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR13 = glm::rotate(wallTileR13, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR14 = glm::translate(wallTileR14, glm::vec3(3.5f, 0.5f, 2.0f));
	wallTileR14 = glm::rotate(wallTileR14, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR15 = glm::translate(wallTileR15, glm::vec3(3.5f, 0.5f, 1.0f));
	wallTileR15 = glm::rotate(wallTileR15, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR16 = glm::translate(wallTileR16, glm::vec3(3.5f, 0.5f, 0.0f));
	wallTileR16 = glm::rotate(wallTileR16, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR17 = glm::translate(wallTileR17, glm::vec3(3.0f, 0.5f, -0.5f));
	wallTileR17 = glm::rotate(wallTileR17, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR17 = glm::rotate(wallTileR17, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR18 = glm::translate(wallTileR18, glm::vec3(2.0f, 0.5f, -0.5f));
	wallTileR18 = glm::rotate(wallTileR18, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR18 = glm::rotate(wallTileR18, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR19 = glm::translate(wallTileR19, glm::vec3(1.5f, 0.5f, -1.0f));
	wallTileR19 = glm::rotate(wallTileR19, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR20 = glm::translate(wallTileR20, glm::vec3(1.5f, 0.5f, -2.0f));
	wallTileR20 = glm::rotate(wallTileR20, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR21 = glm::translate(wallTileR21, glm::vec3(1.5f, 0.5f, -3.0f));
	wallTileR21 = glm::rotate(wallTileR21, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR22 = glm::translate(wallTileR22, glm::vec3(1.5f, 0.5f, -4.0f));
	wallTileR22 = glm::rotate(wallTileR22, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR23 = glm::translate(wallTileR23, glm::vec3(2.0f, 0.5f, -4.5f));
	wallTileR23 = glm::rotate(wallTileR23, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR23 = glm::rotate(wallTileR23, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR24 = glm::translate(wallTileR24, glm::vec3(3.0f, 0.5f, -4.5f));
	wallTileR24 = glm::rotate(wallTileR24, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR24 = glm::rotate(wallTileR24, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR25 = glm::translate(wallTileR25, glm::vec3(3.5f, 0.5f, -4.0f));
	wallTileR25 = glm::rotate(wallTileR25, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR26 = glm::translate(wallTileR26, glm::vec3(3.5f, 0.5f, -3.0f));
	wallTileR26 = glm::rotate(wallTileR26, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR27 = glm::translate(wallTileR27, glm::vec3(3.5f, 0.5f, -2.0f));
	wallTileR27 = glm::rotate(wallTileR27, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR28 = glm::translate(wallTileR28, glm::vec3(4.0f, 0.5f, -1.5f));
	wallTileR28 = glm::rotate(wallTileR28, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR28 = glm::rotate(wallTileR28, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR29 = glm::translate(wallTileR29, glm::vec3(4.5f, 0.5f, -1.0f));
	wallTileR29 = glm::rotate(wallTileR29, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR30 = glm::translate(wallTileR30, glm::vec3(4.5f, 0.5f, -0.0f));
	wallTileR30 = glm::rotate(wallTileR30, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR31 = glm::translate(wallTileR31, glm::vec3(4.5f, 0.5f, 1.0f));
	wallTileR31 = glm::rotate(wallTileR31, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR32 = glm::translate(wallTileR32, glm::vec3(4.5f, 0.5f, 2.0f));
	wallTileR32 = glm::rotate(wallTileR32, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR33 = glm::translate(wallTileR33, glm::vec3(4.5f, 0.5f, 3.0f));
	wallTileR33 = glm::rotate(wallTileR33, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	wallTileR34 = glm::translate(wallTileR34, glm::vec3(4.0f, 0.5f, 3.5f));
	wallTileR34 = glm::rotate(wallTileR34, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR34 = glm::rotate(wallTileR34, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR35 = glm::translate(wallTileR35, glm::vec3(3.0f, 0.5f, 3.5f));
	wallTileR35 = glm::rotate(wallTileR35, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR35 = glm::rotate(wallTileR35, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR36 = glm::translate(wallTileR36, glm::vec3(2.0f, 0.5f, 3.5f));
	wallTileR36 = glm::rotate(wallTileR36, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR36 = glm::rotate(wallTileR36, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	wallTileR37 = glm::translate(wallTileR37, glm::vec3(1.0f, 0.5f, 3.5f));
	wallTileR37 = glm::rotate(wallTileR37, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wallTileR37 = glm::rotate(wallTileR37, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.5f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.5f, -3.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat angleX = M_PI;
	GLfloat angleY = 0.0f;
	GLfloat camSpeed = 0.5f;
	GLfloat mouseSpeed = 0.5f;

	GLfloat prevTime = glfwGetTime();

	GLdouble xpos, ypos;

	int forwardInput = glfwGetKey(window, GLFW_KEY_W);
	int backwardInput = glfwGetKey(window, GLFW_KEY_S);
	int leftInput = glfwGetKey(window, GLFW_KEY_A);
	int rightInput = glfwGetKey(window, GLFW_KEY_D);

	SoundEngine->play2D("Thesis Game.mp3", true);
	SoundEngine->setSoundVolume(0.1f);
	sfx = sfxEngine->play2D("FootStep.mp3", false, false, true);
	sfxEngine->setSoundVolume(0.5f);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat time = glfwGetTime();
		GLfloat deltaTime = time - prevTime;
		prevTime = time;

		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, (double)windowWidth / 2, (double)windowHeight / 2);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		angleX += mouseSpeed * deltaTime * float(windowWidth / 2 - xpos);
		angleY += mouseSpeed * deltaTime * float(windowHeight / 2 - ypos);

		glm::vec3 cameraTarget(cos(angleY) * sin(angleX), sin(angleY), cos(angleY) * cos(angleX));
		glm::vec3 right(sin(angleX - M_PI / 2.0f), 0.0f, cos(angleX - M_PI / 2.0f));
		glm::vec3 cameraUp = glm::cross(right, cameraTarget);


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraPosition += camSpeed * deltaTime * glm::normalize(glm::vec3(cameraTarget.x, 0.0f, cameraTarget.z));
			if (sfx->isFinished())
			{
				sfx = sfxEngine->play2D("FootStep.mp3", false, false, true);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraPosition -= camSpeed * deltaTime * glm::normalize(glm::vec3(cameraTarget.x, 0.0f, cameraTarget.z));
			if (sfx->isFinished())
			{
				sfx = sfxEngine->play2D("FootStep.mp3", false, false, true);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraPosition -= camSpeed * right * deltaTime;
			if (sfx->isFinished())
			{
				sfx = sfxEngine->play2D("FootStep.mp3", false, false, true);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraPosition += camSpeed * right * deltaTime;
			if (sfx->isFinished())
			{
				sfx = sfxEngine->play2D("FootStep.mp3", false, false, true);
			}
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
		glBindVertexArray(floorVao);

		GLint floorUniformLocation = glGetUniformLocation(depthshaders, "modelMatrix");
		glUniformMatrix4fv(floorUniformLocation, 1, GL_FALSE, glm::value_ptr(floorTile01));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindVertexArray(0);


		glBindVertexArray(planeVao);

		GLint planeUniformLocation = glGetUniformLocation(depthshaders, "modelMatrix");
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL01));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL02));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL03));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL04));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL05));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL06));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL07));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL08));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL09));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL10));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL11));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL12));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL13));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL14));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL15));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL16));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL17));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL18));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL19));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL20));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL21));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL22));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL23));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL24));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL25));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL26));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL27));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL28));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL29));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL30));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL31));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL32));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL33));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL34));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL35));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL36));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL37));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL38));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL39));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL40));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL41));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL42));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL43));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL44));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL45));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL46));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL47));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		

		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR01));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR02));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR03));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR04));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR05));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR06));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR07));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR08));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR09));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR10));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR11));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR12));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR13));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR14));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR15));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR16));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR17));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR18));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR19));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR20));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR21));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR22));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR23));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR24));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR25));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR26));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR27));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR28));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR29));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR30));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR31));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR32));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR33));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR34));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR35));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR36));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR37));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	



		//
		// SECOND PASS 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboTex);
		

		// Draw Skybox
		glDepthMask(GL_FALSE);
		glUseProgram(skyboxshaders);

		GLint skyboxProjectionUniformLocation = glGetUniformLocation(skyboxshaders, "projection");
		GLint skyboxViewUniformLocation = glGetUniformLocation(skyboxshaders, "view");
	
		glUniformMatrix4fv(skyboxProjectionUniformLocation, 1, GL_FALSE, glm::value_ptr(perspective));
		glUniformMatrix4fv(skyboxViewUniformLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera))));


		glBindVertexArray(skyboxVao);
		stbi_set_flip_vertically_on_load(true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		glBindVertexArray(0);

		// Use the shader program that we created
		glUseProgram(program);
		

		GLint shadowMapTexUniformLocation = glGetUniformLocation(program, "shadowMap");
		glUniform1i(shadowMapTexUniformLocation, 0);
		
		

		// FLOOR
		//
		glBindVertexArray(floorVao);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, floorTex);
		floorUniformLocation = glGetUniformLocation(program, "modelMatrix");
		glUniformMatrix4fv(floorUniformLocation, 1, GL_FALSE, glm::value_ptr(floorTile01));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Bind our texture to texture unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, wallTex);

		glBindVertexArray(0);


		// PLANE
		glBindVertexArray(planeVao);
		planeUniformLocation = glGetUniformLocation(program, "modelMatrix");
		
		// Make our sampler in the fragment shader use texture unit 0
		GLint texUniformLocation = glGetUniformLocation(program, "tex");
		glUniform1i(texUniformLocation, 1);

		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL01));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL02));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL03));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL04));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL05));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL06));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL07));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL08));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL09));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL10));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL11));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL12));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL13));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL14));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL15));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL16));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL17));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL18));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL19));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL20));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL21));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL22));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL23));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL24));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL25));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL26));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL27));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL28));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL29));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL30));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL31));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL32));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL33));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL34));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL35));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL36));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL37));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL38));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL39));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL40));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL41));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL42));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL43));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL44));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL45));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL46));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileL47));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		

		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR01));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR02));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR03));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR04));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR05));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR06));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR07));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR08));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR09));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR10));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR11));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR12));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR13));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR14));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR15));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR16));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR17));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR18));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR19));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR20));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR21));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR22));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR23));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR24));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR25));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR26));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR27));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR28));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR29));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR30));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR31));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR32));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR33));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR34));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR35));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR36));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUniformMatrix4fv(planeUniformLocation, 1, GL_FALSE, glm::value_ptr(wallTileR37));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(0);


		GLint cameraPositionUniformLocation = glGetUniformLocation(program, "cameraPosition");
		GLint objectSpecUniformLocation = glGetUniformLocation(program, "objectSpec");
		GLint objectShineUniformLocation = glGetUniformLocation(program, "objectShine");
		GLint dirLightDirUniformLocation = glGetUniformLocation(program, "directionalLightDirection");
		
		GLint dirLightAmbientUniformLocation = glGetUniformLocation(program, "lightAmbient");
		GLint dirLightDiffuseUniformLocation = glGetUniformLocation(program, "lightDiffuse");
		GLint dirLightSpecularUniformLocation = glGetUniformLocation(program, "lightSpecular");

		GLint sLightAmbientUniformLocation = glGetUniformLocation(program, "sLightAmbient");
		GLint sLightDiffuseUniformLocation = glGetUniformLocation(program, "sLightDiffuse");
		GLint sLightSpecularUniformLocation = glGetUniformLocation(program, "sLightSpecular");
		GLint spotLightPositionUniformLocation = glGetUniformLocation(program, "spotLightPosition");
		GLint spotLightDirectionUniformLocation = glGetUniformLocation(program, "spotLightDirection");

		GLint sLightConstantUniformLocation = glGetUniformLocation(program, "sLightConstant");
		GLint sLightLinearUniformLocation = glGetUniformLocation(program, "sLightLinear");
		GLint sLightQuadraticUniformLocation = glGetUniformLocation(program, "sLightQuadratic");

		GLint dirLightProjectionUniformLocation2 = glGetUniformLocation(program, "lightProjection");
		GLint dirLightViewUniformLocation2 = glGetUniformLocation(program, "lightView");
		
		glUniform3fv(cameraPositionUniformLocation, 1, glm::value_ptr(cameraPosition));
		glUniform3f(objectSpecUniformLocation, 1.0f, 1.0f, 1.0f);
		glUniform1f(objectShineUniformLocation, 100.f);
		glUniform3f(dirLightDirUniformLocation, -0.25f, -1.0f, -0.25f);

		glUniform3f(dirLightAmbientUniformLocation, 1.f, 0.f, 0.f);
		glUniform3f(dirLightDiffuseUniformLocation, 0.f, 1.f, 0.f);
		glUniform3f(dirLightSpecularUniformLocation, 0.f, 0.f, 1.f);

		glUniform3f(sLightAmbientUniformLocation, 1.0f, 1.0f, 1.0f);
		glUniform3f(sLightDiffuseUniformLocation, 1.0f, 1.0f, 1.0f);
		glUniform3f(sLightSpecularUniformLocation, 1.0f, 1.0f, 1.0f);

		glUniform1f(sLightAmbientUniformLocation, 1.0f);
		glUniform1f(sLightLinearUniformLocation, 0.35f);
		glUniform1f(sLightQuadraticUniformLocation, 0.44f);

		glUniform3f(spotLightPositionUniformLocation,  cameraPosition.x, cameraPosition.y, cameraPosition.z);
		glUniform3f(spotLightDirectionUniformLocation, cameraTarget.x, cameraTarget.y, cameraTarget.z);

		glUniformMatrix4fv(dirLightProjectionUniformLocation2, 1, GL_FALSE, glm::value_ptr(lightProjection));
		glUniformMatrix4fv(dirLightViewUniformLocation2, 1, GL_FALSE, glm::value_ptr(lightView));
		
		// Camera computations
		camera = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, cameraUp);
		perspective = glm::perspective(glm::radians(90.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);

		GLint camUniformLocation = glGetUniformLocation(program, "camera");
		GLint perspectiveUniformLocation = glGetUniformLocation(program, "perspective");
		glUniformMatrix4fv(camUniformLocation, 1, GL_FALSE, glm::value_ptr(camera));
		glUniformMatrix4fv(perspectiveUniformLocation, 1, GL_FALSE, glm::value_ptr(perspective));

		glBindVertexArray(0);

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
	glDeleteBuffers(1, &floorVbo);
	glDeleteBuffers(1, &skyboxVbo);

	// Delete the vertex array object
	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &planeVao);
	glDeleteVertexArrays(1, &floorVao);
	glDeleteVertexArrays(1, &skyboxVao);


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
