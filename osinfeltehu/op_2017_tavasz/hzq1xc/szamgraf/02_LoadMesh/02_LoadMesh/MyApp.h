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
	// belsõ eljárások
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

	// shaderekhez szükséges változók
	GLuint m_programID; // shaderek programja

	// transzformációs mátrixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// mátrixok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;
	GLuint	m_loc_texture2;
	GLuint	m_loc_t;

	GLuint	m_loc_world;
	GLuint	m_loc_worldIT;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object erõforrás azonosító
	GLuint m_vboID; // vertex buffer object erõforrás azonosító
	GLuint m_ibID;  // index buffer object erõforrás azonosító
	GLuint m_waterTextureID; // fájlból betöltött textúra azonosítója
	GLuint m_waterTextureID2; // fájlból betöltött textúra azonosítója

	struct Vertex
	{
		glm::vec3 p; // pozíció
		glm::vec3 n; // szín
		glm::vec2 t; // textúra koordináták
	};

	// mesh adatok
	Mesh *m_mesh;
	Mesh *m_mesh2;
};

