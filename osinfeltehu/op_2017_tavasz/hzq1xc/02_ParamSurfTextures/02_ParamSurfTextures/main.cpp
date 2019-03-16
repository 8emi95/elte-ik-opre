
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
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

/*void calc(float x, float y,
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
) {
	float u = 2 * osg::PI * x;
	float v = 3 * y;
	vertices->push_back(osg::Vec3(cos(u), v, sin(u)));
	normals->push_back(osg::Vec3(cos(u), 0, sin(u)));
	texcoords->push_back(osg::Vec2(x, y));
}
*/


void gomb(float x, float y,
osg::ref_ptr<osg::Vec3Array>& vertices,
osg::ref_ptr<osg::Vec3Array>& normals,
osg::ref_ptr<osg::Vec2Array>& texcoords
) {
float u = 2 * osg::PI*x;
float v = osg::PI*y ;
vertices->push_back(osg::Vec3(cos(u)*sin(v), cos(v), sin(u)*sin(v)));
normals->push_back(osg::Vec3(cos(u), 0, sin(u)));
texcoords->push_back(osg::Vec2(x, y));
}


/*osg::ref_ptr<osg::Geometry> kor() {
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;

	//v->push_back(osg::Vec3(0, 0, 0));
	for (int i = 0;i <= 16;i++)
	{
		float in = 2*osg::PI/16.*i;
		float in1 = 2*osg::PI / 16.*(i+1);
		v->push_back(osg::Vec3(cos(in), 0, sin(in)));
		v->push_back(osg::Vec3(cos(in1), 0, sin(in1)));
		v->push_back(osg::Vec3(0.5*cos(in), 0, 0.5*sin(i)));
	}

	for (int i = 0; i < 24; ++i)
		n->push_back(osg::Vec3(0, 1, 0));

	t->push_back(osg::Vec2(0.5, 0.5));
	for (int i = 0;i <= 16;++i)
	{
		float alpha = 2 * osg::PI*i / 16;
		float alpha2 = 2 * osg::PI*(i + 1) / 16;
		t->push_back(osg::Vec2((cos(i) + 1) / 2, (sin(i) + 1) / 2));
		t->push_back(osg::Vec2((cos(i+1) + 1) / 2, (sin(i+1) + 1) / 2));
		t->push_back(osg::Vec2((cos(i) + 1) / 2, (sin(i) + 1) / 2));
	}

	osg::ref_ptr<osg::Geometry> kor = new osg::Geometry;

	kor->setVertexArray(v);
	kor->setNormalArray(n);
	kor->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	kor->setTexCoordArray(0, t);
	kor->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 18));

	return kor;
}*/

osg::ref_ptr<osg::Geometry> kor()
{
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;

	v->push_back(osg::Vec3(0, 0, 0));
	for (int i = 0;i <= 16;++i)
	{
		float alpha = 2 * osg::PI*i / 16;
		v->push_back(osg::Vec3(cos(alpha), 0, sin(alpha)));
	}

	for (int i = 0; i < 18; ++i)
		n->push_back(osg::Vec3(0, -1, 0));

	t->push_back(osg::Vec2(0.5, 0.5));
	for (int i = 0;i <= 16;++i)
	{
		float alpha = 2 * osg::PI*i / 16;
		t->push_back(osg::Vec2((cos(alpha) + 1) / 2, (sin(alpha) + 1) / 2));
	}

	osg::ref_ptr<osg::Geometry> kor = new osg::Geometry;

	kor->setVertexArray(v);
	kor->setNormalArray(n);
	kor->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	kor->setTexCoordArray(0, t);
	kor->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 18));

	return kor;
}

