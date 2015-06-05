/*
Lesson 08: In Which We Learn about animation using per-instance attributes
Topics Covered:
Animating Per-Instance Attributes
Changing vertex data buffer contents
*/

#ifndef _L08_H_
#define _L08_H_
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

class L08_Motion_II : public App
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

	gl::BatchRef	mSphereBatch;
	gl::GlslProgRef	mSphereShader;
	gl::VboRef		mSphereInstanceData;
	gl::VboMeshRef	mSphereMeshData;
	int				mNumSpheres;
	vector<vec3>	mSpherePositions;

	gl::BatchRef	mTorusBatch;
	gl::GlslProgRef	mTorusShader;
	gl::VboRef		mTorusInstanceData;
	gl::VboMeshRef	mTorusMeshData;
	int				mNumTorii;
	vector<vec3>	mTorusPositions;

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