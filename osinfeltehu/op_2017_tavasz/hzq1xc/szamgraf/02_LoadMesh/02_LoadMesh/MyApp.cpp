#include "MyApp.h"
#include "GLUtils.hpp"

#include <GL/GLU.h>
#include <math.h>
#include <stdlib.h>   
#include <time.h> 

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_waterTextureID = 0;

	m_mesh = 0;
}


CMyApp::~CMyApp(void)
{
}



glm::vec3 CMyApp::ToDescartes(float u, float v) {
	return glm::vec3(cosf(u)*sinf(v), cosf(u), sinf(u)*sinf(v));
}


bool CMyApp::Init()
{
	//rand init
	srand(time(NULL));
	
	
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		//          x,  y, z               nx,ny,nz			 s, t
		{glm::vec3(-10, 0, -10), glm::vec3( 0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3(-10, 0,  10), glm::vec3( 0, 1, 0), glm::vec2(0, 1)},
		{glm::vec3( 10, 0, -10), glm::vec3( 0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3( 10, 0,  10), glm::vec3( 0, 1, 0), glm::vec2(1, 1)},
	};

	// indexpuffer adatai
    GLushort indices[]=
    {
		// 1. háromszög
        0,1,2,
		// 2. háromszög
        2,1,3,
    };


	
	

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erőforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// erről a rendszermemóriabeli címről olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az első 3 float sizeof(Vertex)-enként lesz az első attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexű" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexű attribútum hol kezdődik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// textúrakoordináták bekapcsolása a 2-es azonosítójú attribútom csatornán
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(
		2,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3)) );

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	// a VAO észreveszi, hogy bind-olunk egy index puffert és feljegyzi, hogy melyik volt ez!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés előtt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonosítója, amiből egy változóhoz szeretnénk hozzárendelést csinálni
							0,				// a VAO-beli azonosító index
							"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation( m_programID, 1, "vs_in_normal");
	glBindAttribLocation( m_programID, 2, "vs_in_tex0");

	// illesszük össze a shadereket (kimenő-bemenő változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::cout << "Kaki van a shader forditásában.." << std::endl;
	}

	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	//
	// other initializations
	//

	// set the projection matrix
	m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);

	// query the IDs of the shader uniform variables
	m_loc_world = glGetUniformLocation(m_programID, "world");
	m_loc_worldIT = glGetUniformLocation(m_programID, "worldIT");
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");
	m_loc_texture = glGetUniformLocation(m_programID, "texture");

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egyéb inicializálás
	//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation( m_programID, "MVP");

	m_loc_texture = glGetUniformLocation( m_programID, "texture" );
	m_loc_texture2 = glGetUniformLocation(m_programID, "texture2");
	m_loc_t = glGetUniformLocation(m_programID, "t");
	//
	// egyéb erőforrások betöltése
	//

	// textúra betöltése
	m_waterTextureID = TextureFromFile("as.tga");
	m_waterTextureID2 = TextureFromFile("can.tga");

	// mesh betoltese
	m_mesh = ObjParser::parse("a.obj");
	m_mesh->initBuffers();
	m_mesh2 = ObjParser::parse("b.obj");
	m_mesh2->initBuffers();

	for (int i = 0; i < 6;++i) {
		x[i] = ((double)rand()) / (double)RAND_MAX;
		y[i] = ((double)rand()) / (double)RAND_MAX;

		std::cout << x[i] << " " << y[i] << " ";
	}
	

	return true;
}

void CMyApp::Clean()
{
	delete m_mesh;
	glDeleteTextures(1, &m_waterTextureID);

	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// nézeti transzformáció beállítása
	m_matView = glm::lookAt(eye, at, up);		// felfelé mutató irány a világban
}



