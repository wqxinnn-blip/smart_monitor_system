#ifndef WINMANAGER_H
#define WINMANAGER_H

#include "../view/mainwidget.h"
#include "../view/loginwidget.h"
#include "../view/registerwidget.h"
#include "../view/monitorwidget.h"
#include "../view/settingwidget.h"
#include "../view/reviewwidget.h"
#include "../view/imagewidget.h"

class WinManager
{
public:
    static WinManager *getInstence();
    WinManager (const WinManager&)=delete;
    WinManager operator=(const WinManager&)=delete;

    mainWidget main_widget;
    LoginWidget login_widget;
    RegisterWidget register_widget;
    MonitorWidget monitor_widget;
    SettingWidget setting_widget;
    ReviewWidget review_widget;
    ImageWidget image_widget;
private:
    WinManager();
    static WinManager *instence;
};

#endif // WINMANAGER_H
