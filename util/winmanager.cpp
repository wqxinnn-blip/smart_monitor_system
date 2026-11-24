#include "winmanager.h"
WinManager *WinManager::instence;

WinManager::WinManager() {}


WinManager *WinManager::getInstence()
{
    if(WinManager::instence==nullptr){
        WinManager::instence=new WinManager;
    }
    return WinManager::instence;
}

