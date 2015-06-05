/*
Lesson 06: In Which We Learn about instancing
Topics Covered:
Instancing
*/

#include "L10_FBOApp.h"

void L10_FBO::setup()
{
	mNumSpheres = 32;
	mNumTorii = 32;
	setupGUI();
	setupCamera();
	setupSkybox();
	setupSpheres(mNumSpheres);
	setupTorii(mNumTorii);
	setupFbo();
}

void L10_FBO::setupCamera()
{
	float verticalFOV = 45.0f;
	float aspectRatio = getWindowAspectRatio();
	float nearClip = 0.1f;
	float farClip = 10.0f;
	vec3 cameraPosition = vec3(0, 0.5f, -1.5);
	vec3 cameraTarget = vec3(0);
	vec3 cameraUpAxis = vec3(0, 1, 0);

	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
	mCamera.setPivotDistance(length(cameraPosition));
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void L10_FBO::setupSpheres(int numElements)
{
	//setup sphere
	string sphereVertShader = "shaders/shape_vert.glsl";
	string sphereFragShader = "shaders/shape_reflect_frag.glsl";
	mSphereShader = gl::GlslProg::create(loadAsset(sphereVertShader), loadAsset(sphereFragShader));

	float sphereRadius = 0.075f;
	int sphereResolution = 32;
	mSphereMeshData = gl::VboMesh::create(geom::Sphere().radius(sphereRadius).subdivisions(sphereResolution));

	float step = (M_PI*2.0) / numElements;

	for (int i = 0; i < numElements; ++i)
	{
		float x = math<float>::cos(i*step);
		float y = math<float>::sin(i*step);
		mSpheres.push_back(Particle(vec3(x, y, 0.0), i));
	}

	geom::BufferLayout attributes;
	mSphereInstanceData = gl::Vbo::create(GL_ARRAY_BUFFER, mSpheres, GL_DYNAMIC_DRAW);
	attributes.append(geom::CUSTOM_0, 3, sizeof(Particle), offsetof(Particle, Position), 1);
	attributes.append(geom::CUSTOM_1, 3, sizeof(Particle), offsetof(Particle, Emissive), 1);
	attributes.append(geom::CUSTOM_2, 1, sizeof(Particle), offsetof(Particle, Size), 1);

	mSphereMeshData->appendVbo(attributes, mSphereInstanceData);
	mSphereBatch = gl::Batch::create(mSphereMeshData, mSphereShader, { { geom::CUSTOM_0, "iPosition" }, { geom::CUSTOM_1, "iColor" }, { geom::CUSTOM_2, "iSize" } });
	mSphereBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L10_FBO::setupTorii(int numElements)
{
	//setup torus
	string torusVertShader = "shaders/shape_vert.glsl";
	string torusFragShader = "shaders/shape_refract_frag.glsl";
	mTorusShader = gl::GlslProg::create(loadAsset(torusVertShader), loadAsset(torusFragShader));

	float torusOuterRadius = 0.1f;
	float torusInnerRadius = 0.075f;
	int torusAxisResolution = 32;
	int torusSegmentResolution = 16;
	mTorusMeshData = gl::VboMesh::create(geom::Torus().radius(torusOuterRadius, torusInnerRadius).subdivisionsHeight(torusSegmentResolution).subdivisionsAxis(torusAxisResolution));

	float step = (M_PI*2.0) / numElements;

	for (int i = 0; i < numElements; ++i)
	{
		float x = math<float>::cos(i*step);
		float z = math<float>::sin(i*step);
		mTorii.push_back(Particle(vec3(x, 0.0, z), i));
	}

	geom::BufferLayout attributes;
	mTorusInstanceData = gl::Vbo::create(GL_ARRAY_BUFFER, mTorii, GL_DYNAMIC_DRAW);
	attributes.append(geom::CUSTOM_0, 3, sizeof(Particle), offsetof(Particle, Position), 1);
	attributes.append(geom::CUSTOM_1, 3, sizeof(Particle), offsetof(Particle, Emissive), 1);
	attributes.append(geom::CUSTOM_2, 1, sizeof(Particle), offsetof(Particle, Size), 1);

	mTorusMeshData->appendVbo(attributes, mTorusInstanceData);
	mTorusBatch = gl::Batch::create(mTorusMeshData, mTorusShader, { { geom::CUSTOM_0, "iPosition" }, { geom::CUSTOM_1, "iColor" }, { geom::CUSTOM_2, "iSize" } });
	mTorusBatch->getGlslProg()->uniform("uCubemapSampler", 0);
}

void L10_FBO::setupSkybox()
{
	mSkyboxCubemap = gl::TextureCubeMap::create(loadImage(loadAsset("textures/skybox.png")));
	mSkyboxShader = gl::GlslProg::create(loadAsset("shaders/skybox_vert.glsl"), loadAsset("shaders/skybox_frag.glsl"));
	mSkyboxShader->uniform("uCubemapSampler", 0);
}

void L10_FBO::setupGUI()
{
	mParamLightPosX = 0.0f;
	mParamLightPosY = 1.0f;
	mParamLightPosZ = -1.0f;

	mParamSphereSpecularPower = 16.0f;
	mParamSphereSpecularStrength = 1.0f;
	mParamSphereAmbientStrength = 1.0f;
	mParamSphereEnvStrength = 1.0f;

	mParamTorusSpecularPower = 16.0f;
	mParamTorusSpecularStrength = 1.0f;
	mParamTorusAmbientStrength = 1.0f;
	mParamTorusEnvStrength = 1.0f;
	mParamTorusRefractId = 0;
	mParamTorusRefractIndices = vector<float>{1.0f, 1.33f, 1.309f, 1.52f, 2.42f};
	mParamTorusRefractNames = vector<string>{"Air", "Water", "Ice", "Glass", "Diamond"};
	mParamSphereColor = Color(0.1f, 0.25f, 0.5f);
	mParamTorusColor = Color(0.1f, 0.5f, 0.25f);

	mParamWhiteMax = 1.0f;
	mParamWhiteMid = 0.1f;
	mParamWhiteThresh = 0.25f;

	mGUI = params::InterfaceGl::create("Params", ivec2(300, 350));
	mGUI->addParam<float>("mParamLightPosX", &mParamLightPosX).optionsStr("label='light x'");
	mGUI->addParam<float>("mParamLightPosY", &mParamLightPosY).optionsStr("label='light y'");
	mGUI->addParam<float>("mParamLightPosZ", &mParamLightPosZ).optionsStr("label='light z'");

	mGUI->addParam<float>("mParamWhiteMax", &mParamWhiteMax);
	mGUI->addParam<float>("mParamWhiteMid", &mParamWhiteMid);
	mGUI->addParam<float>("mParamWhiteThresh", &mParamWhiteThresh);

	mGUI->addSeparator("");
	mGUI->addText("Sphere");
	mGUI->addSeparator("");
	mGUI->addParam<Color>("mParamSphereColor", &mParamSphereColor).optionsStr("label='color'");

	mGUI->addSeparator("");
	mGUI->addText("Torus");
	mGUI->addSeparator("");
	mGUI->addParam<Color>("mParamTorusColor", &mParamTorusColor).optionsStr("label='color'");
	mGUI->addParam("mParamTorusRefract", mParamTorusRefractNames, &mParamTorusRefractId).optionsStr("label='refraction'");
}

void L10_FBO::setupFbo()
{
	mRawFbo = gl::Fbo::create(1280,720,gl::Fbo::Format().colorTexture(gl::Texture2d::Format().dataType(GL_FLOAT).internalFormat(GL_RGBA32F)));
	mHighPassFbo = gl::Fbo::create(1280, 720, gl::Fbo::Format().colorTexture(gl::Texture2d::Format().internalFormat(GL_RGB)));
	mBlurHFbo = gl::Fbo::create(1280, 720, gl::Fbo::Format().colorTexture(gl::Texture2d::Format().internalFormat(GL_RGB)));
	mBlurVFbo = gl::Fbo::create(1280, 720, gl::Fbo::Format().colorTexture(gl::Texture2d::Format().internalFormat(GL_RGB)));

	mHighPassShader = gl::GlslProg::create(loadAsset("shaders/passthru_vert.glsl"), loadAsset("shaders/highpass_frag.glsl"));
	mHighPassShader->uniform("uTextureSampler", 0);
	
	mBlurShader = gl::GlslProg::create(loadAsset("shaders/blur_vert.glsl"), loadAsset("shaders/blur_frag.glsl"));
	mBlurShader->uniform("uTextureSampler", 0);
}

void L10_FBO::mouseDown(MouseEvent event)
{
}

void L10_FBO::update()
{
	updateSpheres();
	updateTorii();
	updateFbo();
}

void L10_FBO::updateSpheres()
{
	float frames = getElapsedFrames();
	float step = (M_PI*2.0) / mNumSpheres;

	for (auto s = begin(mSpheres); s != end(mSpheres); ++s)
	{
		s->Update(frames, step, true);
	}

	mSphereInstanceData->bufferData(mSpheres.size()*sizeof(Particle), mSpheres.data(), GL_DYNAMIC_DRAW);
}

void L10_FBO::updateTorii()
{
	float frames = getElapsedFrames();
	float step = (M_PI*2.0) / mNumTorii;

	for (auto s = begin(mTorii); s != end(mTorii); ++s)
	{
		s->Update(frames, step, false);
	}

	mTorusInstanceData->bufferData(mTorii.size()*sizeof(Particle), mTorii.data(), GL_DYNAMIC_DRAW);

}

void L10_FBO::updateFbo()
{
	mRawFbo->bindFramebuffer();
	gl::clear(ColorA::zero());
	drawScene();
	mRawFbo->unbindFramebuffer();
	
	mHighPassFbo->bindFramebuffer();
	gl::clear(ColorA::zero());
	gl::setMatricesWindow(getWindowSize());
	mHighPassShader->bind();
	mHighPassShader->uniform("uWhiteMax", mParamWhiteMax);
	mHighPassShader->uniform("uWhiteMid", mParamWhiteMid);
	mHighPassShader->uniform("uWhiteThreshold", mParamWhiteThresh);
	mRawFbo->bindTexture(0);
	gl::drawSolidRect(Rectf({vec2(0), getWindowSize()}));
	mRawFbo->unbindTexture(0);
	mHighPassFbo->unbindFramebuffer();

	mBlurHFbo->bindFramebuffer();
	gl::clear(ColorA::zero());
	gl::setMatricesWindow(getWindowSize());
	mBlurShader->bind();
	mBlurShader->uniform("uBlurAxis", vec2(1, 0));
	mBlurShader->uniform("uBlurSize", 1.5f);
	mBlurShader->uniform("uBlurStrength", 2.0f);
	mHighPassFbo->bindTexture(0);
	gl::drawSolidRect(Rectf({ vec2(0), getWindowSize() }));
	mHighPassFbo->unbindTexture(0);
	mBlurHFbo->unbindFramebuffer();

	mBlurVFbo->bindFramebuffer();
	gl::clear(ColorA::zero());
	gl::setMatricesWindow(getWindowSize());
	mBlurShader->bind();
	mBlurShader->uniform("uBlurAxis", vec2(0, 1));
	mBlurShader->uniform("uBlurSize", 3.0f);
	mBlurShader->uniform("uBlurStrength", 2.0f);
	mBlurHFbo->bindTexture(0);
	gl::drawSolidRect(Rectf({ vec2(0), getWindowSize() }));
	mBlurHFbo->unbindTexture(0);
	mBlurVFbo->unbindFramebuffer();
}

void L10_FBO::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCamera);

	drawSkybox();

	gl::setMatricesWindow(getWindowSize());
	gl::enableAlphaBlending();
	gl::draw(mRawFbo->getColorTexture(), vec2(0));

	gl::enableAdditiveBlending();
	gl::draw(mBlurVFbo->getColorTexture(), vec2(0));
	gl::disableAlphaBlending();
	mGUI->draw();
}

