/*
Lesson 06: In Which We Learn about instancing, i.e. defining a model once and drawing it multiple times with different attributes
Topics Covered:
Instancing
Cinder Vertex Buffer Objects
Cinder Mesh Objects
Cinder Attribute Mapping
*/
#ifndef _L06_H_
#define _L06_H_
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

	const string mUniformLightPos = "uLightPos";
	const string mUniformEyePos = "uCameraPos";
	const string mUniformSpecularPower = "uSpecularPower";
	const string mUniformSpecularStrength = "uSpecularStrength";
	const string mUniformAmbientStrength = "uAmbientStrength";
	const string mUniformEnvStrength = "uEnvStrength";
	const string mUniformColor = "uColor";
	const string mUniformRefract = "uRefract";

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