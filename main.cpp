
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
GLuint GrassID, PyramidID, FanID, GemID, HouseBaseID, CeilingID, WallID;
float rotateFan = 0.0f, delta = 0.1f, scale = 1.0f;
GLint x_press_num = 0, y_press_num = 0, z_press_num;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) scale -= 0.1f;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) scale += 0.1f;
}

void keyboard(unsigned char key, int x, int y)
{
	//TODO:
	if (key == 'r') scale += 0.01f;
	if (key == 't') scale -= 0.01f;
	if (key == 'z') rotateFan -= 0.5f;
	if (key == 'x') rotateFan += 0.5f;
	if (key == 'a') x_press_num -= 1;
	if (key == 'd') x_press_num += 1;
	if (key == 's') y_press_num -= 1;
	if (key == 'w') y_press_num += 1;
	if (key == 'i') z_press_num -= 1;
	if (key == 'o') z_press_num += 1;
}

void sendDataToOpenGL()
{
	//TODO:
	//create solid objects here and bind to VAO & VBO
	const GLfloat Grass[] = {
		-20.0f, -10.0f, -28.0f,
		+0.0f, +0.6f, +0.0f,
		-20.0f, -10.0f, -8.0f,
		+0.0f, +0.6f, +0.0f,
		+20.0f, -10.0f, -28.0f,
		+0.0f, +0.6f, +0.0f,
		+20.0f, -10.0f, -8.0f,
		+0.0f, +0.6f, +0.0f,
	};
	GLushort indices1[] = { 0,1,2,1,2,3 };
	glGenVertexArrays(1, &GrassID);
	glBindVertexArray(GrassID);
	GLuint vbo1;
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Grass), Grass, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx1;
	glGenBuffers(1, &vbo_idx1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	const GLfloat Wall[] = {
		-22.0f, -11.0f, -28.0f,
		+0.7f, +0.7f, +0.7f,
		-22.0f, -11.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,
		-20.0f, -11.0f, -28.0f,
		+0.4f, +0.4f, +0.4f,
		-20.0f, -11.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,

		-22.0f, +4.0f, -28.0f,
		+0.7f, +0.7f, +0.7f,
		-22.0f, +4.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,
		-20.0f, +4.0f, -28.0f,
		+0.4f, +0.4f, +0.4f,
		-20.0f, +4.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,

		+22.0f, -11.0f, -28.0f,
		+0.7f, +0.7f, +0.7f,
		+22.0f, -11.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,
		+20.0f, -11.0f, -28.0f,
		+0.4f, +0.4f, +0.4f,
		+20.0f, -11.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,

		+22.0f, +4.0f, -28.0f,
		+0.7f, +0.7f, +0.7f,
		+22.0f, +4.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,
		+20.0f, +4.0f, -28.0f,
		+0.4f, +0.4f, +0.4f,
		+20.0f, +4.0f, -8.0f,
		+0.7f, +0.7f, +0.7f,
	};
	GLushort indices8[] = { 0,1,2,1,2,3,1,5,7,1,3,7,0,3,6,0,4,6,2,3,6,3,7,6,0,1,5,0,4,5,4,5,6,5,6,7,8,9,10,9,10,11,8,9,13,8,9,12,8,10,14,8,12,14,10,11,15,10,14,15,12,13,14,13,14,15,9,11,13,11,15,13,2,10,14,2,6,14};
	glGenVertexArrays(8, &WallID);
	glBindVertexArray(WallID);
	GLuint vbo8;
	glGenBuffers(1, &vbo8);
	glBindBuffer(GL_ARRAY_BUFFER, vbo8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wall), Wall, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx8;
	glGenBuffers(1, &vbo_idx8);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices8), indices8, GL_STATIC_DRAW);

	const GLfloat Pyramid[] = {
		-10.0f, +0.0f, -12.0f,
		+0.9f, +0.9f, +0.9f,

		-5.0f, -10.0f, -13.25f,
		+0.9f, +0.7f, +0.4f,
		-5.0f, -10.0f, -10.75f,
		+0.9f, +0.7f, +0.4f,

		-15.0f, -10.0f, -10.75f,
		+0.9f, +0.7f, +0.4f,
		-15.0f, -10.0f, -13.25f,
		+0.9f, +0.7f, +0.4f,
	};
	GLushort indices3[] = { 0,1,2,0,2,3,0,3,4,0,4,1,1,2,3,2,3,4 };
	glGenVertexArrays(1, &PyramidID);
	glBindVertexArray(PyramidID);
	GLuint vbo3;
	glGenBuffers(1, &vbo3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx3;
	glGenBuffers(1, &vbo_idx3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

	const GLfloat Fan[] = {
		+0.0f, +0.0f, -12.0f,
		+1.0f, +1.0f, +1.0f,

		-6.0f, +2.0f, -12.0f,
		+0.9f, +0.1f, +0.1f,
		-6.0f, -2.0f, -12.0f,
		+0.9f, +0.1f, +0.1f,
		-8.0f, +0.0f, -12.0f,
		+0.9f, +0.1f, +0.1f,

		+6.0f, +2.0f, -12.0f,
		+0.1f, +0.9f, +0.1f,
		+6.0f, -2.0f, -12.0f,
		+0.1f, +0.9f, +0.1f,
		+8.0f, +0.0f, -12.0f,
		+0.1f, +0.9f, +0.1f,

		-2.0f, +6.0f, -12.0f,
		+0.1f, +0.1f, +0.9f,
		+2.0f, +6.0f, -12.0f,
		+0.1f, +0.1f, +0.9f,
		+0.0f,  +8.0f, -12.0f,
		+0.1f, +0.1f, +0.9f,

		+2.0f, -6.0f, -12.0f,
		+0.0f, +0.0f, +0.0f,
		-2.0f, -6.0f, -12.0f,
		+0.0f, +0.0f, +0.0f,
		0.0f,  -8.0f, -12.0f,
		+0.0f, +0.0f, +0.0f,

		-6.0f, +0.0f, -9.0f,
		+1.0f, +1.0f, +1.0f,
		+6.0f, +0.0f, -9.0f,
		+1.0f, +1.0f, +1.0f,
		+0.0f, +6.0f, -9.0f,
		+1.0f, +1.0f, +1.0f,
		+0.0f, -6.0f, -9.0f,
		+1.0f, +1.0f, +1.0f,

	};
	GLushort indices4[] = { 0,1,2,0,4,5,0,7,8,0,10,11,1,2,3,4,5,6,7,8,9,10,11,12,13,1,0,13,2,0,13,1,3,13,2,3,14,0,4,14,0,5,14,4,6,14,5,6,15,0,7,15,0,8,15,7,9,15,8,9,16,0,10,16,0,11,16,10,12,16,11,12 };
	glGenVertexArrays(1, &FanID);
	glBindVertexArray(FanID);
	GLuint vbo4;
	glGenBuffers(1, &vbo4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Fan), Fan, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx4;
	glGenBuffers(1, &vbo_idx4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices4), indices4, GL_STATIC_DRAW);

	const GLfloat Gem[] = {
		+5.0f, -4.0f, -9.0f,
		+1.0f, +1.0f, +1.0f,
		+5.0f, -4.0f, -12.0f,
		+0.6f, +0.8f, +1.0f,
		+3.0f, -4.0f, -10.0f,
		+0.6f, +0.8f, +1.0f,
		+7.0f, -4.0f, -10.0f,
		+0.6f, +0.8f, +1.0f,
		+5.0f, -1.0f, -10.0f,
		+0.6f, +0.8f, +1.0f,
		+5.0f, -7.0f, -10.0f,
		+0.6f, +0.8f, +1.0f,
	};
	GLushort indices5[] = { 0,1,2,0,1,3,0,4,2,1,4,2,1,4,3,0,4,3,2,0,5,3,0,5,3,1,5,2,1,5};
	glGenVertexArrays(1, &GemID);
	glBindVertexArray(GemID);
	GLuint vbo5;
	glGenBuffers(1, &vbo5);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Gem), Gem, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx5;
	glGenBuffers(1, &vbo_idx5);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices5), indices5, GL_STATIC_DRAW);
	
	const GLfloat HouseBase[] = {
		+20.0f, -10.0f, -10.0f,
		+1.0f, +1.0f, +1.0f,
		+20.0f, -10.0f, -13.0f,
		+1.0f, +1.0f, +1.0f,
		+12.0f, -10.0f, -10.0f,
		+1.0f, +1.0f, +1.0f,
		+12.0f,-10.0f, -13.0f,
		+1.0f, +1.0f, +1.0f,
		+20.0f, -3.0f, -10.0f,
		+1.0f, +1.0f, +1.0f,
		+20.0f, -3.0f, -13.0f,
		+1.0f, +1.0f, +1.0f,
		+12.0f, -3.0f, -10.0f,
		+1.0f, +1.0f, +1.0f,
		+12.0f,-3.0f, -13.0f,
		+1.0f, +1.0f, +1.0f,
	};
	GLushort indices6[] = { 0,1,2,1,2,3,0,1,4,1,4,5,1,3,5,3,5,7,3,7,6,3,2,6,0,2,4,2,4,6,4,5,6,5,6,7 };
	glGenVertexArrays(1, &HouseBaseID);
	glBindVertexArray(HouseBaseID);
	GLuint vbo6;
	glGenBuffers(1, &vbo6);
	glBindBuffer(GL_ARRAY_BUFFER, vbo6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(HouseBase), HouseBase, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx6;
	glGenBuffers(1, &vbo_idx6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices6), indices6, GL_STATIC_DRAW);
	
	const GLfloat Ceiling[] = {
		+20.0f, -3.0f, -9.0f,
		+0.9f, +0.1f, +0.1f,
		+20.0f, -3.0f, -14.0f,
		+1.0f, +0.0f, +0.0f,
		+11.0f, -3.0f, -9.0f,
		+1.0f, +0.0f, +0.0f,
		+11.0f,-3.0f, -14.0f,
		+1.0f, +0.0f, +0.0f,
		+16.0f, +2.0f, -11.0f,
		+1.0f, +0.0f, +0.0f,
	};
	GLushort indices7[] = { 4,0,1,4,0,2,4,1,3,4,2,3,0,1,2,3};
	glGenVertexArrays(1, &CeilingID);
	glBindVertexArray(CeilingID);
	GLuint vbo7;
	glGenBuffers(1, &vbo7);
	glBindBuffer(GL_ARRAY_BUFFER, vbo7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ceiling), Ceiling, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLuint vbo_idx7;
	glGenBuffers(1, &vbo_idx7);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices7), indices7, GL_STATIC_DRAW);
}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	glClearColor(0.8f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	GLint projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");
	projectionMatrix = glm::perspective(1.9f, 16.0f / 9.0f, 5.0f, 40.0f);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glBindVertexArray(GrassID);
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(FanID);
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	modelTransformMatrix = glm::rotate(glm::mat4(), rotateFan, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(PyramidID);
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(x_press_num*delta, y_press_num*delta,0.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(WallID);
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(HouseBaseID);
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f,0.0f, z_press_num*delta));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(CeilingID);
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, z_press_num*delta));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(GemID);
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(-4.0f, +5.0f, +10.0f));
	modelTransformMatrix = glm::scale(modelTransformMatrix, glm::vec3(scale, scale, scale));
	modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(+4.0f, -5.0f, -10.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_SHORT, 0);

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char *argv[])
{
	/*Initialization*/
	glutInit(&argc, argv);
	glutCreateWindow("OpenGL Test");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}