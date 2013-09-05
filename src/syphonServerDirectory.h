//
//  syphonServerDirectory.h
//  Syphon-BasicExample
//
//  Created by Anthony Stellato on 9/3/13.
//
//

#pragma once

#include "cinder/Cinder.h"
#include <boost/signals2.hpp>
#include <algorithm>

class syphonServerDirectory;

class syphonServerDescription {
public:
    syphonServerDescription(){
        serverName = appName = "null";
    }
    
    syphonServerDescription(std::string _serverName, std::string _appName) : serverName(_serverName), appName(_appName){
        //
    }
    
    friend bool operator== (const syphonServerDescription& lhs, const syphonServerDescription& rhs){
        return (lhs.serverName == rhs.serverName) && (lhs.appName == rhs.appName);
    }
    
    std::string serverName, appName;
};

typedef boost::signals2::signal<void( std::vector<syphonServerDescription> )> SyphonServerAnnouncedSignal;
typedef boost::signals2::signal<void( std::vector<syphonServerDescription> )> SyphonServerUpdatedSignal;
typedef boost::signals2::signal<void( std::vector<syphonServerDescription> )> SyphonServerRetiredSignal;

class syphonServerDirectory {
public:
	syphonServerDirectory();
	~syphonServerDirectory();
	
    void setup();
    bool isSetup();
    int size();
    
    bool isValidIndex(int _idx);
    bool serverExists(std::string _serverName, std::string _appName);
    bool serverExists(syphonServerDescription _server);
    syphonServerDescription& getDescription(int _idx);
    
    std::vector<syphonServerDescription>& getServerList();
    void printList();
	SyphonServerAnnouncedSignal* getServerAnnouncedSignal();
    SyphonServerUpdatedSignal* getServerUpdatedSignal();
    SyphonServerRetiredSignal* getServerRetiredSignal();
    
    //needs to be public because of the nature of CFNotifications.  please do not call this.
    void handleNotification(CFStringRef name, CFDictionaryRef userInfo);
	
private:
	//void update(ofEventArgs& args);
    void refresh(bool isAnnounce);
    void serverAnnounced();
    void serverUpdated();
    void serverRetired();
    
    void addObservers();
    void removeObservers();
    
    bool CFStringRefToString(CFStringRef src, std::string &dest);
    
	bool bSetup;
    std::vector<syphonServerDescription> serverList;
    
    SyphonServerAnnouncedSignal mAnnouncedSignal;
    SyphonServerUpdatedSignal mUpdatedSignal;
    SyphonServerRetiredSignal mRetiredSignal;
};
