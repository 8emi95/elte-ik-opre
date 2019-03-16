#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgViewerd.lib")
#else
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgViewer.lib")
#endif

#pragma warning(disable : 4482 )

void calc(float x, float y,   //a referencia szerint kapott t�mb�kbe belem�soljuk a ...-t
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
	){
	float u = 2 * osg::PI * x;
	float v = osg::PI * y;
	vertices->push_back(osg::Vec3(sin(u)*sin(v), cos(v), cos(u)*sin(v)));   //g�mbi koordin�t�k (ismert k�plet)
	normals->push_back(osg::Vec3(sin(u)*sin(v), cos(v), cos(u)*sin(v)));   //norm�lvektor (ugyanaz!!)
	texcoords->push_back(osg::Vec2(1-x, y));   //alapb�l x volt az 1-x helyett. Ez megv�ltoztatja az egyenl�t� (????) k�r�lj�r�si ir�ny�t (????).
											   //Ez mondja meg, hogy pl. Afrika t�kr�zve legyen-e a (f�ld)g�mb�nk�n
	//ugye:
	//a text�ra egy (0,0)-t�l (1,1)-ig lev� n�gyzet (egy k�p) �s ezt akarjuk r�rakni a g�mbre
}

int main(int argc, char** argv)
{
	// poz�ci�k
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;

	const int N = 16;   //feloszt�s finoms�ga (??)
	float delta = 1.0 / N;
	for (int i = 0; i<N; ++i){
		for (int j = 0; j<N; ++j){
			float x = i*delta;
			float y = j*delta;
			// 1. h�romsz�g: x,y x+delta,y y+delta,x
			calc(x, y, vertices, normals, texcoords);
			calc(x, y + delta, vertices, normals, texcoords);
			calc(x + delta, y, vertices, normals, texcoords);
			// 2. h�romsz�g: x+delta,y x+delta,y+delta y+delta,x
			calc(x + delta, y, vertices, normals, texcoords);
			calc(x, y + delta, vertices, normals, texcoords);
			calc(x + delta, y + delta, vertices, normals, texcoords);
			//a k�t h�romsz�g az ismert technik�t val�s�tj�k meg, vagyis egy n�gyzetet (t�glalapot??) adnak
		}
	}

	// n�gysz�g geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setUseVertexBufferObjects(true);

	// �ll�tsuk be, hogy a VBO-ba milyen adatok ker�ljenek
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	quad->setTexCoordArray(0, texcoords.get());
	// kirajzoland� primit�v meghat�roz�sa
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 6 * N*N));

	// text�ra bet�lt�se
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/moon256128.TGA");
	texture->setImage(image.get());
	texture->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);

	// rakjuk be egy geode-ba a quad-ot, mint kirajzoland� elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable(quad.get());

	// 0-�s mintav�telez�re rakjuk r� a text�r�t
	root->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	osg::StateSet* state = root->getOrCreateStateSet();   //csin�lunk egy egy cs�csb�l �ll� sz�nt�rgr�fot
	// kikapcsoljuk a megvil�g�t�st
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::OFF |
		osg::StateAttribute::PROTECTED);   //ugye: ha a sz�nt�rgr�f sz�l�j�re be�ll�tunk valamit, akkor az leterjed a gyerekre is.
										   //Viszont ha PROTECTED-el be�ll�tjuk a gyereket, akkor az j� lesz (????)
	// bekapcsoljuk a h�trafel� n�z� lapok eldob�s�t
	state->setMode(GL_CULL_FACE, osg::StateAttribute::ON);

	// hozzuk l�tre a viewer-t �s �ll�tsuk be a gy�keret megjelen�tend� adatnak
	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());

	// a (20,20) kezdeti poz�ci�ba hozzunk l�tre egy 640x480-as ablakot
	viewer.setUpViewInWindow(20, 20, 640, 480);
	viewer.realize();

	// �rjuk ki egy obj f�jlba a parametrikus fel�let�nket!
	osgDB::writeNodeFile(*(root.get()), "fold.obj");   //megadjuk a sz�nt�rgr�f egy cs�cspontj�t (most csak egy van am�gy is) �s a f�jlnevet.
													   //A f�jln�vb�l a kiterjeszt�s alapj�n kital�lja az export�l�s t�pus�t (???? de ronda...)

	// adjuk �t a vez�rl�st a viewer-nek
	return viewer.run();
}