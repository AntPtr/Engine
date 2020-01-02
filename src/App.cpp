/*Rendering with OpenGL 3.3
This software is just educational and free to use, the main loop is in App.cpp, the redering uses three shaders, 
one for lamps(lampfs, lampvf) and one for model(Shader, FragSha), for the import file : Assimp and stbimage implemented in the model class.
Added cubemaps for skybox and reflection maps texture;
Compiled in visual studio for windows 64
Antonio Pietroluongo  last update 16/12/2019
*/
//Dependecies
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

//Window paramenters
int winWidth = 800;
int winHeigt = 600;

//Lamps and ligthing
glm::vec3 ligthdirection(-0.2f, -1.0f, -0.3f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f),
	glm::vec3(-2.0f,  0.05, -2.0f)
};

glm::vec3 DirligthAmbient(0.05f, 0.05f, 0.05f);
glm::vec3 DirligthDiffuse(0.4f, 0.4f, 0.4f);
glm::vec3 DirligthSpecular(0.5f, 0.5f, 0.5f);

glm::vec3 pointAmbient(0.05f, 0.05f, 0.05f);
glm::vec3 pointDiffuse(0.8f, 0.8f, 0.8f);
glm::vec3 pointSpecular(1.0f, 1.0f, 1.0f);

glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
};

//SkyBox
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

//Lamp
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

//Resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	winWidth = width;
	winHeigt = height;
	glViewport(0, 0, width, height);
}

//Input for mouse and keyboard

//Mouse starting coords
float lastX = 400;
float lastY = 300;
bool firstmouse = true;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Setting camera object

//Mouse input function to pass to 
void mouse_call(GLFWwindow* win, double xpos, double ypos) {

	if (firstmouse) {
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	cam.processMouse(xoffset, yoffset);

}
float deltaTime{ 0.0f };
float lastFrame{ 0.0f };

//Keyboard handled by camera class
void read_in(GLFWwindow* win) {

	float cameraSpeed = 0.5f * deltaTime;
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	else if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
		cam.processKey(FORWARD, deltaTime);
	}
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		cam.processKey(BACKWARD, deltaTime);
	}
	else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
		cam.processKey(RIGHT, deltaTime);
	}
	else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
		cam.processKey(LEFT, deltaTime);
	}

}

