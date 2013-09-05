//
//  syphonServerDirectory.cpp
//  Syphon-BasicExample
//
//  Created by Anthony Stellato on 9/3/13.
//
//

#include "syphonServerDirectory.h"
#import <Syphon/Syphon.h>

// CFNotificationCallback implementation

static void notificationHandler(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
    (static_cast<syphonServerDirectory *>(observer))->handleNotification(name, userInfo);
}

// syphonServerDirectory implementation

syphonServerDirectory::syphonServerDirectory(){
    bSetup = false;
}

syphonServerDirectory::~syphonServerDirectory(){
    if(bSetup){
        removeObservers();
    }
}

bool syphonServerDirectory::isValidIndex(int _idx){
    return (_idx < serverList.size());
}

void syphonServerDirectory::setup(){
    if(!bSetup){
        bSetup = true;
        addObservers();
        //don't have to do this in OF.  Not sure what's different in the Cinder implementation, but this works.
        refresh(true);
    }
}

bool syphonServerDirectory::isSetup(){
    return bSetup;
}

void syphonServerDirectory::refresh(bool isAnnounce){
    std::vector<syphonServerDescription> eventArgs;
    
    for(NSDictionary* serverDescription in [[SyphonServerDirectory sharedDirectory] servers])
    {
        NSString* name = [serverDescription objectForKey:SyphonServerDescriptionNameKey];
        NSString* appName = [serverDescription objectForKey:SyphonServerDescriptionAppNameKey];
        //NSString *uuid = [serverDescription objectForKey:SyphonServerDescriptionUUIDKey];
        //NSImage* appImage = [serverDescription objectForKey:SyphonServerDescriptionIconKey];
        
        //NSString *title = [NSString stringWithString:appName];
        
        if(isAnnounce){
            bool exists = serverExists([name UTF8String], [appName UTF8String]);
            if(!exists){
                syphonServerDescription sy = syphonServerDescription(std::string([name UTF8String]),std::string([appName UTF8String]));
                serverList.push_back(sy);
                eventArgs.push_back(sy);
                std::cout<<"Adding server: "<<std::string([name UTF8String])<<" appName: "<< std::string([appName UTF8String])<<"\n";
            }
        } else {
            eventArgs.push_back(syphonServerDescription(std::string([name UTF8String]),std::string([appName UTF8String])));
        }
        
    }
    
    if(!isAnnounce){
        std::vector<syphonServerDescription> foundServers = eventArgs;
        eventArgs.clear();
        for(std::vector<syphonServerDescription>::iterator it = serverList.begin(); it != serverList.end(); ++it){
            if(std::find(foundServers.begin(), foundServers.end(), syphonServerDescription(it->serverName, it->appName)) == foundServers.end()){
                eventArgs.push_back(syphonServerDescription(it->serverName, it->appName));
                std::cout<<"Removing server: "<<it->serverName<<" appName: "<<it->appName<<"\n";
            }
        }
        serverList = foundServers;
    }
    
    if(isAnnounce){
        mAnnouncedSignal(eventArgs);
    } else {
        mRetiredSignal(eventArgs);
    }
}

bool syphonServerDirectory::serverExists(std::string _serverName, std::string _appName){
    return serverExists(syphonServerDescription(_serverName, _appName));
}

bool syphonServerDirectory::serverExists(syphonServerDescription _server){
    for(auto& s : serverList){
        if(s == _server){
            return true;
        }
    }
    
    return false;
}

syphonServerDescription& syphonServerDirectory::getDescription(int _idx){
    return serverList.at(_idx);
}

std::vector<syphonServerDescription>& syphonServerDirectory::getServerList(){
    return serverList;
}

void syphonServerDirectory::printList(){
    for(auto& s : serverList){
        ci::app::console()<<"serverName: "<<s.serverName<<" appName: "<<s.appName<<"\n";
    }
}

int syphonServerDirectory::size(){
    return serverList.size();
}

SyphonServerAnnouncedSignal* syphonServerDirectory::getServerAnnouncedSignal(){
    return &mAnnouncedSignal;
}

SyphonServerUpdatedSignal* syphonServerDirectory::getServerUpdatedSignal(){
    return &mUpdatedSignal;
}

SyphonServerRetiredSignal* syphonServerDirectory::getServerRetiredSignal(){
    return &mRetiredSignal;
}

// Unfortunately userInfo is null when dealing with CFNotifications from a Darwin notification center.  This is one of the few non-toll-free bridges between CF and NS.  Otherwise this class would be far less complicated.
void syphonServerDirectory::handleNotification(CFStringRef name, CFDictionaryRef userInfo){
    if((NSString*)name == SyphonServerAnnounceNotification){
        serverAnnounced();
    } else if((NSString*)name == SyphonServerUpdateNotification){
        serverUpdated();
    } else if((NSString*)name == SyphonServerRetireNotification){
        serverRetired();
    }
}

void syphonServerDirectory::serverAnnounced(){
    refresh(true);
}

void syphonServerDirectory::serverUpdated(){
    //TODO
}

void syphonServerDirectory::serverRetired(){
    refresh(false);
}

void syphonServerDirectory::addObservers(){
    CFNotificationCenterAddObserver
    (
     CFNotificationCenterGetLocalCenter(),
     this,
     (CFNotificationCallback)&notificationHandler,
     (CFStringRef)SyphonServerAnnounceNotification,
     NULL,
     CFNotificationSuspensionBehaviorDeliverImmediately
     );
    
    CFNotificationCenterAddObserver
    (
     CFNotificationCenterGetLocalCenter(),
     this,
     (CFNotificationCallback)&notificationHandler,
     (CFStringRef)SyphonServerUpdateNotification,
     NULL,
     CFNotificationSuspensionBehaviorDeliverImmediately
     );
    
    CFNotificationCenterAddObserver
    (
     CFNotificationCenterGetLocalCenter(),
     this,
     (CFNotificationCallback)&notificationHandler,
     (CFStringRef)SyphonServerRetireNotification,
     NULL,
     CFNotificationSuspensionBehaviorDeliverImmediately
     );
}

void syphonServerDirectory::removeObservers(){
    CFNotificationCenterRemoveEveryObserver(CFNotificationCenterGetLocalCenter(), this);
}

bool syphonServerDirectory::CFStringRefToString(CFStringRef src, std::string& dest){
    const char *cstr = CFStringGetCStringPtr(src, CFStringGetSystemEncoding());
    if (!cstr)
    {
        CFIndex strLen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(src) + 1,
                                                           CFStringGetSystemEncoding());
        char *allocStr = (char*)malloc(strLen);
        
        if(!allocStr)
            return false;
        
        if(!CFStringGetCString(src, allocStr, strLen, CFStringGetSystemEncoding()))
        {
            free((void*)allocStr);
            return false;
        }
        
        dest = allocStr;
        free((void*)allocStr);
        
        return true;
    }
    
    dest = cstr;
    return true;
}