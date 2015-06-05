/*
Lesson 09: In Which We Learn about more things we can do with per-instance attributes, and how to simplify per-instance
attribute management using interleaved data

Topics Covered:
Interleaving attribute data using structs
*/

#ifndef _L09_H_
#define _L09_H_
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/params/Params.h"
#include "Particle.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L09_Motion_III : public App
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
#endif