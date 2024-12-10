#include "ggl.h"
#include "scene.h"
#include "GL/freeglut.h"

unsigned char * LoadFileContent(const char *path, int &filesize) {
	unsigned char*fileContent = nullptr;
	filesize = 0;
	FILE*pFile = fopen(path, "rb");
	if (pFile) {
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0) {
			rewind(pFile);
			fileContent = new unsigned char[nLen + 1];
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			fileContent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return fileContent;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("final_chenghao_xiong");

	glewInit();
	Init();
	SetViewPortSize(1280, 720);

	glutDisplayFunc(Draw);
	glutKeyboardFunc(KeyFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);

	glutMainLoop();

	return 0;
}