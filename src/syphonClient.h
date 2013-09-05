/*
 syphonClient.h
 Cinder Syphon Implementation
 
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
#include "syphonServerDirectory.h"

class syphonClient {
	public:
	syphonClient();
	~syphonClient();
	
	void setup();
	
    void set(syphonServerDescription _server);
    void set(std::string _serverName, std::string _appName);
    
	void setApplicationName(std::string _appName);
    void setServerName(std::string _serverName);
	
    void bind();
    void unbind();
    
	void draw(ci::Vec2f origin, ci::Vec2f drawSize);
	void draw(ci::Vec2f origin);
    void draw(float x, float y, float w, float h);
    void draw(float x, float y);
    //TODO: draw with bounds
    
    int getWidth();
    int getHeight();
    ci::Vec2i getSize();
    
    std::string& getApplicationName();
    std::string& getServerName();
    
protected:

	void* mClient;
    void* latestImage;
	ci::gl::TextureRef mTex;
	int width, height;
	bool bSetup;
	std::string appName, serverName;
};