#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L08_Motion_IIApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L08_Motion_IIApp::setup()
{
}

void L08_Motion_IIApp::mouseDown( MouseEvent event )
{
}

void L08_Motion_IIApp::update()
{
}

void L08_Motion_IIApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L08_Motion_IIApp, RendererGl )
