/*
Lesson 05: In Which We Learn about cubemaps, environment mapping, "advanced" params, and different way to use shaders
Topics Covered:
Loading and working with cubemaps/textures
Reflection and refraction mapping
Direct shader binding
Enumerated parameters
*/

#ifndef _L05_H_
#define _L05_H_
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

class L05_Lighting_III : public App
{
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	void setupCamera();
	void setupScene();
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
	vec3			mSpherePosition;

	gl::BatchRef	mTorusBatch;
	gl::GlslProgRef	mTorusShader;
	vec3			mTorusPosition;

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
#endif