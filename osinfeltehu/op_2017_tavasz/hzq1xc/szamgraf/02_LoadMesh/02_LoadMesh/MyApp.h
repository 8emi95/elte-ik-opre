#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// mesh
#include "ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	double x[6], y[6];

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// bels� elj�r�sok
	void DrawMesh2(double, double);
	void DrawMesh(double,double);
	glm::vec3 ToDescartes(float u,float v);

	glm::vec3 eye = glm::vec3(40, 40, 40);
	glm::vec3 up = glm::vec3(0, 1, 0);
	float u = 0;
	float v = M_PI/2.0;

	glm::vec3 fw = ToDescartes(u,v);
	glm::vec3 at = eye + fw;
	bool isLeft =false;
	int sdlget;
	bool ketto=false;

	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja

	// transzform�ci�s m�trixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// m�trixok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;
	GLuint	m_loc_texture2;
	GLuint	m_loc_t;

	GLuint	m_loc_world;
	GLuint	m_loc_worldIT;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�
	GLuint m_ibID;  // index buffer object er�forr�s azonos�t�
	GLuint m_waterTextureID; // f�jlb�l bet�lt�tt text�ra azonos�t�ja
	GLuint m_waterTextureID2; // f�jlb�l bet�lt�tt text�ra azonos�t�ja

	struct Vertex
	{
		glm::vec3 p; // poz�ci�
		glm::vec3 n; // sz�n
		glm::vec2 t; // text�ra koordin�t�k
	};

	// mesh adatok
	Mesh *m_mesh;
	Mesh *m_mesh2;
};