void CMyApp::DrawMesh2(double x, double y)
{
	// a mesh kirajzolasahoz hasznalt shader bekapcsolasa
	glUseProgram(m_programID);

	/*x(u, v) y(u, v) z(u, v)
	R sugarú gömb R · cos 2πu · sin πv 2πu · sin πv R · cos πv*/


	float R = 6;
	float u = 2 * M_PI * x;
	float v = M_PI * y;
	float cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);


	//m_matWorld = glm::translate<float>(glm::vec3(R*cu*sv, R*su*sv, R*cv));
	m_matWorld = glm::translate<float>(glm::vec3(R*cu*sv, R*su*sv, R*cv));
	glm::mat4 mvp = m_matProj * m_matView * m_matWorld;
	// majd küldjük át a megfelelő mátrixokat!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre töltsünk át adatot
		1,			// egy darab mátrixot
		GL_FALSE,	// NEM transzponálva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

					   // aktiváljuk a 0-és textúra mintavételező egységet
	glActiveTexture(GL_TEXTURE0);
	// aktiváljuk a generált textúránkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// textúra mintavételező és shader-beli sampler2D összerendelése
	glUniform1i(m_loc_texture,	// ezen azonosítójú sampler 2D
		0);				// olvassa az ezen indexű mintavételezőt

	glActiveTexture(GL_TEXTURE1);
	// aktiváljuk a generált textúránkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID2);
	// textúra mintavételező és shader-beli sampler2D összerendelése
	glUniform1i(m_loc_texture2,	// ezen azonosítójú sampler 2D
		1);				// olvassa az ezen indexű mintavételezőt
	glUniform1f(m_loc_t,sin( (SDL_GetTicks()-sdlget) / 9000.0 * 2 * M_PI*0.5 + 0.5));
	
	m_mesh2->draw();

	glUseProgram(0);
}


void CMyApp::DrawMesh(double x, double y)
{
	// a mesh kirajzolasahoz hasznalt shader bekapcsolasa
	glUseProgram(m_programID);

	/*x(u, v) y(u, v) z(u, v)
	R sugarú gömb R · cos 2πu · sin πv 2πu · sin πv R · cos πv*/


	float R = 6;
	float u = 2 * M_PI * x;
	float v = M_PI * y;
	float cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);


	//m_matWorld = glm::translate<float>(glm::vec3(R*cu*sv, R*su*sv, R*cv));
	m_matWorld = glm::translate<float>(glm::vec3(R*cu*sv, R*su*sv, R*cv));
	glm::mat4 mvp = m_matProj * m_matView * m_matWorld;
	// majd küldjük át a megfelelő mátrixokat!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre töltsünk át adatot
		1,			// egy darab mátrixot
		GL_FALSE,	// NEM transzponálva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

					   // aktiváljuk a 0-és textúra mintavételező egységet
	glActiveTexture(GL_TEXTURE0);
	// aktiváljuk a generált textúránkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// textúra mintavételező és shader-beli sampler2D összerendelése
	glUniform1i(m_loc_texture,	// ezen azonosítójú sampler 2D
		0);				// olvassa az ezen indexű mintavételezőt


	m_mesh->draw();

	glUseProgram(0);
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < 3; i++)
	{
		DrawMesh(x[i], y[i]);
		DrawMesh2(x[i+3], y[i+3]);
	}
	
	
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	glm::vec3 sd = glm::normalize(glm::cross(up, fw));
	switch (key.keysym.sym) {
	case SDLK_w:
		eye += fw;
		at += fw;
		break;
	case SDLK_s:
		eye -= fw;
		at -= fw;
		break;
	case SDLK_a:
		eye += sd;
		at += sd;
	break;
	case SDLK_d:
		eye -= sd;
		at -= sd;
		break;
	case SDLK_2:
		ketto=!ketto;
		//sdlget = SDL_GetTicks();
		break;
	}

}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (isLeft) {
		u += mouse.xrel / 100.0;
		v += mouse.yrel / 100.0;

		v = glm::clamp(v, 0.1f, 3.0f);
		fw = ToDescartes(u, v);
		at = eye + fw;
	}
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT) {
		isLeft = true;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT) {
		isLeft = false;
	}
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}