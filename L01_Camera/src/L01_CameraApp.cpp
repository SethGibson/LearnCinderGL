/*
Lesson 01: In Which We Learn To Setup A Camera
Topics Covered:
	Perspective Camera
	Camera Controls
	Simple shape drawing
*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"				// Include this for camera support
#include "cinder/CameraUi.h"			// Include this for camera controls
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

	/*
		Think of Cinder's CameraPersp as being similar to any other camera
		object from any other 3d context/application. Setting up intrinsics
		and extrinisics is pretty simple, we just use the following two
		functions:
	*/
	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);

	// and now we bind our camera to the controller object
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

	/*
		To draw things in OpenGL, we have to know how to transform
		positional data (vertices, normals, etc) from object coordinates
		to world coordinates to screen space coordinates, so we use
		setMatrices to tell Cinder/OpenGL where to get that transform stack from,
		in this case our CameraPersp object.
	*/
	gl::setMatrices(mCamera);

	gl::enableDepthRead();			// try commenting this out to see the effect!

	vec3 cubeCenter = vec3(0);
	vec3 cubeSize = vec3(0.5);

	// and now we draw an object just so we don't have a black screen
	gl::drawColorCube(cubeCenter, cubeSize);
}


/* EXERCISES:
Try changing the camera intrinsics (FOV, aspect, etc) to see the effect on drawing

Try drawing a different object
	Hint: use gl::color to set a drawing color, then use one of the gl::draw functions
	to draw a new shape (e.g. gl::drawSphere(), gl::drawSolidRect(), use intellisense!)

Try drawing two objects in different spots

*/


CINDER_APP( L01_CameraApp, RendererGl )
