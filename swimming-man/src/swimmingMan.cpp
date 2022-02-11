//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

glm::mat4 projectMat;
glm::mat4 viewMat;

GLuint pvmMatrixID;

float rotAngle = 0.0f;
float rotAngleRForearm = 0.0f;
float rotAngleLForearm = 0.0f;
float rotAngleLeg = 0.0f;
float rotAngleLowerLeg = 0.0f;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

bool up = true; // flag for angle of leg movement 
bool lowerLeg_up = true;
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA colors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(0.0, 1.0, 1.0, 1.0),   // cyan
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 1.0, 1.0, 1.0)  // white
};

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d];  Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	pvmMatrixID = glGetUniformLocation(program, "mPVM");

	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	//viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //top
	viewMat = glm::lookAt(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); //right

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
//----------------------------------------------------------------------------

void drawSwimmingMan(glm::mat4 mat)
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
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// head
	modelMat = glm::translate(mat, glm::vec3(0.75, 0, 0)); 
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.3, 0.3));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// arms
	for (int i = 0; i < 2; i++)
	{
		modelMat = glm::translate(mat, glm::vec3(0.2, 0.0, 0.0));	
		modelMat = glm::rotate(modelMat, rotAngle * 2.0f, glm::vec3(0, 1, 0));
		modelMat = glm::translate(modelMat, arm[i]);  
		modelMat = glm::scale(modelMat, glm::vec3(0.4, 0.1, 0.15)); 
		
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldMat = glm::mat4(1.0f);

	drawSwimmingMan(worldMat);

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
		
		rotAngle += glm::radians(t*360.0f / 10000.0f);
		if (rotAngle >= glm::radians(2* 900000.0f / 10000.0f)) {
			rotAngle -= glm::radians(2* 900000.0f / 10000.0f);
			//std::cout << "ÇÑ¹ÙÄû" << std::endl;
		}

		if (rotAngle <= glm::radians(900000.0f / 10000.0f) ) { // half circle
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

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c': case 'C':
		viewMat = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
		break;
	case 'v': case 'V':
		viewMat = glm::lookAt(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
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

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Swimming Man");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
