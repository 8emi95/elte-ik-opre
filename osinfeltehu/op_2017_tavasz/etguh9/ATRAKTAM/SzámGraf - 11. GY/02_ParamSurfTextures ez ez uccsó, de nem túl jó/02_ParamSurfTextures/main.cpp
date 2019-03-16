//Megjegyz�s:
//OSG-ben alapb�l a kamerapoz�ci�ban van a f�nyforr�s
//A kamer�t pedig alapb�l r�poz�cion�lja az objektumomra (gondolom akkor, ha csak egy van.) De mi van t�bb is van????

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

void calc(float x, float y,   //a referencia szerint kapott t�mb�kbe belem�soljuk a ...-t
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
	){
	float u = 2 * osg::PI * x;
	float v = osg::PI * y;
	vertices->push_back(osg::Vec3(cos(u), v, sin(u)));   //henger koordin�t�i
	normals->push_back(osg::Vec3(cos(u), 0, sin(u)));   //norm�lvektor (gondolkozni!!)
	texcoords->push_back(osg::Vec2(1-x, y));   //alapb�l x volt az 1-x helyett. Ez megv�ltoztatja az egyenl�t� (????) k�r�lj�r�si ir�ny�t (????).
											   //Ez mondja meg, hogy pl. Afrika t�kr�zve legyen-e a (f�ld)g�mb�nk�n
	//ugye:
	//a text�ra egy (0,0)-t�l (1,1)-ig lev� n�gyzet (egy k�p) �s ezt akarjuk r�rakni a g�mbre
}

osg::ref_ptr<osg::Geometry> kor()
{
	//Ugye a text�ra egy n�gyzet.
	//K�rt akarunk.
	//Eltorz�thatn�nk a text�r�t, de most csak "lev�gjuk a sarkokat"

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;

	v->push_back(osg::Vec3(0, 0, 0));
	for (int i = 0; i <= 16; ++i)   //az utols�t m�g egyszer hozz�veszi, ez�rt �runk '<='-t
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
		t->push_back(osg::Vec2((cos(alpha) + 1) / 2, (sin(alpha) + 1) / 2));   //[-1;1] -> [0;1] lek�pez�s hozz�rendel�se: f(x) = (x+1)/2
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



	//csin�lunk egy nagyobb (??) sz�nt�rgr�fot:   //************************************
	osg::ref_ptr<osg::Geode> henger = new osg::Geode;
	henger->addDrawable(quad);

	osg::ref_ptr<osg::Geode> alap = new osg::Geode;
	alap->addDrawable(k);

	//1. transzform�ci�:   //************************************
	osg::ref_ptr<osg::MatrixTransform> trf1 = new osg::MatrixTransform;
	trf1->setMatrix(osg::Matrix::translate(osg::Vec3(3,3,3)));
	trf1->addChild(henger);   //Legyen a henger a gyereke. Emiatt eltol�dik a henger (de mi�rt??)

	//2. transzform�ci�:   //************************************   (ezt kivehetn�nk :) )
	osg::ref_ptr<osg::MatrixTransform> trf2 = new osg::MatrixTransform;
	trf2->setMatrix(osg::Matrix::scale(osg::Vec3(1, 1, 1)));   //merthogy nem csin�l semmit
	trf2->addChild(henger);   //Legyen a henger a gyereke

	//3. transzform�ci�:   //************************************
	osg::ref_ptr<osg::MatrixTransform> trf3 = new osg::MatrixTransform;
	trf3->setMatrix(osg::Matrix::scale(osg::Vec3(1, -1, 1)));
	trf3->addChild(alap);

	//4. transzform�ci�:   //************************************
	osg::ref_ptr<osg::MatrixTransform> trf4 = new osg::MatrixTransform;
	trf4->setMatrix(osg::Matrix::translate(osg::Vec3(0, osg::PI, 0)));
	trf4->addChild(alap);

	//L�trehozunk egy Group-ot:   //************************************
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(trf1);
	root->addChild(trf2);
	root->addChild(trf3);
	root->addChild(trf4);



	// rakjuk be egy geode-ba a quad-ot, mint kirajzoland� elemet!   //most egy Group-ba rakjuk, ez�rt ezt a r�szt t�r�lni kell
	/*osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable(quad.get());*/

	// 0-�s mintav�telez�re rakjuk r� a text�r�t
	root->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	osg::StateSet* state = root->getOrCreateStateSet();   //csin�lunk egy egy cs�csb�l �ll� sz�nt�rgr�fot
	// kikapcsoljuk a megvil�g�t�st
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::ON |
		osg::StateAttribute::PROTECTED);   //ugye: ha a sz�nt�rgr�f sz�l�j�re be�ll�tunk valamit, akkor az leterjed a gyerekre is.
										   //Viszont ha PROTECTED-el be�ll�tjuk a gyereket, akkor az j� lesz (????)
	// bekapcsoljuk a h�trafel� n�z� lapok eldob�s�t
	state->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	state->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

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