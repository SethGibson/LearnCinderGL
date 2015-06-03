#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L07_Motion_IApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L07_Motion_IApp::setup()
{
}

void L07_Motion_IApp::mouseDown( MouseEvent event )
{
}

void L07_Motion_IApp::update()
{
}

void L07_Motion_IApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L07_Motion_IApp, RendererGl )
