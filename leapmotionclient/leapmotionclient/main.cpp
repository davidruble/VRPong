#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Leap/Leap.h"
#include "Camera.h"
#include "LeapListener.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <irrKlang\irrKlang.h>
using namespace irrklang;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void initGame();
void initSound();
void update();
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera1(glm::vec3(-0.015f, 0.1f, -3.0f));
Camera camera2(glm::vec3(0.015f, 0.1f, -3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
LeapListener listener;
Leap::Controller controller;
const int playerid = 2;
ISoundEngine *SoundEngine;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
ISound* music;
ISound* sheild;


#include "model.h"
#include "shader.h"
#include "Hand.h"
#include "Level.h"
#include "Ball.h"
#include "Player.h"

#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

#define LVERTEX_SHADER_PATH "laser.vert"
#define LFRAGMENT_SHADER_PATH "laser.frag"

glm::vec3 lightPos(0.0f, 0.2f, 0.0f);
glm::vec3 lightAmbient(0.5f, 0.5f, 0.5f);
glm::vec3 lightDiffuse(0.9f, 0.9f, 0.7f);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

// An example application that renders a simple cube
	vector<Player> players;
	Level * level;
	Ball * ball;
	Shader * shader = NULL;

	GLint shaderProgram;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
	initGame();
	initSound();
    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
		SoundEngine->setListenerPosition(vec3df(camera1.Position.x, camera1.Position.y, camera1.Position.z),
			vec3df(-camera1.Front.x, -camera1.Front.y, -camera1.Front.z));
		update();
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        // don't forget to enable shader before setting uniforms
		
		//pass 1
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColorMask(true, false, false, false);

		shader->Use();

		glUniform3fv(glGetUniformLocation(shader->Program, "light.position"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "light.ambient"), 1, &lightAmbient[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "light.diffuse"), 1, &lightDiffuse[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "light.specular"), 1, &lightSpecular[0]);
        // view/projection transformations
        //glm::mat4 projection = glm::perspective(glm::radians(camera1.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 projection = camera1.stereoProject(SCR_WIDTH, SCR_HEIGHT, true);
        glm::mat4 view = camera1.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "CameraMatrix"), 1, GL_FALSE, &(view)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projection[0][0]);

        // render the loaded model
		glUniform3fv(glGetUniformLocation(shader->Program, "viewPos"), 1, &(camera1.Position)[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "viewDir"), 1, &(camera1.Front)[0]);

		ball->Draw(*shader);
		level->Draw(*shader);
		for (int i = 0; i < players.size(); ++i) {
			players[i].Draw(*shader, 2);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		glColorMask(false, true, true, false);


		shader->Use();

		glUniform3fv(glGetUniformLocation(shader->Program, "light.position"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "light.ambient"), 1, &lightAmbient[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "light.diffuse"), 1, &lightDiffuse[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "light.specular"), 1, &lightSpecular[0]);
		// view/projection transformations
		//projection = glm::perspective(glm::radians(camera2.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		projection = camera2.stereoProject(SCR_WIDTH, SCR_HEIGHT, false);
		view = camera2.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "CameraMatrix"), 1, GL_FALSE, &(view)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projection[0][0]);

		// render the loaded model
		glUniform3fv(glGetUniformLocation(shader->Program, "viewPos"), 1, &(camera2.Position)[0]);
		glUniform3fv(glGetUniformLocation(shader->Program, "viewDir"), 1, &(camera2.Front)[0]);

		ball->Draw(*shader);
		level->Draw(*shader);
		for (int i = 0; i < players.size(); ++i) {
			players[i].Draw(*shader, 2);
		}

		glColorMask(true, true, true, true);
		cout << "bro" << endl;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void initSound() {
	irrklang::ISoundDeviceList* deviceList = createSoundDeviceList();

	printf("Devices available:\n\n");

	for (int i = 0; i<deviceList->getDeviceCount(); ++i)
		printf("%d: %s\n", i, deviceList->getDeviceDescription(i));

	printf("\nselect a device using the number (or press any key to use default):\n\n");
	int deviceNumber = _getch() - '0';

	// create device with the selected driver

	const char* deviceID = deviceList->getDeviceID(deviceNumber);

	SoundEngine = createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT,
		irrklang::ESEO_DEFAULT_OPTIONS,
		deviceID);

	deviceList->drop(); // delete device list

	music = SoundEngine->play2D("Assets/sound/level.mp3", true, false, true);
	music->setVolume(0.5);
	SoundEngine->setListenerPosition(vec3df(0, 0, -3.0),
		vec3df(0, 0, 1));
}

bool intersect(int playernum) {
	glm::vec3 center = ball->calcCenterPoint();
	glm::vec3 min = players[playernum].hand->min;
	glm::vec3 max = players[playernum].hand->max;
	//cout << "center: " << center.x << center.y << center.z << endl;
	return (center.x >= min.x && center.x <= max.x) &&
		(center.y >= min.y && center.y <= max.y) &&
		(center.z >= min.z && center.z <= max.z);
}

void update() {
	bool triggerr = false;

	ball->update();
	for (int i = 0; i < players.size(); ++i) {
		if (players[i].hand->isLeap) {
			cout << "leap" << endl;
			players[i].hand->pollLeapInput(controller, players[i]);
			players[i].update(NULL, NULL);
		}else {
			cout << "not leap" << endl;
			//set hand and head pose here fromw/e we got from network
			//players[i].head->HeadPose = shit;
			//players[i].hand->HandPose = shit;
			players[i].update(NULL, NULL);
		}
		if (intersect(i) && ball->lastPlayer != players[i].playerNum)
		{
			vec3 s = ball->calcCenterPoint();
			sheild = SoundEngine->play3D("Assets/sound/clang.wav",
				vec3df(s.x, s.y, s.z), false, false, true);
			sheild->setMinDistance(1.0f);
			ball->velocity = -ball->velocity;
			ball->lastPlayer = players[i].playerNum;
			glm::quat direc = ovr::toGlm(players[i].hand->HandPose.Orientation);
			vec3 reflect = glm::mat4_cast(direc)* vec4(0.0, 0.0, 1.0f, 1.0f);
			cout << reflect.x << reflect.y << reflect.z << endl;
			//cin >> a;
			ball->velocity = reflect*-0.15f;
		}
	}
}

void initGame() {
	shader = new Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	//SETUP INITIAL SCENE HERE
	level = new Level();
	ball = new Ball();
	players.push_back(Player(players.size() + 1, new Hand(NULL, 0, true)));
	players.push_back(Player(players.size() + 1, new Hand(true)));
	//controller.addListener(listener);
	//controller.setPolicy(Leap::Controller::POLICY_ALLOW_PAUSE_RESUME);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera1.ProcessKeyboard(FORWARD, deltaTime);
		camera2.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera1.ProcessKeyboard(BACKWARD, deltaTime);
		camera2.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera1.ProcessKeyboard(LEFT, deltaTime);
		camera2.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera1.ProcessKeyboard(RIGHT, deltaTime);
		camera2.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera1.ProcessMouseMovement(xoffset, yoffset);
	camera2.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera1.ProcessMouseScroll(yoffset);
	camera2.ProcessMouseScroll(yoffset);
}