void L10_FBO::drawScene()
{
	gl::setMatrices(mCamera);
	gl::enableDepthRead();
	mSkyboxCubemap->bind(0);
	mSphereBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mSphereBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mSphereBatch->getGlslProg()->uniform(mUniformColor, mParamSphereColor);

	gl::pushMatrices();
	gl::rotate(toRadians((float)getElapsedFrames()));
	mSphereBatch->drawInstanced(mNumSpheres);
	gl::popMatrices();

	mTorusBatch->getGlslProg()->uniform(mUniformEyePos, mCamera.getEyePoint());
	mTorusBatch->getGlslProg()->uniform(mUniformLightPos, vec3(mParamLightPosX, mParamLightPosY, mParamLightPosZ));
	mTorusBatch->getGlslProg()->uniform(mUniformColor, mParamTorusColor);
	mTorusBatch->getGlslProg()->uniform(mUniformRefract, mParamTorusRefractIndices[mParamTorusRefractId]);

	gl::pushMatrices();
	gl::rotate(toRadians((float)getElapsedFrames()), vec3(0, 1, 0));
	mTorusBatch->drawInstanced(mNumTorii);
	gl::popMatrices();

	mSkyboxCubemap->unbind(0);

}

void L10_FBO::drawSkybox()
{
	gl::disableDepthRead();
	mSkyboxShader->bind();
	mSkyboxCubemap->bind(0);
	gl::drawCube(vec3(0), vec3(1));
	mSkyboxCubemap->unbind();
}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(1280, 720);
	pSettings->setFrameRate(60);
}

CINDER_APP(L10_FBO, RendererGl, prepareSettings)
