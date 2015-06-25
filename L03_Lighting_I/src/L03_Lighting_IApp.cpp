/*
Lesson 03: In Which We Learn basic lighting with shaders
Topics Covered:
	Simple diffuse lighting with GLSL
	Shaders!

	"Lighting" is a bit of a misnomer as this lesson serves as our introduction to shaders/GLSL.
*/

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L03_Lighting_I : public App
{
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	void setupCamera();
	void setupBatches();

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	gl::BatchRef	mPlaneBatch;
	gl::GlslProgRef	mPlaneShader;

	gl::BatchRef	mSphereBatch;
	gl::GlslProgRef	mSphereShader;
};

void L03_Lighting_I::setup()
{
	setupCamera();
	setupBatches();
}

void L03_Lighting_I::setupCamera()
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

void L03_Lighting_I::setupBatches()
{
	vec3 planeNormal(0, 1, 0);
	vec2 planeSize(2.0f);
	string planeVertShader = "shaders/plane_vert.glsl";
	string planeFragShader = "shaders/plane_frag.glsl";
	mPlaneShader = gl::GlslProg::create(loadAsset(planeVertShader), loadAsset(planeFragShader));
	mPlaneBatch = gl::Batch::create(geom::Plane().normal(planeNormal).size(planeSize), mPlaneShader);

	float sphereRadius = 0.1f;
	int sphereResolution = 16;
	string sphereVertShader = "shaders/sphere_vert.glsl";
	string sphereFragShader = "shaders/sphere_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));
	mSphereBatch = gl::Batch::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution), mSphereShader);
}

void L03_Lighting_I::mouseDown(MouseEvent event)
{
}

void L03_Lighting_I::update()
{
}

void L03_Lighting_I::draw()
{
	/* Working With Shaders
		So you'll notice that other than moving the batch setup into its own method
		the code in the cpp file is practically identical to the previous lesson...
		What's up with that? Well, once we want to start drawing objects, all the magic is
		performed by the shaders, so we'll be working in the .glsl files for this lesson...
	*/
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);
	gl::enableDepthRead();

	mPlaneBatch->draw();

	gl::pushModelMatrix();
	gl::translate(vec3(0, 0.1, 0));
	mSphereBatch->draw();
	gl::popModelMatrix();

	/* EXERCISES
		Change the color of the of the sphere

		Right now, we just assume a white light. Implement color for the light
			HINT: define another uniform for color, then modulate the surface color
			of the sphere by the new color and the diffuse term in the shader

		Copy sphere_vert and sphere_frag, add a new object to the scene with those shaders, and
		color them something else (we'll explore a simpler way to do this in a future lesson, but try this for now)
			HINT: you'll need a new gl::BatchRef and gl::GlslProgRef
	*/
}

CINDER_APP(L03_Lighting_I, RendererGl)

