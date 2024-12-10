#include "scene.h"
#include "utils.h"
#include "model.h"
#include "GL/freeglut.h"
#include "Cube.data"
#include "car.data"
//#include <Windows.h>
#include "light.h"
#include <stdint.h>


SpotLight sLight(GL_LIGHT0);
glm::vec3 lightDir(0.0, 0.3, -1.0);

bool leftButtonDown = false;
float cameraAngle = 0.0f;
constexpr float pi = 3.1415926;

bool bLight = false;

float Angle2Radian(float angle)
{
	return angle * pi / 180.0f;
}

glm::mat4 viewMatrix, projectionMatrix;
glm::vec3 cameraPos(0.0f, 3.0f, 8.0f);
glm::vec3 cameraCenter(0.0f, 3.0f, 0.0f);
glm::vec3 forward(0.0f, 0.0f, -1.0f);
Model sky;
Model car;
float carSpeed = 0.0f;
float carPos = 0.0f;
GLuint roadTexture;
GLuint treeTexture;
GLuint grassTexture;
constexpr float maxSpeed = 20.0f;

uint32_t  lastTime;
uint32_t  startTime;
bool start = false;
bool end = false;

#define NUM_POINTS 200
#define MIN_HEIGHT 0.0f
#define MAX_HEIGHT 60.0f
float height[NUM_POINTS];

bool carView = true;


uint32_t getTickCount() {
    return glutGet(GLUT_ELAPSED_TIME);
}

void InitHeight()
{
	float step = (MAX_HEIGHT - MIN_HEIGHT) / (NUM_POINTS - 1);

	for (int i = 0; i < NUM_POINTS; i++) {
		height[i] = MIN_HEIGHT + i * step;
	}

	// Add some smooth variations using sine function
	for (int i = 0; i < NUM_POINTS; i++) {
		float variation = sin((float)i / NUM_POINTS * 2 * pi) * (MAX_HEIGHT - MIN_HEIGHT) / 5;
		height[i] += variation;

		// Ensure the height is within the range [MIN_HEIGHT, MAX_HEIGHT]
		if (height[i] < MIN_HEIGHT) {
			height[i] = MIN_HEIGHT;
		}
		else if (height[i] > MAX_HEIGHT) {
			height[i] = MAX_HEIGHT;
		}
	}
}

void UpdateCamera();
void Init() {
	sky.Init(cubeData.c_str());
	sky.mShader->Init("Res/skybox.vs", "Res/skybox.fs");
	sky.mShader->SetTextureCube("U_Texture",CreateTextureCubeFromBMP("Res/front.bmp", "Res/back.bmp",
		"Res/left.bmp", "Res/right.bmp", "Res/bottom.bmp", "Res/top.bmp"));
	viewMatrix = glm::lookAt(cameraPos, cameraCenter, glm::vec3(0.0f,1.0f,0.0f));

	std::string carData = carData1 + carData2 + carData3 + carData4 + carData5
		+ carData6 + carData7 + carData8 + carData9;

	car.Init(carData.c_str());
	car.mShader->Init("Res/light.vs", "Res/light.fs");
	car.mShader->SetTexture("U_MainTexture", CreateTexture2DFromBMP("Res/car.bmp"));

	roadTexture = CreateTexture2DFromBMP("Res/road.bmp");
	treeTexture = CreateTexture2DFromBMP("Res/tree.bmp");
	grassTexture = CreateTexture2DFromBMP("Res/grass.bmp");

	lastTime = getTickCount();

	InitHeight();

	sLight.SetAmbientColor(0.3, 0.3, 0.3, 1.0);
	sLight.SetDiffuseColor(0.8, 0.8, 0.8, 1.0);
	sLight.SetSpecularColor(0.1, 0.1, 0.1, 1.0);
	sLight.SetConstAttenuation(1.0);
	sLight.SetLinearAttenuation(0.0001);
	sLight.SetConstAttenuation(0.00001);

	sLight.SetDirection(lightDir.x, lightDir.y, lightDir.z);
	sLight.SetPosition(0.0,0.5, -2.0);
	sLight.SetCutoff(20);
	sLight.SetExponent(10);
	sLight.Update(0.0, 0.0, 0.0);

	// glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	// glEnable(GL_COLOR_MATERIAL);
}
void SetViewPortSize(float width, float height) {
	projectionMatrix = glm::perspective(50.0f, width / height, 0.1f, 100.0f);
}

