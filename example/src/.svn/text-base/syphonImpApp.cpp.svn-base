/*
 syphonImpApp.cpp
 Cinder Syphon Implementation Sample App
 
 Created by astellato on 2/6/11
 
 Copyright 2011 astellato, bangnoise (Tom Butterworth) & vade (Anton Marini).
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

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinderSyphon.h"

#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;

#define WIDTH 800
#define HEIGHT 600

//A quick spiral class in order to draw something to the screen
class spiral {
	
public:
	spiral(){
		limit = 10.0f;
		resolution = 0.1f;
	}
	
	void set(float _limit, float _resolution){
		limit = _limit;
		resolution = _resolution;
	}
	
	void calc(){
		Vec2f here;
		
		for(float t = 0.0f; t <= limit; t += resolution){
			here.x = t * cos(t);
			here.y = t * sin(t);
			mSpiral.push_back(here);
		}
	}
	
	void draw(){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &mSpiral[0].x);
		glDrawArrays(GL_LINE_STRIP, 0, mSpiral.size());
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	
	vector<Vec2f> mSpiral;
	float limit;
	float resolution;
};

class syphonImpApp : public AppBasic {
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
	
	void randomizeSurface(Surface* inputSurface);
	 
	gl::Texture mTex; // the texture to be published
	Surface mSurface; // a surface to manipulate
	spiral archimedes; // our spiral
	float mRot; // our changing rotation
	
	syphonServer mScreenSyphon; //each item to publish requires a different server
	syphonServer mTextureSyphon;
	
	syphonClient mClientSyphon; //our syphon client
};

void syphonImpApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(WIDTH,HEIGHT);
	settings->setFrameRate(60.f);
}

void syphonImpApp::setup()
{
	mTex = gl::Texture(200, 100); //create our texture to publish
	mSurface = Surface8u(200, 100, false); //create a surface to manipulate
	randomizeSurface(&mSurface); //randomize our surface
	mTex.update(mSurface); //tell the texture about our changes
	
	archimedes.set(100.f, 0.6f); //set up and calculate our spiral
	archimedes.calc();
	mRot = 0.f;
	
	mScreenSyphon.setName("Cinder Screen"); // set a name for each item to be published
	mTextureSyphon.setName("Cinder Texture");
	
	mClientSyphon.setup();
    
	// in order for this to work, you must run simple server from the testapps directory
	// any other syphon item you create would work as well, just change the name
    mClientSyphon.setApplicationName("Simple Server");
    mClientSyphon.setServerName("");
	
	mClientSyphon.bind();
}

void syphonImpApp::update()
{
	if(getElapsedFrames() % 2 == 0) // for those of us with slower computers
		randomizeSurface(&mSurface);
	mTex.update(mSurface);
	mRot += 0.2f;
}

void syphonImpApp::draw()
{
	gl::enableAlphaBlending();
	gl::clear( Color( 0.1f, 0.1f, 0.1f ) );

	//draw our spiral
	gl::pushModelView();
	gl::translate(Vec2f(getWindowWidth()/2, getWindowHeight()/2));
	gl::rotate(mRot);
	gl::scale(Vec3f(4.f, 4.f, 1.f));
	gl::color(ColorA(1.f, 0.f, 0.f, 1.f));
	archimedes.draw();
	gl::popModelView();
	
	//draw our publishable texture
	if(mTex){
		gl::color(ColorA(1.f, 1.f, 1.f, 1.f));
		gl::draw(mTex);
	}
	
	mScreenSyphon.publishScreen(); //publish the screen
	mTextureSyphon.publishTexture(&mTex); //publish our texture
	
	//anything that we draw after here will not be published
	
	mClientSyphon.draw(Vec2f(300.0f, 0.0f)); //draw our client image
}

void syphonImpApp::randomizeSurface(Surface* inputSurface)
{
	Surface::Iter inputIter( inputSurface->getIter() );
	while( inputIter.line() ) {
		while( inputIter.pixel() ) {
			inputIter.r() = Rand::randInt(0, 255);
			inputIter.g() = Rand::randInt(0, 255);
			inputIter.b() = Rand::randInt(0, 255);
		}
	}
}

void syphonImpApp::keyDown( KeyEvent event )
{
	//
}

void syphonImpApp::mouseDown( MouseEvent event )
{
	//
}

void syphonImpApp::mouseUp( MouseEvent event )
{
	//
}

void syphonImpApp::mouseMove( MouseEvent event )
{
	//
}

void syphonImpApp::mouseDrag( MouseEvent event )
{
	//
}

CINDER_APP_BASIC( syphonImpApp, RendererGl )

