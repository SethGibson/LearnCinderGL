#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L06_InstancingApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L06_InstancingApp::setup()
{
}

void L06_InstancingApp::mouseDown( MouseEvent event )
{
}

void L06_InstancingApp::update()
{
}

void L06_InstancingApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L06_InstancingApp, RendererGl )
