/* Lesson 02: In Which We Learn to setup shaders and use batches to draw basic shapes.

Topics Covered:
	Drawing simple primitives with BatchRefs
	Writing shaders and using GlslProgRef to apply GLSL shaders to our shapes
	Basic object positioning using glm, the GL Math library
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

class L02_BasicScene : public App
{
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	void setupCamera();

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	/* Declaring Scene Objects:
		Our "scene" is going to be a simple sphere "hovering" over a plane.
		In Cinder, we use "Batch" objects to encapsulate information about
		geometry (shapes, shaders, etc) and later draw the objects.

	*/
	gl::BatchRef	mPlaneBatch;
	gl::BatchRef	mSphereBatch;

	/* Declaring Shaders:
		In Modern OpenGL, nothing gets done without shaders (or "GLSL programs").
		Cinder encapsulates shader information in GlslProg objects.
	*/
	gl::GlslProgRef	mPlaneShader;
	gl::GlslProgRef	mSphereShader;
};

void L02_BasicScene::setup()
{
	// We've moved all our camera setup into its own function
	// If we were to define an app constructor, we could do that
	// there as well.
	setupCamera();

	/* Defining Scene Objects: Shaders
		Alright, let's set up our scene objects. Shader objects are pretty simple,
		we just pass in paths to shaders, in this case, we're using asset folder relative
		paths so we can make use of Cinder's built in loadAsset(). For these examples,
		we're just using vertex and fragment programs, altho Cinder supports geometry shaders,
		tesselation shaders, and compute shaders
	*/
	string planeVertShader = "shaders/plane_vert.glsl";
	string planeFragShader = "shaders/plane_frag.glsl";
	mPlaneShader = gl::GlslProg::create(loadAsset(planeVertShader), loadAsset(planeFragShader));

	/* Defining Scene Objects: Draw Batches
		Now let's set up our draw batches. While there's nothing in OpenGL quite analogous to a batch,
		the concept of batches is all over modern computer graphics. This is the simplest way to
		set up a batch, wherein we pass in a geometry definition using geom::<Something>, in this case geom::Plane to
		define a plane, and then we pass in our shader object as well.  geom::Plane() and the related objects are collectively
		known as geom::Source objects, which you can think of as geometry generators. in fact, if you remember from our
		last lesson where we drew a cube, you could just as well have issued this statement:

		gl::draw(geom::Cube());
	*/
	vec3 planeNormal(0, 1, 0);
	vec2 planeSize(2.0f);
	mPlaneBatch = gl::Batch::create(geom::Plane().normal(planeNormal).size(planeSize), mPlaneShader);

	// We'll setup our sphere pretty much the same way we setup our plane:
	string sphereVertShader = "shaders/sphere_vert.glsl";
	string sphereFragShader = "shaders/sphere_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));

	float sphereRadius = 0.1f;
	int sphereResolution = 16;
	mSphereBatch = gl::Batch::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution),mSphereShader);
}

void L02_BasicScene::setupCamera()
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

void L02_BasicScene::mouseDown(MouseEvent event)
{
}

void L02_BasicScene::update()
{
}

void L02_BasicScene::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);
	gl::enableDepthRead();

	// drawing our plane is just as simple as calling the batch's draw() method
	mPlaneBatch->draw();

	/* Drawing The Sphere
		Drawing our sphere requires an extra step. By default, objects are drawn at the origin, or
		0,0,0 on the x,y, and z axes. Some geom::Sources and gl::draw()s allow you to specify an object's center, but
		let's avoid those to minimize confusion. Instead, we'll use proper matrix transformation to "move our
		object." Under the hood, it's a bit more complicated than that, but for now let's keep it simple:
	*/
	gl::pushModelMatrix();				// Preserve the current transformation
	gl::translate(vec3(0, 0.1, 0));		// translate 0.1 units up the y axis
	mSphereBatch->draw();				// Now draw
	gl::popModelMatrix();				// Restore the last transformation, which undoes any transforms we just did
}

/* EXERCISES
	If you're familiar with 3d, you probably are familiar with the concept of translating, rotating, and scaling. Try
	scaling the sphere up or down a bit.
		HINT: Maybe there's a gl:: function for scaling?

	Try drawing multiple objects using the same sphere batch
		HINT: See this post on the cinder forums: https://forum.libcinder.org/#Topic/23286000002367065

	Set up another batch with a different geom::Source or sources. For example, try drawing a silo by creating
	a batch with a geom::Cylinder and a geom::Cone
		HINT: Batches can share the same shader
		HINT: Use gl::translate to place the cone on top of the cylinder
*/
CINDER_APP(L02_BasicScene, RendererGl)

