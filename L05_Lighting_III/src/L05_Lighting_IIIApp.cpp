/*
Lesson 05: In Which We Learn about cubemaps, environment mapping, "advanced" params, and different way to use shaders
Topics Covered:
	Loading and working with cubemaps/textures
	Reflection and refraction mapping
	Direct shader binding
	Enumerated parameters
*/

#include "L05_Lighting_IIIApp.h"
void L05_Lighting_III::setup()
{
	setupGUI();
	setupCamera();
	setupSkybox();
	setupScene();
}

void L05_Lighting_III::setupCamera()
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
	mCamera.setPivotDistance(length(cameraPosition));
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void L05_Lighting_III::setupScene()
{
	//setup sphere
	float sphereRadius = 0.2f;
	int sphereResolution = 32;
	mSpherePosition = vec3(-0.25, 0.1, 0);
	string sphereVertShader = "shaders/shape_vert.glsl";
	string sphereFragShader = "shaders/shape_reflect_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));
	mSphereBatch = gl::Batch::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution), mSphereShader);
	mSphereBatch->getGlslProg()->uniform("uCubemapSampler", 0);

	//setup torus
	float torusOuterRadius = 0.2f;
	float torusInnderRadius = 0.15f;
	int torusAxisResolution = 32;
	int torusSegmentResolution = 16;
	mTorusPosition = vec3(0.25, 0.1, 0);
	string torusVertShader = "shaders/shape_vert.glsl";
	string torusFragShader = "shaders/shape_refract_frag.glsl";
	mTorusShader = gl::GlslProg::create(loadAsset(torusVertShader), loadAsset(torusFragShader));
	mTorusBatch = gl::Batch::create(geom::Torus().radius(torusOuterRadius,torusInnderRadius).subdivisionsAxis(torusAxisResolution).subdivisionsHeight(torusSegmentResolution), mTorusShader);
	mTorusBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L05_Lighting_III::setupSkybox()
{
	mSkyboxCubemap = gl::TextureCubeMap::create(loadImage(loadAsset("textures/skybox.png")));
	mSkyboxShader = gl::GlslProg::create(loadAsset("shaders/skybox_vert.glsl"), loadAsset("shaders/skybox_frag.glsl"));
	mSkyboxShader->uniform("uCubemapSampler", 0);
}

void L05_Lighting_III::setupGUI()
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
	mParamTorusRefractNames = vector<string>{"Air","Water","Ice","Glass","Diamond"};
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

void L05_Lighting_III::mouseDown(MouseEvent event)
{
}

void L05_Lighting_III::update()
{
}

void L05_Lighting_III::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);

	drawSkybox();
	drawScene();

	gl::setMatricesWindow(getWindowSize());
	mGUI->draw();
}

void L05_Lighting_III::drawScene()
{
	gl::enableDepthRead();
	mSkyboxCubemap->bind(0);
	gl::pushModelMatrix();
	gl::translate(mSpherePosition);
	mSphereBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mSphereBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mSphereBatch->getGlslProg()->uniform(mUniformSpecularPower, mParamSphereSpecularPower);
	mSphereBatch->getGlslProg()->uniform(mUniformSpecularStrength, mParamSphereSpecularStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformAmbientStrength, mParamSphereAmbientStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformEnvStrength, mParamSphereEnvStrength);
	mSphereBatch->getGlslProg()->uniform(mUniformColor, mParamSphereColor);
	mSphereBatch->draw();
	gl::popModelMatrix();

	gl::pushModelMatrix();
	gl::translate(mTorusPosition);
	mTorusBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mTorusBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mTorusBatch->getGlslProg()->uniform(mUniformSpecularPower, mParamTorusSpecularPower);
	mTorusBatch->getGlslProg()->uniform(mUniformSpecularStrength, mParamTorusSpecularStrength);
	mTorusBatch->getGlslProg()->uniform(mUniformAmbientStrength, mParamTorusAmbientStrength);
	mTorusBatch->getGlslProg()->uniform(mUniformEnvStrength, mParamTorusEnvStrength);
	mTorusBatch->getGlslProg()->uniform(mUniformColor, mParamTorusColor);
	mTorusBatch->getGlslProg()->uniform(mUniformRefract, mParamTorusRefractIndices[mParamTorusRefractId]);
	mTorusBatch->draw();
	gl::popModelMatrix();
	mSkyboxCubemap->unbind(0);
}

void L05_Lighting_III::drawSkybox()
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

CINDER_APP(L05_Lighting_III, RendererGl, prepareSettings)