osg::ref_ptr<osg::Geometry> kor2()
{
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;

	v->push_back(osg::Vec3(0, 0, 0));
	for (int i = 0;i <= 16;++i)
	{
		float alpha = 2 * osg::PI*i / 16;
		v->push_back(osg::Vec3(cos(alpha), 0, sin(alpha)));
	}

	for (int i = 0; i < 18; ++i)
		n->push_back(osg::Vec3(0, -1, 0));

	t->push_back(osg::Vec2(0.5, 0.5));
	for (int i = 0;i <= 16;++i)
	{
		float alpha = 2 * osg::PI*i / 16;
		t->push_back(osg::Vec2((cos(alpha) + 1) / 2, (sin(alpha) + 1) / 2));
	}

	osg::ref_ptr<osg::Geometry> kor2 = new osg::Geometry;

	kor2->setVertexArray(v);
	kor2->setNormalArray(n);
	kor2->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	kor2->setTexCoordArray(0, t);
	kor2->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 18));

	return kor2;
}



int main(int argc, char** argv)
{

	osg::ref_ptr<osg::Geometry> k = kor();
	osg::ref_ptr<osg::Geometry> k1 = kor2();

	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;

	const int N = 16;
	float delta = 1.0 / N;
	for (int i = 0; i<N; ++i) {
		for (int j = 0; j<N; ++j) {
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			gomb(x, y + delta, vertices, normals, texcoords);
			gomb(x, y, vertices, normals, texcoords);
			
			gomb(x + delta, y, vertices, normals, texcoords);
			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			gomb(x, y + delta, vertices, normals, texcoords);
			gomb(x + delta, y, vertices, normals, texcoords);
			
			gomb(x + delta, y + delta, vertices, normals, texcoords);
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
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/saturn_preview.bmp");
	texture->setImage(image.get());
	texture->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);

	osg::ref_ptr<osg::Texture2D> texture2 = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image2 = osgDB::readImageFile("Images/ring_preview.tga");
	texture2->setImage(image2.get());
	texture2->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture2->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture2->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture2->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);

	
	osg::ref_ptr<osg::Geode> alap1 = new osg::Geode;
	alap1->addDrawable(k1);




	osg::ref_ptr<osg::Geode> henger = new osg::Geode;
	henger->addDrawable(quad);

	osg::ref_ptr<osg::Geode> alap = new osg::Geode;
	alap->addDrawable(k);

	osg::ref_ptr<osg::MatrixTransform> trf1 = new osg::MatrixTransform;
	//trf1->setMatrix(osg::Matrix::translate(osg::Vec3(3, 3, 3)));
	trf1->addChild(henger);



	osg::ref_ptr<osg::MatrixTransform> trf3 = new osg::MatrixTransform;
	trf3->setMatrix(osg::Matrix::scale(osg::Vec3(2.5, 1, 2.5))*osg::Matrix::rotate(osg::PI, osg::Vec3(26.73 / 180, 0, 0)));
	trf3->addChild(alap);

	osg::ref_ptr<osg::MatrixTransform> trf4 = new osg::MatrixTransform;
	//trf4->setMatrix(osg::Matrix::rotate(osg::PI, osg::Vec3(1, 0, 0))*osg::Matrix::translate(osg::Vec3(0, 3, 0)));
	trf4->setMatrix(osg::Matrix::scale(osg::Vec3(2.5, 1, 2.5))*osg::Matrix::rotate(osg::PI, osg::Vec3(1, 0, 0))*osg::Matrix::rotate(osg::PI, osg::Vec3(26.73/180, 0, 0)));
	trf4->addChild(alap1);

	osg::ref_ptr<osg::Group> boly = new osg::Group;
	boly->addChild(trf1);


	osg::ref_ptr<osg::Group> gyur = new osg::Group;
	gyur->addChild(trf3);
	gyur->addChild(trf4);
	// rakjuk be egy geode-ba a quad-ot, mint kirajzolandó elemet!
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(boly);
	root->addChild(gyur);

	// 0-ás mintavételezõre rakjuk rá a textúrát
	boly->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	gyur->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture2.get());
	osg::StateSet* state = root->getOrCreateStateSet();
	// kikapcsoljuk a megvilágítást
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::ON |
		osg::StateAttribute::PROTECTED);
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
	osgDB::writeNodeFile(*(boly.get()), "planet.obj");
	osgDB::writeNodeFile(*(gyur.get()), "ring.obj");
	// adjuk át a vezérlést a viewer-nek
	return viewer.run();
}