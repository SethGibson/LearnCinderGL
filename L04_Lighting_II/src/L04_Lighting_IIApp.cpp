/*
Lesson 04: In Which We Learn more lighting and how to use Cinder's debug UI
Topics Covered:
	Lighting math in shaders
	Cinder's Params window
	Changing application settings
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

class L04_Lighting_II : public App
{
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:

	void setupCamera();
	void setupBatches();
	void setupGUI();

	void drawSphere();

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	gl::BatchRef	mPlaneBatch;
	gl::GlslProgRef	mPlaneShader;

	gl::BatchRef	mSphereBatch;
	gl::GlslProgRef	mSphereShader;
	string			mUniformLightPos =			"uLightPos";
	string			mUniformEyePos =			"uCameraPos";
	string			mUniformSpecularPower =		"uSpecularPower";
	string			mUniformSpecularStrength =	"uSpecularStrength";
	string			mUniformAmbientStrength =	"uAmbientStrength";
	string			mUniformColor =				"uColor";

	params::InterfaceGlRef	mGUI;
	float					mParamLightPosX,
							mParamLightPosY,
							mParamLightPosZ,
							mParamSpecularPower,
							mParamSpecularStrength,
							mParamAmbientStrength;

	Color					mParamColor;
};

void L04_Lighting_II::setup()
{
	setupGUI();
	setupCamera();
	setupBatches();
}

void L04_Lighting_II::setupCamera()
{
	float verticalFOV = 45.0f;
	float aspectRatio = getWindowAspectRatio();
	float nearClip = 0.1f;
	float farClip = 10.0f;
	vec3 cameraPosition = vec3(0, 0.1, -1);
	vec3 cameraTarget = vec3(0);
	vec3 cameraUpAxis = vec3(0, 1, 0);

	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void L04_Lighting_II::setupBatches()
{
	//setup plane
	string planeVertShader = "shaders/plane_vert.glsl";
	string planeFragShader = "shaders/plane_frag.glsl";
	mPlaneShader = gl::GlslProg::create(loadAsset(planeVertShader), loadAsset(planeFragShader));

	vec3 planeNormal(0, 1, 0);
	vec2 planeSize(2.0f);
	mPlaneBatch = gl::Batch::create(geom::Plane().normal(planeNormal).size(planeSize), mPlaneShader);

	//setup sphere
	string sphereVertShader = "shaders/sphere_vert.glsl";
	string sphereFragShader = "shaders/sphere_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));

	float sphereRadius = 0.1f;
	int sphereResolution = 32;
	mSphereBatch = gl::Batch::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution), mSphereShader);
}

void L04_Lighting_II::setupGUI()
{
	mParamLightPosX = 0.0f;
	mParamLightPosY = 0.0f;
	mParamLightPosZ = 0.0f;
	mParamSpecularPower = 16.0f;
	mParamSpecularStrength = 2.0f;
	mParamAmbientStrength = 0.25f;
	mParamColor = Color(0.5f, 0.1f, 0.25f);

	mGUI = params::InterfaceGl::create("Params", ivec2(300, 300));
	mGUI->addParam<float>("mParamLightPosX", &mParamLightPosX).optionsStr("label='light x'");
	mGUI->addParam<float>("mParamLightPosY", &mParamLightPosY).optionsStr("label='light y'");
	mGUI->addParam<float>("mParamLightPosZ", &mParamLightPosZ).optionsStr("label='light z'");
	mGUI->addParam<float>("mParamSpecularPower", &mParamSpecularPower).optionsStr("label='specular power'");
	mGUI->addParam<float>("mParamSpecularStrength", &mParamSpecularStrength).optionsStr("label='specular strength'");
	mGUI->addParam<float>("mParamAmbientStrength)", &mParamAmbientStrength).optionsStr("label='ambient strength'");
	mGUI->addParam<Color>("mParamColor", &mParamColor).optionsStr("label='color'");
}

void L04_Lighting_II::mouseDown(MouseEvent event)
{
}

void L04_Lighting_II::update()
{
}

void L04_Lighting_II::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);
	gl::enableDepthRead();

	mPlaneBatch->draw();
	
	drawSphere();

	gl::setMatricesWindow(getWindowSize());
	//gl::disableDepthRead();
	mGUI->draw();
}

void L04_Lighting_II::drawSphere()
{

	gl::pushMatrices();
	gl::translate(vec3(0, 0.1, 0));

	mSphereBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mSphereBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mSphereBatch->getGlslProg()->uniform(mUniformSpecularPower, mParamSpecularPower);
	mSphereBatch->getGlslProg()->uniform(mUniformSpecularStrength, mParamSpecularStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformAmbientStrength, mParamAmbientStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformColor, mParamColor);
	mSphereBatch->draw();
	
	gl::popMatrices();

}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(1280, 720);
	pSettings->setFrameRate(60);
}

CINDER_APP(L04_Lighting_II, RendererGl, prepareSettings)

