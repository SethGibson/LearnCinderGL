#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L03_Lighting_IApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L03_Lighting_IApp::setup()
{
}

void L03_Lighting_IApp::mouseDown( MouseEvent event )
{
}

void L03_Lighting_IApp::update()
{
}

void L03_Lighting_IApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L03_Lighting_IApp, RendererGl )
