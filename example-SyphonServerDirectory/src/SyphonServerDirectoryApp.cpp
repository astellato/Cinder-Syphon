#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinderSyphon.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SyphonServerDirectoryApp : public AppNative {
public:
	void prepareSettings( Settings *settings );
    void setup();
    void quit();

    void keyUp( KeyEvent event );
    void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
    void mouseWheel( MouseEvent event );
    void resize();
    
	void update();
	void draw();
    
    void serverAnnounced(vector<syphonServerDescription> _servers);
    void serverRetired(vector<syphonServerDescription> _servers);
    
    syphonServerDirectory dir;
    syphonClient client;
    int dirIdx;
    string serverName, appName;
	
};

void SyphonServerDirectoryApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(800, 600);
    settings->setFrameRate(60.f);
}

void SyphonServerDirectoryApp::setup()
{
    dir.getServerAnnouncedSignal()->connect(bind(&SyphonServerDirectoryApp::serverAnnounced, this, std::placeholders::_1));
    dir.getServerRetiredSignal()->connect(bind(&SyphonServerDirectoryApp::serverRetired, this, std::placeholders::_1));
    
	dir.setup();
    client.setup();
    
    serverName = "SyphonServerDirectory";
    appName = "Example";
    
    if(dir.size() > 0){
        dirIdx = 0;
        client.set(dir.getDescription(dirIdx));
        client.bind();
        serverName = client.getServerName();
        appName = client.getApplicationName();
        
        if(serverName == ""){
            serverName = "null";
        }
        if(appName == ""){
            appName = "null";
        }
    } else {
        dirIdx = -1;
    }
}

void SyphonServerDirectoryApp::serverAnnounced(vector<syphonServerDescription> _servers){
    for( auto& dir : _servers){
        console()<<"SyphonServerDirectory Server Announced\nServer Name: "<<dir.serverName<<" | AppName: "<<dir.appName<<"\n";
    }
}

void SyphonServerDirectoryApp::serverRetired(vector<syphonServerDescription> _servers){
    for( auto& dir : _servers){
        console()<<"SyphonServerDirectory Server Retired\nServer Name: "<<dir.serverName<<" | AppName: "<<dir.appName<<"\n";
    }
}

void SyphonServerDirectoryApp::quit()
{
    
}

void SyphonServerDirectoryApp::update()
{
	//
}

void SyphonServerDirectoryApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    gl::color( ColorA::white() );
    gl::enableAlphaBlending();
    
    if(dir.isValidIndex(dirIdx))
        client.draw(Vec2f::zero());
    
    gl::drawStringCentered(serverName + ":" + appName, Vec2f(getWindowCenter().x, 20.f));
    gl::drawStringCentered("press any key to move through available Syphon servers", Vec2f(getWindowCenter().x, 580.f));
}

void SyphonServerDirectoryApp::resize()
{
	//
}

void SyphonServerDirectoryApp::keyDown( KeyEvent event )
{
	switch( event.getCode() )
	{
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        case KeyEvent::KEY_f:
            setFullScreen( !isFullScreen() );
            break;
        case KeyEvent::KEY_v:
            gl::enableVerticalSync( !gl::isVerticalSyncEnabled() );
            break;
    }
}

void SyphonServerDirectoryApp::keyUp( KeyEvent event )
{
    //press any key to move through all available Syphon servers
    dirIdx++;
    if(dirIdx > dir.size() - 1){
        dirIdx = 0;
    }
    
    client.set(dir.getDescription(dirIdx));
    client.bind();
    serverName = client.getServerName();
    appName = client.getApplicationName();
    
    if(serverName == ""){
        serverName = "null";
    }
    if(appName == ""){
        appName = "null";
    }
    
    
}

void SyphonServerDirectoryApp::mouseDown( MouseEvent event )
{
	//
}

void SyphonServerDirectoryApp::mouseUp( MouseEvent event )
{
	//
}

void SyphonServerDirectoryApp::mouseMove( MouseEvent event )
{
	//
}

void SyphonServerDirectoryApp::mouseDrag( MouseEvent event )
{
	//
}

void SyphonServerDirectoryApp::mouseWheel( MouseEvent event )
{
	//
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( SyphonServerDirectoryApp, RendererGl )
