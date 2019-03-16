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
float v = osg::PI * y;
//kör
//vertices->push_back(osg::Vec3(sin(u)*sin(v), cos(v), cos(u)*sin(v)));
//normals->push_back(osg::Vec3(sin(u)*sin(v), cos(v), cos(u)*sin(v)));//ugyan az mint az eredeti
//texcoords->push_back(osg::Vec2(x, y));//texturakordináták ugyana az mint a paraméter tranzformáció ha szar a textura 1-x et irun x helyére
//henger
vertices->push_back(osg::Vec3(cos(u), v, sin(u)));
normals->push_back(osg::Vec3(cos(u), 0, sin(u)));
texcoords->push_back(osg::Vec2(1 - x, y));
}*/

void torusz(float x, float y,
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
) {
	float u = 2 * osg::PI * x;
	float v = osg::PI * y;

	float cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	float r = 1;
	float R = 5;

	vertices->push_back(osg::Vec3((R + r*cu)*cv, r*su, (R + r*cu)*sv));
	normals->push_back(osg::Vec3((R + r*cu)*cv, r*su, (R + r*cu)*sv));
	texcoords->push_back(osg::Vec2(1-x, y));
}

osg::ref_ptr<osg::Geometry> tor() {
	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;


	const int N = 16;
	float delta = 2. / N;//mekkopra lépéseket teszünk
	for (int i = 0; i<N; ++i) {
		for (int j = 0; j<N; ++j) {
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			torusz(x, y, vertices, normals, texcoords);//egy négyzethez6 hatszor vertex körüljárás
			torusz(x + delta, y, vertices, normals, texcoords);
			torusz(x, y + delta, vertices, normals, texcoords);

			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			torusz(x + delta, y, vertices, normals, texcoords);
			torusz(x + delta, y + delta, vertices, normals, texcoords);
			torusz(x, y + delta, vertices, normals, texcoords);
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
	return quad;
}


//kuppalast
void kupP(float x, float y,
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
) {
	float u = 2 * osg::PI * x;
	float v = osg::PI * y;

	float cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	float r = 3;
	float h = 2;
	//x(u, v) R · (1 − v) · sin 2πu    R ·(1 − v) · cos 2πu         h · v
	vertices->push_back(osg::Vec3((r*(1-y)*su), (h*(v)), r*(1-y)*cu));
	normals->push_back(osg::Vec3((r*(1 - y)*su), (h*(v)), r*(1 - y)*cu));
	texcoords->push_back(osg::Vec2(1 - x, y));
}

//kup
osg::ref_ptr<osg::Geometry> kopol() {
	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;


	const int N = 10;
	float delta = 1. / N;//mekkopra lépéseket teszünk
	for (int i = 0; i<N; ++i) {
		for (int j = 0; j<N; ++j) {
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			kupP(x, y, vertices, normals, texcoords);//egy négyzethez6 hatszor vertex körüljárás
			kupP(x + delta, y, vertices, normals, texcoords);
			kupP(x, y + delta, vertices, normals, texcoords);

			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			kupP(x + delta, y, vertices, normals, texcoords);
			kupP(x + delta, y + delta, vertices, normals, texcoords);
			kupP(x, y + delta, vertices, normals, texcoords);
		}
	}

	osg::ref_ptr<osg::Geometry> qua = new osg::Geometry;
	qua->setUseVertexBufferObjects(true);

	// állítsuk be, hogy a VBO-ba milyen adatok kerüljenek
	qua->setVertexArray(vertices.get());
	qua->setNormalArray(normals.get());
	qua->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	qua->setTexCoordArray(0, texcoords.get());
	// kirajzolandó primitív meghatározása
	qua->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 6 * N*N));
	return qua;
}


//körlap
osg::ref_ptr<osg::Geometry> kor() {
osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array;

v->push_back(osg::Vec3(0, 0, 0));
for (int i = 0;i <= 16;++i) {
float alpha = 2 * osg::PI*i / 16;
v->push_back(osg::Vec3(cos(alpha), 0, sin(alpha)));
}

for (int i = 0;i < 18;++i) {
n->push_back(osg::Vec3(0, 1, 0));
}

t->push_back(osg::Vec2(0.5, 0.5));
for (int i = 0; i <= 16;++i) {
float alpha = 2 * osg::PI*i / 16;
t->push_back(osg::Vec2((cos(alpha)+1)/2, (sin(alpha) + 1) / 2));
}

osg::ref_ptr<osg::Geometry> kor = new osg::Geometry;

kor->setVertexArray(v);
kor->setNormalArray(n);
kor->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
kor->setTexCoordArray(0, t);;
kor->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_FAN, 0, 18));

return kor;

}

