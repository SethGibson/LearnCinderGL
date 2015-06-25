#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class L01_Ex1App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void L01_Ex1App::setup()
{
}

void L01_Ex1App::mouseDown( MouseEvent event )
{
}

void L01_Ex1App::update()
{
}

void L01_Ex1App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( L01_Ex1App, RendererGl )
