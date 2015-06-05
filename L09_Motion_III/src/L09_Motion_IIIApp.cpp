/*
Lesson 09: In Which We Learn about more things we can do with per-instance attributes, and how to simplify per-instance
attribute management using interleaved data

Topics Covered:
	Interleaving attribute data using structs
*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L09_Motion_III : public App
{
public:
	struct Particle
	{
		vec3	Position;
		int		Age,
				Life,
				Index;

		float	Size;

		Color	Emissive;

		bool	IsShining;

		Particle(vec3 pPos, int pId) : Position(pPos), Index(pId),
								Life(60),Age(60),
								Size(1.0f),
								Emissive(Color::black()),
								IsShining(false)
		{}

		void Update(float frames, float step, bool zaxis)
		{
			float x = math<float>::cos(Index*step);
			float y = math<float>::sin(Index*step);

			float t = (float)frames*0.1f;
			float offset = math<float>::sin((Index*step*8.f) + t)*0.1f;
			float size = math<float>::sin((Index*step*2.f) + t)*0.1f;

			if (zaxis)
				Position = vec3(x + x*offset, y + y*offset, 0.0);
			else
				Position = vec3(x + x*offset, 0.0, y + y*offset);
			Size = math<float>::max(size*10.0f, 0.5f);

			if ( (offset >= 0.099f) && (IsShining==false))
				IsShining = true;

			if (IsShining)
			{
				Age--;
				if (Age > 0)
				{
					float norm = Age / (float)Life;
					Emissive = Color::white()*norm;
				}
				else if (Age<=0)
				{
					Age = Life;
					IsShining = false;
					Emissive = Color::black();
				}
			}
		}
	};

	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	void setupCamera();
	void setupSpheres(int numElements);
	void setupTorii(int numElements);
	void setupGUI();
	void setupSkybox();

	void updateSpheres();
	void updateTorii();

	void drawScene();
	void drawSkybox();

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	gl::BatchRef	mPlaneBatch;
	gl::GlslProgRef	mPlaneShader;

	gl::BatchRef		mSphereBatch;
	gl::GlslProgRef		mSphereShader;
	gl::VboRef			mSphereInstanceData;
	gl::VboMeshRef		mSphereMeshData;
	int					mNumSpheres;
	vector<Particle>	mSpheres;

	gl::BatchRef		mTorusBatch;
	gl::GlslProgRef		mTorusShader;
	gl::VboRef			mTorusInstanceData;
	gl::VboMeshRef		mTorusMeshData;
	int					mNumTorii;
	vector<Particle>	mTorii;

	string			mUniformLightPos = "uLightPos";
	string			mUniformEyePos = "uCameraPos";
	string			mUniformSpecularPower = "uSpecularPower";
	string			mUniformSpecularStrength = "uSpecularStrength";
	string			mUniformAmbientStrength = "uAmbientStrength";
	string			mUniformEnvStrength = "uEnvStrength";
	string			mUniformColor = "uColor";
	string			mUniformRefract = "uRefract";

	gl::BatchRef			mSkyboxBatch;
	gl::GlslProgRef			mSkyboxShader;
	gl::TextureCubeMapRef	mSkyboxCubemap;

	params::InterfaceGlRef	mGUI;
	float					mParamLightPosX,
		mParamLightPosY,
		mParamLightPosZ;

	float					mParamSphereSpecularPower,
							mParamSphereSpecularStrength,
							mParamSphereAmbientStrength,
							mParamSphereEnvStrength;

	vector<float>			mParamTorusRefractIndices;
	vector<string>			mParamTorusRefractNames;
	int						mParamTorusRefractId;

	float					mParamTorusSpecularPower,
							mParamTorusSpecularStrength,
							mParamTorusAmbientStrength,
							mParamTorusEnvStrength;

	Color					mParamSphereColor,
							mParamTorusColor;
};

void L09_Motion_III::setup()
{
	mNumSpheres = 32;
	mNumTorii = 32;
	setupGUI();
	setupCamera();
	setupSkybox();
	setupSpheres(mNumSpheres);
	setupTorii(mNumTorii);
}

void L09_Motion_III::setupCamera()
{
	float verticalFOV = 45.0f;
	float aspectRatio = getWindowAspectRatio();
	float nearClip = 0.1f;
	float farClip = 10.0f;
	vec3 cameraPosition = vec3(0, 0.5f, -1.5);
	vec3 cameraTarget = vec3(0);
	vec3 cameraUpAxis = vec3(0, 1, 0);

	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
	mCamera.setPivotDistance(length(cameraPosition));
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void L09_Motion_III::setupSpheres(int numElements)
{
	float sphereRadius = 0.075f;
	int sphereResolution = 32;
	string sphereVertShader = "shaders/shape_vert.glsl";
	string sphereFragShader = "shaders/shape_reflect_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));
	mSphereMeshData = gl::VboMesh::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution));

	float step = (M_PI*2.0) / numElements;
	for (int i = 0; i < numElements; ++i)
	{
		float x = math<float>::cos(i*step);
		float y = math<float>::sin(i*step);
		mSpheres.push_back(Particle(vec3(x, y, 0.0),i));
	}

	geom::BufferLayout attributes;
	mSphereInstanceData = gl::Vbo::create(GL_ARRAY_BUFFER, mSpheres, GL_DYNAMIC_DRAW);
	attributes.append(geom::CUSTOM_0, 3, sizeof(Particle), offsetof(Particle, Position), 1);
	attributes.append(geom::CUSTOM_1, 3, sizeof(Particle), offsetof(Particle, Emissive), 1);
	attributes.append(geom::CUSTOM_2, 1, sizeof(Particle), offsetof(Particle, Size), 1);

	mSphereMeshData->appendVbo(attributes, mSphereInstanceData);
	mSphereBatch = gl::Batch::create(mSphereMeshData, mSphereShader, { { geom::CUSTOM_0, "iPosition" }, { geom::CUSTOM_1, "iColor" }, { geom::CUSTOM_2, "iSize" } });
	mSphereBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L09_Motion_III::setupTorii(int numElements)
{
	float torusOuterRadius = 0.1f;
	float torusInnerRadius = 0.075f;
	int torusAxisResolution = 32;
	int torusSegmentResolution = 16;
	string torusVertShader = "shaders/shape_vert.glsl";
	string torusFragShader = "shaders/shape_refract_frag.glsl";
	mTorusShader = gl::GlslProg::create(loadAsset(torusVertShader), loadAsset(torusFragShader));
	mTorusMeshData = gl::VboMesh::create(geom::Torus().radius(torusOuterRadius, torusInnerRadius).subdivisionsHeight(torusSegmentResolution).subdivisionsAxis(torusAxisResolution));

	float step = (M_PI*2.0) / numElements;
	for (int i = 0; i < numElements; ++i)
	{
		float x = math<float>::cos(i*step);
		float z = math<float>::sin(i*step);
		mTorii.push_back(Particle(vec3(x, 0.0, z), i));
	}

	geom::BufferLayout attributes;
	mTorusInstanceData = gl::Vbo::create(GL_ARRAY_BUFFER, mTorii, GL_DYNAMIC_DRAW);
	attributes.append(geom::CUSTOM_0, 3, sizeof(Particle), offsetof(Particle, Position), 1);
	attributes.append(geom::CUSTOM_1, 3, sizeof(Particle), offsetof(Particle, Emissive), 1);
	attributes.append(geom::CUSTOM_2, 1, sizeof(Particle), offsetof(Particle, Size), 1);

	mTorusMeshData->appendVbo(attributes, mTorusInstanceData);
	mTorusBatch = gl::Batch::create(mTorusMeshData, mTorusShader, { { geom::CUSTOM_0, "iPosition" },{ geom::CUSTOM_1, "iColor" }, { geom::CUSTOM_2, "iSize" } });
	mTorusBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L09_Motion_III::setupSkybox()
{
	mSkyboxCubemap = gl::TextureCubeMap::create(loadImage(loadAsset("textures/skybox.png")));
	mSkyboxShader = gl::GlslProg::create(loadAsset("shaders/skybox_vert.glsl"), loadAsset("shaders/skybox_frag.glsl"));
	mSkyboxShader->uniform("uCubemapSampler", 0);
}

void L09_Motion_III::setupGUI()
{
	mParamLightPosX = 0.0f;
	mParamLightPosY = 1.0f;
	mParamLightPosZ = -1.0f;

	mParamSphereSpecularPower = 16.0f;
	mParamSphereSpecularStrength = 1.0f;
	mParamSphereAmbientStrength = 1.0f;
	mParamSphereEnvStrength = 1.0f;

	mParamTorusSpecularPower = 16.0f;
	mParamTorusSpecularStrength = 1.0f;
	mParamTorusAmbientStrength = 1.0f;
	mParamTorusEnvStrength = 1.0f;
	mParamTorusRefractId = 0;
	mParamTorusRefractIndices = vector<float>{1.0f, 1.33f, 1.309f, 1.52f, 2.42f};
	mParamTorusRefractNames = vector<string>{"Air", "Water", "Ice", "Glass", "Diamond"};
	mParamSphereColor = Color(0.1f, 0.25f, 0.5f);
	mParamTorusColor = Color(0.1f, 0.5f, 0.25f);

	mGUI = params::InterfaceGl::create("Params", ivec2(300, 350));
	mGUI->addParam<float>("mParamLightPosX", &mParamLightPosX).optionsStr("label='light x'");
	mGUI->addParam<float>("mParamLightPosY", &mParamLightPosY).optionsStr("label='light y'");
	mGUI->addParam<float>("mParamLightPosZ", &mParamLightPosZ).optionsStr("label='light z'");

	mGUI->addSeparator("");
	mGUI->addText("Sphere");
	mGUI->addSeparator("");
	mGUI->addParam<Color>("mParamSphereColor", &mParamSphereColor).optionsStr("label='color'");

	mGUI->addSeparator("");
	mGUI->addText("Torus");
	mGUI->addSeparator("");
	mGUI->addParam<Color>("mParamTorusColor", &mParamTorusColor).optionsStr("label='color'");
	mGUI->addParam("mParamTorusRefract", mParamTorusRefractNames, &mParamTorusRefractId).optionsStr("label='refraction'");
}

void L09_Motion_III::mouseDown(MouseEvent event)
{
}

void L09_Motion_III::update()
{
	updateSpheres();
	updateTorii();
}

void L09_Motion_III::updateSpheres()
{
	float frames = getElapsedFrames();
	float step = (M_PI*2.0) / mNumSpheres;

	for (auto s = begin(mSpheres); s != end(mSpheres);++s)
	{
		s->Update(frames, step, true);
	}

	mSphereInstanceData->bufferData(mSpheres.size()*sizeof(Particle), mSpheres.data(), GL_DYNAMIC_DRAW);
}

void L09_Motion_III::updateTorii()
{
	float frames = getElapsedFrames();
	float step = (M_PI*2.0) / mNumTorii;

	for (auto s = begin(mTorii); s != end(mTorii); ++s)
	{
		s->Update(frames, step, false);
	}

	mTorusInstanceData->bufferData(mTorii.size()*sizeof(Particle), mTorii.data(), GL_DYNAMIC_DRAW);

}

void L09_Motion_III::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);

	drawSkybox();
	drawScene();

	gl::setMatricesWindow(getWindowSize());
	mGUI->draw();
}

void L09_Motion_III::drawScene()
{
	gl::enableDepthRead();
	mSkyboxCubemap->bind(0);
	mSphereBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mSphereBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mSphereBatch->getGlslProg()->uniform(mUniformColor, mParamSphereColor);

	gl::pushMatrices();
	gl::rotate(toRadians((float)getElapsedFrames()));
	mSphereBatch->drawInstanced(mNumSpheres);
	gl::popMatrices();

	mTorusBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mTorusBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mTorusBatch->getGlslProg()->uniform(mUniformColor, mParamTorusColor);
	mTorusBatch->getGlslProg()->uniform(mUniformRefract, mParamTorusRefractIndices[mParamTorusRefractId]);

	gl::pushMatrices();
	gl::rotate(toRadians((float)getElapsedFrames()), vec3(0, 1, 0));
	mTorusBatch->drawInstanced(mNumTorii);
	gl::popMatrices();

	mSkyboxCubemap->unbind(0);
}

void L09_Motion_III::drawSkybox()
{
	gl::disableDepthRead();
	mSkyboxShader->bind();
	mSkyboxCubemap->bind(0);
	gl::drawCube(vec3(0), vec3(1));
	mSkyboxCubemap->unbind();
}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(1280, 720);
	pSettings->setFrameRate(60);
}

CINDER_APP(L09_Motion_III, RendererGl, prepareSettings)
