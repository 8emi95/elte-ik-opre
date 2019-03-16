//Megjegyzés:
//OSG-ben alapból a kamerapozícióban van a fényforrás
//A kamerát pedig alapból rápozícionálja az objektumomra (gondolom akkor, ha csak egy van.) De mi van több is van????

#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>   //************************************
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

void calc(float x, float y,   //a referencia szerint kapott tömbökbe belemásoljuk a ...-t
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
	){
	float u = 2 * osg::PI * x;
	float v = osg::PI * y;
	vertices->push_back(osg::Vec3(cos(u), v, sin(u)));   //henger koordinátái
	normals->push_back(osg::Vec3(cos(u), 0, sin(u)));   //normálvektor (gondolkozni!!)
	texcoords->push_back(osg::Vec2(1-x, y));   //alapból x volt az 1-x helyett. Ez megváltoztatja az egyenlítõ (????) körüljárási irányát (????).
											   //Ez mondja meg, hogy pl. Afrika tükrözve legyen-e a (föld)gömbünkön
	//ugye:
	//a textúra egy (0,0)-tól (1,1)-ig levõ négyzet (egy kép) és ezt akarjuk rárakni a gömbre
}

osg::ref_ptr<osg::Geometry> kor()
{
	//Ugye a textúra egy négyzet.
	//Kört akarunk.
	//Eltorzíthatnánk a textúrát, de most csak "levágjuk a sarkokat"

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;

	v->push_back(osg::Vec3(0, 0, 0));
	for (int i = 0; i <= 16; ++i)   //az utolsót még egyszer hozzáveszi, ezért írunk '<='-t
	{
		float alpha = 2 * osg::PI * i / 16;
		v->push_back(osg::Vec3(cos(alpha), 0, sin(alpha)));
	}
	//Most akkor van 1+17(ciklus) = 18 pontunk

	for (int i = 0; i < 18; ++i)
	{
		n->push_back(osg::Vec3(0, 1, 0));
	}

	t->push_back(osg::Vec2(0.5, 0.5));
	for (int i = 0; i <= 16; ++i)
	{
		float alpha = 2 * osg::PI * i / 16;
		t->push_back(osg::Vec2((cos(alpha) + 1) / 2, (sin(alpha) + 1) / 2));   //[-1;1] -> [0;1] leképezés hozzárendelése: f(x) = (x+1)/2
	}


	osg::ref_ptr<osg::Geometry> kor = new osg::Geometry;

	kor->setVertexArray(v);
	kor->setNormalArray(v);
	kor->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	kor->setTexCoordArray(0, t);
	kor->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 18));

	return kor;
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Geometry> k = kor();

	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;

	const int N = 16;   //felosztás finomsága (??)
	float delta = 1.0 / N;
	for (int i = 0; i<N; ++i){
		for (int j = 0; j<N; ++j){
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			calc(x, y, vertices, normals, texcoords);
			calc(x, y + delta, vertices, normals, texcoords);
			calc(x + delta, y, vertices, normals, texcoords);
			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			calc(x + delta, y, vertices, normals, texcoords);
			calc(x, y + delta, vertices, normals, texcoords);
			calc(x + delta, y + delta, vertices, normals, texcoords);
			//a két háromszög az ismert technikát valósítják meg, vagyis egy négyzetet (téglalapot??) adnak
		}
	}

	// négyszög geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setUseVertexBufferObjects(true);

	// állítsuk be, hogy a VBO-ba milyen adatok kerüljenek
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	quad->setTexCoordArray(0, texcoords.get());
	// kirajzolandó primitív meghatározása
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 6 * N*N));

	// textúra betöltése
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/moon256128.TGA");
	texture->setImage(image.get());
	texture->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);



	//csinálunk egy nagyobb (??) színtérgráfot:   //************************************
	osg::ref_ptr<osg::Geode> henger = new osg::Geode;
	henger->addDrawable(quad);

	osg::ref_ptr<osg::Geode> alap = new osg::Geode;
	alap->addDrawable(k);

	//1. transzformáció:   //************************************
	osg::ref_ptr<osg::MatrixTransform> trf1 = new osg::MatrixTransform;
	trf1->setMatrix(osg::Matrix::translate(osg::Vec3(3,3,3)));
	trf1->addChild(henger);   //Legyen a henger a gyereke. Emiatt eltolódik a henger (de miért??)

	//2. transzformáció:   //************************************   (ezt kivehetnénk :) )
	osg::ref_ptr<osg::MatrixTransform> trf2 = new osg::MatrixTransform;
	trf2->setMatrix(osg::Matrix::scale(osg::Vec3(1, 1, 1)));   //merthogy nem csinál semmit
	trf2->addChild(henger);   //Legyen a henger a gyereke

	//3. transzformáció:   //************************************
	osg::ref_ptr<osg::MatrixTransform> trf3 = new osg::MatrixTransform;
	trf3->setMatrix(osg::Matrix::scale(osg::Vec3(1, -1, 1)));
	trf3->addChild(alap);

	//4. transzformáció:   //************************************
	osg::ref_ptr<osg::MatrixTransform> trf4 = new osg::MatrixTransform;
	trf4->setMatrix(osg::Matrix::translate(osg::Vec3(0, osg::PI, 0)));
	trf4->addChild(alap);

	//Létrehozunk egy Group-ot:   //************************************
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(trf1);
	root->addChild(trf2);
	root->addChild(trf3);
	root->addChild(trf4);



	// rakjuk be egy geode-ba a quad-ot, mint kirajzolandó elemet!   //most egy Group-ba rakjuk, ezért ezt a részt törölni kell
	/*osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable(quad.get());*/

	// 0-ás mintavételezõre rakjuk rá a textúrát
	root->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	osg::StateSet* state = root->getOrCreateStateSet();   //csinálunk egy egy csúcsból álló színtérgráfot
	// kikapcsoljuk a megvilágítást
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::ON |
		osg::StateAttribute::PROTECTED);   //ugye: ha a színtérgráf szülõjére beállítunk valamit, akkor az leterjed a gyerekre is.
										   //Viszont ha PROTECTED-el beállítjuk a gyereket, akkor az jó lesz (????)
	// bekapcsoljuk a hátrafelé nézõ lapok eldobását
	state->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	state->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendõ adatnak
	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());

	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
	viewer.setUpViewInWindow(20, 20, 640, 480);
	viewer.realize();

	// írjuk ki egy obj fájlba a parametrikus felületünket!
	osgDB::writeNodeFile(*(root.get()), "fold.obj");   //megadjuk a színtérgráf egy csúcspontját (most csak egy van amúgy is) és a fájlnevet.
													   //A fájlnévbõl a kiterjesztés alapján kitalálja az exportálás típusát (???? de ronda...)

	// adjuk át a vezérlést a viewer-nek
	return viewer.run();
}