void DrawCube(double size)
{
	double hSize = size / 2.0;

	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-hSize, 0.0, hSize);
	glVertex3f(hSize, 0.0, hSize);
	glVertex3f(hSize, 0.0, -hSize);
	glVertex3f(-hSize, 0.0, -hSize);

	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-hSize, size, hSize);
	glVertex3f(hSize, size, hSize);
	glVertex3f(hSize, size, -hSize);
	glVertex3f(-hSize, size, -hSize);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-hSize, 0.0, -hSize);
	glVertex3f(-hSize, 0.0, hSize);
	glVertex3f(-hSize, size, hSize);
	glVertex3f(-hSize, size, -hSize);

	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(hSize, 0.0, -hSize);
	glVertex3f(hSize, 0.0, hSize);
	glVertex3f(hSize, size, hSize);
	glVertex3f(hSize, size, -hSize);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-hSize, 0.0, hSize);
	glVertex3f(hSize, 0.0, hSize);
	glVertex3f(hSize, size, hSize);
	glVertex3f(-hSize, size, hSize);

	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-hSize, 0.0, -hSize);
	glVertex3f(hSize, 0.0, -hSize);
	glVertex3f(hSize, size, -hSize);
	glVertex3f(-hSize, size, -hSize);
	glEnd();
}

void DrawLight(float x, float y, float z)
{
	glColor3f(0.3, 0.3, 0.3);

	float hSize = 0.3;
	float size = 6.0;

	{
		float diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	}
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-hSize + x, 0.0, hSize + z);
	glVertex3f(hSize + x, 0.0, hSize + z);
	glVertex3f(hSize + x, 0.0, -hSize + z);
	glVertex3f(-hSize + x, 0.0, -hSize + z);

	glVertex3f(-hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size, -hSize + z);
	glVertex3f(-hSize + x, size, -hSize + z);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-hSize + x, 0.0, -hSize + z);
	glVertex3f(-hSize + x, 0.0, hSize + z);
	glVertex3f(-hSize + x, size, hSize + z);
	glVertex3f(-hSize + x, size, -hSize + z);

	glVertex3f(hSize + x, 0.0, -hSize + z);
	glVertex3f(hSize + x, 0.0, hSize + z);
	glVertex3f(hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size, -hSize + z);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-hSize + x, 0.0, hSize + z);
	glVertex3f(hSize + x, 0.0, hSize + z);
	glVertex3f(hSize + x, size, hSize + z);
	glVertex3f(-hSize + x, size, hSize + z);

	glVertex3f(-hSize + x, 0.0, -hSize + z);
	glVertex3f(hSize + x, 0.0, -hSize + z);
	glVertex3f(hSize + x, size, -hSize + z);
	glVertex3f(-hSize + x, size, -hSize + z);
	glEnd();
	glPopMatrix();

	//
	hSize = 0.5;
	glColor3f(1.0, 1.0, 0.0);
	{
		float diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	}
	
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size, -hSize + z);
	glVertex3f(-hSize + x, size, -hSize + z);

	glVertex3f(-hSize + x, size + hSize + 0.3, hSize + z);
	glVertex3f(hSize + x, size + hSize + 0.3, hSize + z);
	glVertex3f(hSize + x, size + hSize + 0.3, -hSize + z);
	glVertex3f(-hSize + x, size + hSize + 0.3, -hSize + z);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-hSize + x, size, -hSize + z);
	glVertex3f(-hSize + x, size, hSize + z);
	glVertex3f(-hSize + x, size + hSize + 0.3, hSize + z);
	glVertex3f(-hSize + x, size + hSize + 0.3, -hSize + z);

	glVertex3f(hSize + x, size, -hSize + z);
	glVertex3f(hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size + hSize + 0.3, hSize + z);
	glVertex3f(hSize + x, size + hSize + 0.3, -hSize + z);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size, hSize + z);
	glVertex3f(hSize + x, size + hSize + 0.3, hSize + z);
	glVertex3f(-hSize + x, size + hSize + 0.3, hSize + z);

	glVertex3f(-hSize + x, size, -hSize + z);
	glVertex3f(hSize + x, size, -hSize + z);
	glVertex3f(hSize + x, size + hSize + 0.3, -hSize + z);
	glVertex3f(-hSize + x, size + hSize + 0.3, -hSize + z);
	glEnd();
	glPopMatrix();
}

