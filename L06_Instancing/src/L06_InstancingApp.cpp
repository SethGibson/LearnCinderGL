/*
Lesson 06: In Which We Learn about instancing, i.e. defining a model once and drawing it multiple times with different attributes
Topics Covered:
	Instancing
	Cinder Vertex Buffer Objects
	Cinder Mesh Objects
	Cinder Attribute Mapping
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

class L06_Instancing : public App
{
public:
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

	void drawScene();
	void drawSkybox();

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	gl::BatchRef	mPlaneBatch;
	gl::GlslProgRef	mPlaneShader;

	gl::BatchRef	mSphereBatch;
	gl::GlslProgRef	mSphereShader;
	gl::VboRef		mSphereInstanceData;
	gl::VboMeshRef	mSphereMeshData;
	int				mNumSpheres;

	gl::BatchRef	mTorusBatch;
	gl::GlslProgRef	mTorusShader;
	gl::VboRef		mTorusInstanceData;
	gl::VboMeshRef	mTorusMeshData;
	int				mNumTorii;

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

void L06_Instancing::setup()
{
	mNumSpheres = 32;
	mNumTorii = 32;
	setupGUI();
	setupCamera();
	setupSkybox();
	setupSpheres(mNumSpheres);
	setupTorii(mNumTorii);
}

void L06_Instancing::setupCamera()
{
	float verticalFOV = 45.0f;
	float aspectRatio = getWindowAspectRatio();
	float nearClip = 0.1f;
	float farClip = 10.0f;
	vec3 cameraPosition = vec3(0, 0.1, -2);
	vec3 cameraTarget = vec3(0);
	vec3 cameraUpAxis = vec3(0, 1, 0);

	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
	mCamera.setPivotDistance(length(cameraPosition));
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void L06_Instancing::setupSpheres(int numElements)
{
	float sphereRadius = 0.1f;
	int sphereResolution = 32;
	string sphereVertShader = "shaders/shape_vert.glsl";
	string sphereFragShader = "shaders/shape_reflect_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));
	mSphereMeshData = gl::VboMesh::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution));
	
	float step = (M_PI*2.0) / numElements;
	vector<vec3> spherePositions;
	for (int i = 0; i < numElements; ++i)
	{
		float x = math<float>::cos(i*step);
		float y = math<float>::sin(i*step);
		spherePositions.push_back(vec3(x, y, 0.0));
	}

	geom::BufferLayout attributes;
	mSphereInstanceData = gl::Vbo::create(GL_ARRAY_BUFFER, spherePositions, GL_STATIC_DRAW);
	attributes.append(geom::CUSTOM_0, 3, sizeof(vec3), 0, 1);
	
	mSphereMeshData->appendVbo(attributes, mSphereInstanceData);
	mSphereBatch = gl::Batch::create(mSphereMeshData, mSphereShader, { {geom::CUSTOM_0, "iPosition"} });
	mSphereBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L06_Instancing::setupTorii(int numElements)
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
	vector<vec3> torusPositions;
	for (int i = 0; i < numElements; ++i)
	{
		float x = math<float>::cos(i*step);
		float z = math<float>::sin(i*step);
		torusPositions.push_back(vec3(x, 0.0, z));
	}

	geom::BufferLayout attributes;
	mTorusInstanceData = gl::Vbo::create(GL_ARRAY_BUFFER, torusPositions, GL_STATIC_DRAW);
	attributes.append(geom::CUSTOM_0, 3, sizeof(vec3), 0, 1);

	mTorusMeshData->appendVbo(attributes, mTorusInstanceData);
	mTorusBatch = gl::Batch::create(mTorusMeshData, mTorusShader, { { geom::CUSTOM_0, "iPosition" } });
	mTorusBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L06_Instancing::setupSkybox()
{
	mSkyboxCubemap = gl::TextureCubeMap::create(loadImage(loadAsset("textures/skybox.png")));
	mSkyboxShader = gl::GlslProg::create(loadAsset("shaders/skybox_vert.glsl"), loadAsset("shaders/skybox_frag.glsl"));
	mSkyboxShader->uniform("uCubemapSampler", 0);
}

void L06_Instancing::setupGUI()
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
	mGUI->addParam<float>("mParamSphereSpecularPower", &mParamSphereSpecularPower).optionsStr("label='specular power'");
	mGUI->addParam<float>("mParamSphereSpecularStrength", &mParamSphereSpecularStrength).optionsStr("label='specular strength'");
	mGUI->addParam<float>("mParamSphereAmbientStrength)", &mParamSphereAmbientStrength).optionsStr("label='ambient strength'");
	mGUI->addParam<float>("mParamSphereEnvStrength)", &mParamSphereEnvStrength).optionsStr("label='environment strength'");
	mGUI->addParam<Color>("mParamSphereColor", &mParamSphereColor).optionsStr("label='color'");

	mGUI->addSeparator("");
	mGUI->addText("Torus");
	mGUI->addSeparator("");
	mGUI->addParam<float>("mParamTorusSpecularPower", &mParamTorusSpecularPower).optionsStr("label='specular power'");
	mGUI->addParam<float>("mParamTorusSpecularStrength", &mParamTorusSpecularStrength).optionsStr("label='specular strength'");
	mGUI->addParam<float>("mParamTorusAmbientStrength)", &mParamTorusAmbientStrength).optionsStr("label='ambient strength'");
	mGUI->addParam<float>("mParamTorusEnvStrength)", &mParamTorusEnvStrength).optionsStr("label='environment strength'");
	mGUI->addParam<Color>("mParamTorusColor", &mParamTorusColor).optionsStr("label='color'");
	mGUI->addParam("mParamTorusRefract", mParamTorusRefractNames, &mParamTorusRefractId).optionsStr("label='refraction'");
}

void L06_Instancing::mouseDown(MouseEvent event)
{
}

void L06_Instancing::update()
{
}

void L06_Instancing::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);

	drawSkybox();
	drawScene();

	gl::setMatricesWindow(getWindowSize());
	mGUI->draw();
}

void L06_Instancing::drawScene()
{
	gl::enableDepthRead();
	mSkyboxCubemap->bind(0);
	mSphereBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mSphereBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mSphereBatch->getGlslProg()->uniform(mUniformSpecularPower, mParamSphereSpecularPower);
	mSphereBatch->getGlslProg()->uniform(mUniformSpecularStrength, mParamSphereSpecularStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformAmbientStrength, mParamSphereAmbientStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformEnvStrength, mParamSphereEnvStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformColor, mParamSphereColor);
	mSphereBatch->drawInstanced(mNumSpheres);

	mTorusBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mTorusBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mTorusBatch->getGlslProg()->uniform(mUniformSpecularPower, mParamTorusSpecularPower);
	mTorusBatch->getGlslProg()->uniform(mUniformSpecularStrength, mParamTorusSpecularStrength);
	mTorusBatch->getGlslProg()->uniform(mUniformAmbientStrength, mParamTorusAmbientStrength);
	mTorusBatch->getGlslProg()->uniform(mUniformEnvStrength, mParamTorusEnvStrength);
	mTorusBatch->getGlslProg()->uniform(mUniformColor, mParamTorusColor);
	mTorusBatch->getGlslProg()->uniform(mUniformRefract, mParamTorusRefractIndices[mParamTorusRefractId]);
	mTorusBatch->drawInstanced(mNumTorii);
	mSkyboxCubemap->unbind(0);
}

void L06_Instancing::drawSkybox()
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

CINDER_APP(L06_Instancing, RendererGl, prepareSettings)