int main(int argc, char** argv)
{
	//körlap
	osg::ref_ptr<osg::Geometry> k = kor();

	//körlap
	osg::ref_ptr<osg::Geometry> t = tor();

	//körlap
	osg::ref_ptr<osg::Geometry> q = kopol();

	/*// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;


	const int N = 16;
	float delta = 2. / N;//mekkopra lépéseket teszünk
	for (int i = 0; i<N; ++i) {
		for (int j = 0; j<N; ++j) {
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			torusz(x, y, vertices, normals, texcoords);//egy négyzethez6 hatszor vertex körüljárás
			torusz(x + delta, y, vertices, normals, texcoords);
			torusz(x, y + delta, vertices, normals, texcoords);
			
			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			torusz(x + delta, y, vertices, normals, texcoords);
			torusz(x + delta, y + delta, vertices, normals, texcoords);
			torusz(x, y + delta, vertices, normals, texcoords);
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
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 6 * N*N));*/

	

	// textúra betöltése
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("image/as.tga");
	texture->setImage(image.get());
	texture->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);

	//több rajzolás
	osg::ref_ptr<osg::Geode> toru = new osg::Geode;
	toru->addDrawable(t);

	//kor
	osg::ref_ptr<osg::Geode> alapkor = new osg::Geode;
	alapkor->addDrawable(k);

	//kup
	osg::ref_ptr<osg::Geode> kup = new osg::Geode;
	kup ->addDrawable(q);


	//Hengerek
	osg::ref_ptr<osg::MatrixTransform> trf1 = new osg::MatrixTransform;
	trf1->setMatrix(osg::Matrix::scale(osg::Vec3((3./5.), 1 , (3./5.)))*osg::Matrix::translate(osg::Vec3(10, 10, 10))*osg::Matrix::scale(osg::Vec3(1.4,1.4,1.4)));
	trf1->addChild(toru);

	osg::ref_ptr<osg::MatrixTransform> trf2 = new osg::MatrixTransform;
	trf2->setMatrix(osg::Matrix::scale(osg::Vec3(1, (5./3.), 1))*osg::Matrix::translate(osg::Vec3(10, 10, 10))*osg::Matrix::scale(osg::Vec3(1.4, 1.4, 1.4)));
	trf2->addChild(kup);
	///trf3 ez a köré -1 miatt lefelenéz
	osg::ref_ptr<osg::MatrixTransform> trf3 = new osg::MatrixTransform;
	trf3->setMatrix(osg::Matrix::scale(osg::Vec3(1, -1, 1))*osg::Matrix::scale(osg::Vec3(3, 1, 3))*osg::Matrix::translate(osg::Vec3(10, 10, 10))*osg::Matrix::scale(osg::Vec3(1.4, 1.4, 1.4)));
	trf3->addChild(alapkor);


	//Hengerek
	osg::ref_ptr<osg::MatrixTransform> trf21 = new osg::MatrixTransform;
	trf21->setMatrix(osg::Matrix::scale(osg::Vec3((3. / 5.), 1, (3. / 5.)))*osg::Matrix::translate(osg::Vec3(-10, -10, -10))*osg::Matrix::scale(osg::Vec3(0.75, 0.75, 0.75)));
	trf21->addChild(toru);

	osg::ref_ptr<osg::MatrixTransform> trf22 = new osg::MatrixTransform;
	trf22->setMatrix(osg::Matrix::scale(osg::Vec3(1, (5. / 3.), 1))*osg::Matrix::translate(osg::Vec3(-10, -10, -10))*osg::Matrix::scale(osg::Vec3(0.75, 0.75, 0.75)));
	trf22->addChild(kup);
	///trf3 ez a köré -1 miatt lefelenéz
	osg::ref_ptr<osg::MatrixTransform> trf23 = new osg::MatrixTransform;
	trf23->setMatrix(osg::Matrix::scale(osg::Vec3(1, -1, 1))*osg::Matrix::scale(osg::Vec3(3, 1, 3))*osg::Matrix::translate(osg::Vec3(-10, -10, -10))*osg::Matrix::scale(osg::Vec3(0.75, 0.75, 0.75)));
	trf23->addChild(alapkor);

	// rakjuk be egy geode-ba a quad-ot, mint kirajzolandó elemet!
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(trf1);
	root->addChild(trf2);
	root->addChild(trf3);//ezt még implementálni kell
	root->addChild(trf21);
	root->addChild(trf22);
	root->addChild(trf23);

   

	// 0-ás mintavételezőre rakjuk rá a textúrát
	root->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	osg::StateSet* state = root->getOrCreateStateSet();
	// kikapcsoljuk a megvilágítást
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::ON |
		osg::StateAttribute::PROTECTED);
	// bekapcsoljuk a hátrafelé néző lapok eldobását
	state->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	state->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

	
	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendő adatnak
	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());

	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
	viewer.setUpViewInWindow(20, 20, 640, 480);
	viewer.realize();

	osg::ref_ptr<osg::MatrixTransform> a = new osg::MatrixTransform;
	//a->setMatrix(osg::Matrix::rotate(osg::PI / 2.0, osg::X_AXIS));
	a->setMatrix(osg::Matrix::translate(osg::Vec3(-10, -10, -10)));
	a->addChild(trf1);
	a->addChild(trf2);
	a->addChild(trf3);//ezt még implementálni kell

	osg::ref_ptr<osg::MatrixTransform> a2 = new osg::MatrixTransform;
	//a2->setMatrix(osg::Matrix::rotate(osg::PI / 2.0, osg::X_AXIS));//majd zhn kell
	a2->setMatrix(osg::Matrix::translate(osg::Vec3(10, 10, 10)));
	a2->addChild(trf21);
	a2->addChild(trf22);
	a2->addChild(trf23);//ezt még implementálni kell

	// írjuk ki egy obj fájlba a parametrikus felületünket!
	osgDB::writeNodeFile(*(a.get()), "a.obj");
	osgDB::writeNodeFile(*(a2.get()), "b.obj");

	// adjuk át a vezérlést a viewer-nek
	return viewer.run();
}
