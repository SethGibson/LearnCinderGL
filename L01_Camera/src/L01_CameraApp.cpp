/*
Lesson 01: In Which We Learn To Setup A Camera
Topics Covered:
	Perspective Camera
	Camera Controls
	Simple shape drawing
*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L01_CameraApp : public App
{
public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;
};

void L01_CameraApp::setup()
{
	float verticalFOV = 45.0f;
	float aspectRatio = getWindowAspectRatio();
	float nearClip = 0.1f;
	float farClip = 10.0f;
	vec3 cameraPosition = vec3(0, 0, -1);
	vec3 cameraTarget = vec3(0);
	vec3 cameraUpAxis = vec3(0, 1, 0);

	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void L01_CameraApp::mouseDown( MouseEvent event )
{
}

void L01_CameraApp::update()
{
}

void L01_CameraApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::setMatrices(mCamera);
	gl::enableDepthRead();

	vec3 cubeCenter = vec3(0);
	vec3 cubeSize = vec3(0.5);
	gl::drawColorCube(cubeCenter, cubeSize);
}

CINDER_APP( L01_CameraApp, RendererGl )