//Entry point
int main()
{
	//GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Create a window
	GLFWwindow* window = glfwCreateWindow(800,600,"Test",NULL,NULL);
	if(window == NULL) {
		std::cout << "Failed to create a window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_call);	

	Model tesla("tech/nanosuit.obj");

	//Setting up OpenGL buffers
	unsigned int VBO2, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//SkyBox Buffer
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	


	std::vector<std::string> faces{
		"city_rt.tga",
		"city_lf.tga",
		"city_up.tga",
		"city_dn.tga",
		"city_bk.tga",
		"city_ft.tga"
	};
	unsigned int cubemapTexture = loadskybox(faces);
	// -----------------------------------------------------------------------------

	// shader configuration
	cShaderProg ligth_shader("Lampvs.txt", "Lampfs.txt");

	cShaderProg shader_program("Shader.txt", "FragSha.txt");

	cShaderProg sky_shader("SkyVer.txt", "SkyFra.txt");

	shader_program.use();

	shader_program.set_float("material.shiness", 32.0f);
	

	sky_shader.use();
	sky_shader.set_int("skybox", 0);

	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //uncomment for wireframe mode

	//Main Loop
	while (!glfwWindowShouldClose(window)) {

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		read_in(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_program.use();
		int find;
		find = glGetUniformLocation(shader_program.get_id(), "viewPos");
		glUniform3fv(find, 1, &cam.cameraPos[0]);

		//Ligth Direction
		find = glGetUniformLocation(shader_program.get_id(), "directionl.direction");
		glUniform3fv(find, 1, &ligthdirection[0]);


		//Directional propietis
		find = glGetUniformLocation(shader_program.get_id(), "directionl.ambient");
		glUniform3fv(find, 1, &DirligthAmbient[0]);


		find = glGetUniformLocation(shader_program.get_id(), "directionl.diffuse");
		glUniform3fv(find, 1, &DirligthDiffuse[0]);

		find = glGetUniformLocation(shader_program.get_id(), "directionl.specular");
		glUniform3fv(find, 1, &DirligthSpecular[0]);

		//Points Ligths
		find = glGetUniformLocation(shader_program.get_id(), "point[0].position");
		glUniform3fv(find, 1, &pointLightPositions[0][0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[0].ambient");
		glUniform3fv(find, 1, &pointAmbient[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[0].diffuse");
		glUniform3fv(find, 1, &pointDiffuse[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[0].specular");
		glUniform3fv(find, 1, &pointSpecular[0]);

		shader_program.set_float("point[0].constant", 1.0f);
		shader_program.set_float("point[0].linear", 0.09);
		shader_program.set_float("point[0].quadratic", 0.032);

		find = glGetUniformLocation(shader_program.get_id(), "point[1].position");
		glUniform3fv(find, 1, &pointLightPositions[1][0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[1].ambient");
		glUniform3fv(find, 1, &pointAmbient[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[1].diffuse");
		glUniform3fv(find, 1, &pointDiffuse[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[1].specular");
		glUniform3fv(find, 1, &pointSpecular[0]);

		shader_program.set_float("point[1].constant", 1.0f);
		shader_program.set_float("point[1].linear", 0.09);
		shader_program.set_float("point[1].quadratic", 0.032);

		find = glGetUniformLocation(shader_program.get_id(), "point[2].position");
		glUniform3fv(find, 1, &pointLightPositions[2][0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[2].ambient");
		glUniform3fv(find, 1, &pointAmbient[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[2].diffuse");
		glUniform3fv(find, 1, &pointDiffuse[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[2].specular");
		glUniform3fv(find, 1, &pointSpecular[0]);

		shader_program.set_float("point[2].constant", 1.0f);
		shader_program.set_float("point[2].linear", 0.09);
		shader_program.set_float("point[2].quadratic", 0.032);

		find = glGetUniformLocation(shader_program.get_id(), "point[3].position");
		glUniform3fv(find, 1, &pointLightPositions[3][0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[3].ambient");
		glUniform3fv(find, 1, &pointAmbient[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[3].diffuse");
		glUniform3fv(find, 1, &pointDiffuse[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[3].specular");
		glUniform3fv(find, 1, &pointSpecular[0]);

		shader_program.set_float("point[3].constant", 1.0f);
		shader_program.set_float("point[3].linear", 0.09);
		shader_program.set_float("point[3].quadratic", 0.032);

		find = glGetUniformLocation(shader_program.get_id(), "point[4].position");
		glUniform3fv(find, 1, &pointLightPositions[4][0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[4].ambient");
		glUniform3fv(find, 1, &pointAmbient[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[4].diffuse");
		glUniform3fv(find, 1, &pointDiffuse[0]);

		find = glGetUniformLocation(shader_program.get_id(), "point[4].specular");
		glUniform3fv(find, 1, &pointSpecular[0]);

		shader_program.set_float("point[4].constant", 1.0f);
		shader_program.set_float("point[4].linear", 0.09);
		shader_program.set_float("point[4].quadratic", 0.032);
	
		
		//model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	
		find = glGetUniformLocation(shader_program.get_id(), "model");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(model));

		//view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = cam.getViewMat();
		find = glGetUniformLocation(shader_program.get_id(), "view");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(view));

		//projection matrix
		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f),((float)(winWidth))/ ((float)(winHeigt)), 0.1f, 100.0f);
		find = glGetUniformLocation(shader_program.get_id(), "proj");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(proj));

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		shader_program.set_int("skybox", 10);
		tesla.draw(shader_program);


		ligth_shader.use();

		//Matrixs for lamps 
		find = glGetUniformLocation(ligth_shader.get_id(), "proj");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(proj));

		find = glGetUniformLocation(ligth_shader.get_id(), "view");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(view));


		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 5; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); 
			find = glGetUniformLocation(ligth_shader.get_id(), "model");
			glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		sky_shader.use();
		view = glm::mat4(glm::mat3(cam.getViewMat())); // remove translation from the view matrix
		find = glGetUniformLocation(sky_shader.get_id(), "view");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(view));

		find = glGetUniformLocation(sky_shader.get_id(), "projection");
		glUniformMatrix4fv(find, 1, GL_FALSE, glm::value_ptr(proj));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function 



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}