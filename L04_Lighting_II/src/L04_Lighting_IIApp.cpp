#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L04_Lighting_IIApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L04_Lighting_IIApp::setup()
{
}

void L04_Lighting_IIApp::mouseDown( MouseEvent event )
{
}

void L04_Lighting_IIApp::update()
{
}

void L04_Lighting_IIApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L04_Lighting_IIApp, RendererGl )
