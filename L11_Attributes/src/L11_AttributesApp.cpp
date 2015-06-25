#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L11_AttributesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L11_AttributesApp::setup()
{
}

void L11_AttributesApp::mouseDown( MouseEvent event )
{
}

void L11_AttributesApp::update()
{
}

void L11_AttributesApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L11_AttributesApp, RendererGl )
