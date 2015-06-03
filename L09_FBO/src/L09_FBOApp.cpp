#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L09_FBOApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L09_FBOApp::setup()
{
}

void L09_FBOApp::mouseDown( MouseEvent event )
{
}

void L09_FBOApp::update()
{
}

void L09_FBOApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L09_FBOApp, RendererGl )
