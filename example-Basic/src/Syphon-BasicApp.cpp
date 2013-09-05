/*
 SyphonBasicApp.cpp
 Cinder Syphon Implementation Sample App
 
 Created by astellato on 2/6/11
 
 Copyright 2011 astellato (Anthony Stellato), bangnoise (Tom Butterworth) & vade (Anton Marini).
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cinder/app/AppNative.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinderSyphon.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define WIDTH 512
#define HEIGHT 512

class SyphonBasicApp : public AppNative {
public:
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void setup();
	void update();
	void draw();
    
    gl::TextureRef	mLogo;
	gl::GlslProgRef	mShader;
	float			mAngle;
	
	syphonServer mScreenSyphon; //each item to publish requires a different server
	syphonServer mTextureSyphon;
	
	syphonClient mClientSyphon; //our syphon client
};

void SyphonBasicApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(WIDTH,HEIGHT);
	settings->setFrameRate(60.f);
}

void SyphonBasicApp::setup()
{
	try {
		mLogo = gl::Texture::create( loadImage( loadAsset("cinder_logo_alpha.png") ) );
	}
	catch( ... ) {
		std::cout << "unable to load the texture file!" << std::endl;
	}
	
	try {
		mShader = gl::GlslProg::create( loadAsset("passThru_vert.glsl"), loadAsset("gaussianBlur_frag.glsl") );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}
	
	mAngle = 0.0f;
	
	mScreenSyphon.setName("Screen Output"); // set a name for each item to be published
	mTextureSyphon.setName("Texture Output");
	
	mClientSyphon.setup();
    
	// in order for this to work, you must run simple server which is a syphon test application
    // feel free to change the app and server name for your specific case
    mClientSyphon.set("", "Simple Server");
    
    mClientSyphon.bind();
}

void SyphonBasicApp::update()
{
	mAngle += 0.05f;
}

void SyphonBasicApp::draw()
{
	gl::enableAlphaBlending();
	gl::clear(Color::white());
    gl::color(ColorA(1.f, 1.f, 1.f, 1.f));
	
    mShader->bind();
    mShader->uniform( "tex0", 0 );
    mShader->uniform( "sampleOffset", Vec2f( cos( mAngle ), sin( mAngle ) ) * ( 3.0f / getWindowWidth() ) );
    gl::draw(mLogo, Vec2f::zero());
    mShader->unbind();
    
    mClientSyphon.draw(Vec2f(16.f, 64.f)); //draw our client image
    
	mScreenSyphon.publishScreen(); //publish the screen's output
	mTextureSyphon.publishTexture(mLogo); //publish our texture without shader
	
	//anything that we draw after here will not be published
    gl::drawStringCentered("This text will not be published to Syphon.", Vec2f(getWindowCenter().x, 20.f), ColorA::black());
	
}

void SyphonBasicApp::keyDown( KeyEvent event )
{
	//
}

void SyphonBasicApp::mouseDown( MouseEvent event )
{
	//
}

void SyphonBasicApp::mouseUp( MouseEvent event )
{
	//
}

void SyphonBasicApp::mouseMove( MouseEvent event )
{
	//
}

void SyphonBasicApp::mouseDrag( MouseEvent event )
{
	//
}

CINDER_APP_NATIVE( SyphonBasicApp, RendererGl )