//
// Display a illuminated cube
//

#include "initShader.h"
#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "texture.hpp"

enum eShadeMode { NO_LIGHT, GOURAUD, PHONG, NUM_LIGHT_MODE };

glm::mat4 projectMat;
glm::mat4 viewMat;
glm::mat4 modelMat = glm::mat4(1.0f);

float rotAngle = 0.0f;
float rotAngleRForearm = 0.0f;
float rotAngleLForearm = 0.0f;
float rotAngleLeg = 0.0f;
float rotAngleLowerLeg = 0.0f;

bool up = true; // flag for angle of leg movement 
bool lowerLeg_up = true;

int shadeMode = PHONG;
int isTexture = true;

GLuint projectMatrixID;
GLuint viewMatrixID;
GLuint modelMatrixID;
GLuint shadeModeID;
GLuint textureModeID;
GLuint pvmMatrixID;

Cube cube;
//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
	// Create a vertex array object
	GLuint vao[1];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// Create and initialize a buffer object
	GLuint buffer[1];
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

	int vertSize = sizeof(cube.verts[0])*cube.verts.size();
	int normalSize = sizeof(cube.normals[0])*cube.normals.size();
	int texSize = sizeof(cube.texCoords[0])*cube.texCoords.size();

	glBufferData(GL_ARRAY_BUFFER, vertSize + normalSize + texSize,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, cube.verts.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertSize, normalSize, cube.normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertSize+normalSize, texSize, cube.texCoords.data());

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertSize+normalSize));

	projectMatrixID = glGetUniformLocation(program, "mProject");
	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);

	viewMatrixID = glGetUniformLocation(program, "mView");
	viewMat = glm::lookAt(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); //right
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);

	modelMatrixID = glGetUniformLocation(program, "mModel");
	modelMat = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);

	shadeModeID = glGetUniformLocation(program, "shadeMode");
	glUniform1i(shadeModeID, shadeMode);

	textureModeID = glGetUniformLocation(program, "isTexture");
	glUniform1i(textureModeID, isTexture);

	pvmMatrixID = glGetUniformLocation(program, "mPVM");

	// Load the texture using any two methods
	GLuint Texture = loadBMP_custom("flag.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(program, "cubeTexture");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------
void drawcube(glm::mat4 mat)
{
	glm::mat4 modelMat, pvmMat;

	glm::vec3 arm[2];
	arm[0] = glm::vec3(0.2, 0.35, 0);
	arm[1] = glm::vec3(-0.2, -0.35, 0);

	glm::vec3 forearm[2];
	forearm[0] = glm::vec3(0.4, 0.35, 0);
	forearm[1] = glm::vec3(-0.4, -0.35, 0); //right


	glm::vec3 uleg[2];
	uleg[0] = glm::vec3(-0.8, -0.13, 0);
	uleg[1] = glm::vec3(-0.8, 0.13, 0);

	glm::vec3 lleg[2];
	lleg[0] = glm::vec3(-1.0, -0.13, 0);
	lleg[1] = glm::vec3(-1.0, 0.13, 0);

	// body
	modelMat = glm::scale(mat, glm::vec3(0.85, 0.5, 0.4));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// head
	modelMat = glm::translate(mat, glm::vec3(0.75, 0, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.3, 0.3));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// arms
	for (int i = 0; i < 2; i++)
	{
		modelMat = glm::translate(mat, glm::vec3(0.2, 0.0, 0.0));
		modelMat = glm::rotate(modelMat, rotAngle * 2.0f, glm::vec3(0, 1, 0));

		modelMat = glm::translate(modelMat, arm[i]);
		modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.1, 0.15));

		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// forearms
	for (int i = 0; i < 2; i++)
	{
		modelMat = glm::translate(mat, glm::vec3(0.2, 0.0, 0.0));
		modelMat = glm::rotate(modelMat, rotAngle * 2.0f, glm::vec3(0, 1, 0));
		if (i == 1)
		{
			modelMat = glm::translate(modelMat, forearm[i]);
			modelMat = glm::rotate(modelMat, rotAngleRForearm * 4.0f, glm::vec3(0, 1, 0));
		}
		else
		{
			modelMat = glm::translate(modelMat, forearm[i]);
			modelMat = glm::rotate(modelMat, rotAngleLForearm * 4.0f, glm::vec3(0, 1, 0));
			modelMat = glm::translate(modelMat, glm::vec3(0.4, 0.0, 0.0));
		}

		modelMat = glm::translate(modelMat, glm::vec3(-0.2, 0.0, 0.0));
		modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.1, 0.15));
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// upper legs
	for (int i = 0; i < 2; i++)
	{
		modelMat = mat;
		if (i == 1) {
			modelMat = glm::rotate(modelMat, rotAngleLeg * -0.5f, glm::vec3(0, 1, 0));
		}
		else
		{
			modelMat = glm::rotate(modelMat, rotAngleLeg * 0.5f, glm::vec3(0, 1, 0));
		}
		modelMat = glm::translate(modelMat, uleg[i]);
		modelMat = glm::scale(modelMat, glm::vec3(0.6, 0.15, 0.2));
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	// lower legs
	for (int i = 0; i < 2; i++)
	{
		modelMat = mat;
		// left or right
		if (i == 1)
		{
			modelMat = glm::rotate(modelMat, rotAngleLeg * -0.5f, glm::vec3(0, 1, 0));
			modelMat = glm::translate(modelMat, lleg[i]);
			modelMat = glm::rotate(modelMat, rotAngleLowerLeg * 0.5f, glm::vec3(0, 1, 0));
		}
		else
		{
			modelMat = glm::rotate(modelMat, rotAngleLeg * 0.5f, glm::vec3(0, 1, 0));
			modelMat = glm::translate(modelMat, lleg[i]);
			modelMat = glm::rotate(modelMat, rotAngleLowerLeg * 0.5f, glm::vec3(0, 1, 0));
		}
		modelMat = glm::translate(modelMat, glm::vec3(-0.5, 0, 0));
		modelMat = glm::scale(modelMat, glm::vec3(0.6, 0.15, 0.2));
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldMat = glm::mat4(1.0f);

	drawcube(worldMat);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);

	if (abs(currTime - prevTime) >= 20)
	{
		float t = abs(currTime - prevTime);

		rotAngle += glm::radians(t * 360.0f / 10000.0f);
		if (rotAngle >= glm::radians(2 * 900000.0f / 10000.0f)) {
			rotAngle -= glm::radians(2 * 900000.0f / 10000.0f);
			//std::cout << "ÇÑ¹ÙÄû" << std::endl;
		}

		if (rotAngle <= glm::radians(900000.0f / 10000.0f)) { // half circle
			rotAngleLForearm = 0.0f;
			rotAngleRForearm -= glm::radians(t * 360.0f / 10000.0f);
		}
		else
		{
			rotAngleRForearm = 0.0f;
			rotAngleLForearm -= glm::radians(t * 360.0f / 10000.0f);
		}

		if (up) {
			rotAngleLeg += glm::radians(t * 360.0f / 10000.0f);
			if (rotAngleLeg >= glm::radians(8.0f)) {
				up = false;
			}
		}
		else
		{
			rotAngleLeg -= glm::radians(t * 360.0f / 10000.0f);
			if (rotAngleLeg <= glm::radians(-8.0f)) {
				up = true;
			}
		}

		if (lowerLeg_up) {
			rotAngleLowerLeg += glm::radians(t * 360.0f / 10000.0f * 1.2f);
			if (rotAngleLowerLeg >= glm::radians(30.0f)) {
				lowerLeg_up = false;
			}
		}
		else
		{
			rotAngleLowerLeg -= glm::radians(t * 360.0f / 10000.0f * 1.2f);
			if (rotAngleLowerLeg <= glm::radians(-3.0f)) {
				lowerLeg_up = true;
			}
		}
		prevTime = currTime;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l': case 'L':
		shadeMode = (++shadeMode % NUM_LIGHT_MODE);
		glUniform1i(shadeModeID, shadeMode);
		glutPostRedisplay();
		break;
	case 'c': case 'C':
		viewMat = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
		break;
	case 'v': case 'V':
		viewMat = glm::lookAt(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Swimming man texture");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
