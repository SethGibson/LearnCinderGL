#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L02_BasicSceneApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L02_BasicSceneApp::setup()
{
}

void L02_BasicSceneApp::mouseDown( MouseEvent event )
{
}

void L02_BasicSceneApp::update()
{
}

void L02_BasicSceneApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L02_BasicSceneApp, RendererGl )