void DrawGround()
{
	glUseProgram(0);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(glm::value_ptr(viewMatrix));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(glm::value_ptr(projectionMatrix));

	glBindTexture(GL_TEXTURE_2D, roadTexture);
	// road
	glPushMatrix();
	glTranslatef(-3.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-7.0f, 0.0f, 3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(7.0f, 0.0f, 3.0f);
	glTexCoord2f(1.0f, 200.0f); glVertex3f(7.0f, 0.0f, -203.0f);
	glTexCoord2f(0.0f, 200.0f); glVertex3f(-7.0f, 0.0f, -203.0f);
	glEnd();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, treeTexture);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.0f, 0.0f, 3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.0f, 10.0f, 3.0f);
	glTexCoord2f(5.0f, 1.0f); glVertex3f(4.0f, 10.0f, -203.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(4.0f, 0.0f, -203.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, grassTexture);
	// ?ùù
	glBegin(GL_QUADS);
	for (int i = 0; i < 199; ++i)
	{
		glTexCoord2f(float(i) / 25.0, 0.0f); glVertex3f(-9.0f, 0.0f, 3 - i);
		glTexCoord2f(float(i + 1) / 25.0, 0.0f); glVertex3f(-9.0f, 0.0f, 2 - i);
		glTexCoord2f(float(i + 1) / 25.0, 1.0f); glVertex3f(-37.0f, height[i + 1], 2 - i);
		glTexCoord2f(float(i) / 25.0, 1.0f); glVertex3f(-37.0f, height[i], 3 - i);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	for (double z = 0.0; z >= -200.0; z -= 8.0)
	{
		DrawLight(-9, 0, z);
	}

	glColor3f(1.0, 1.0, 1.0);
}
double useTime;
void UpdateCar(float deltaTime)
{
	// static float lastCarPos = 0.0;
	if (carPos >= 200)
	{
		carSpeed = 0.0;
		if (!end)
		{
			useTime = double(getTickCount() - startTime) / 1000.0;
			printf("you cost %.2lf s  \n", useTime);
			end = true;
		}
	}
		
	carPos += carSpeed * deltaTime;

	if (carView)
	{
		cameraPos.y = 3.0;
		cameraPos.z = -carPos + 8;
	}
	else
	{
		cameraPos.y = 1.23;
		cameraPos.z = -carPos - 1.31;
		forward = glm::vec3(0.0f, 0.0f, -1.0);
		cameraAngle = 0.0;
	}
	
	sLight.Update(0.0, 0.0, -carSpeed * deltaTime);
	UpdateCamera();
}

void Draw() {
	uint32_t current = getTickCount();
	float deltaTime = (current - lastTime) / 1000.0f;
	lastTime = current;

	sLight.SetPosition(0.0f, 0.0f, -carPos + 5);

	if (bLight)
	{
		sLight.Enable();
		sLight.SetDirection(lightDir.x, lightDir.y, lightDir.z);
		sLight.Update(0.0, 0.0, 0.0);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	sky.SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	sky.Draw(viewMatrix, projectionMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
	glEnable(GL_DEPTH_TEST);
	car.SetPosition(0.0f, 0.0f, -carPos);
	car.Draw(viewMatrix, projectionMatrix, cameraPos.x, cameraPos.y, cameraPos.z);

	DrawGround();

	UpdateCar(deltaTime);

	glutSwapBuffers();
	glutPostRedisplay();
}

void UpdateCamera()
{
	cameraCenter = cameraPos + forward;
	viewMatrix = glm::lookAt(cameraPos, cameraCenter, glm::vec3(0.0f, 1.0f, 0.0f));
}

void KeyFunc(GLubyte byte, int x, int y)
{
	switch (byte)
	{
	case 27:
		exit(0);
	// case 'w':
	// case 'W':
	// 	cameraPos += forward;
	// 	break;

	// case 's':
	// case 'S':
	// 	cameraPos -= forward;
	// 	break;

	case 'e':
	case 'E':
		carView = !carView;
		break;

	case 'w':
	case 'W':
		carSpeed += 0.1;
		{
			if (!start)
			{
				start = true;
				startTime = getTickCount();
			}
		}
		break;

	case 's':
	case 'S':
		carSpeed -= 0.1;
		break;

	case 'l':
	case 'L':
		bLight = !bLight;
		break;
	}

	if (carSpeed < 0.0)
		carSpeed = 0.0;
	if (carSpeed > maxSpeed)  
		carSpeed = maxSpeed;

	sLight.Update(0.0f, 0.0f, 0.0f);

	UpdateCamera();
}

int lastX = 0;
void MouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			leftButtonDown = true;
			lastX = x;
		}
		else if(state == GLUT_UP)
		{
			leftButtonDown = false;
		}
	}
}

void MotionFunc(int x, int y)
{
	int deltaX = x - lastX;
	lastX = x;
	if (deltaX > 0)
	{
		cameraAngle += 3.0f;
	}		
	else
	{
		cameraAngle -= 3.0f;
	}
	float theta = Angle2Radian(cameraAngle);
	forward.x = sin(theta);
	forward.z = -cos(theta);

	UpdateCamera();
}
