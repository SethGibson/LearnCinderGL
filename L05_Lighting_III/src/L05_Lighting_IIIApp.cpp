#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L05_Lighting_IIIApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L05_Lighting_IIIApp::setup()
{
}

void L05_Lighting_IIIApp::mouseDown( MouseEvent event )
{
}

void L05_Lighting_IIIApp::update()
{
}

void L05_Lighting_IIIApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L05_Lighting_IIIApp, RendererGl )
