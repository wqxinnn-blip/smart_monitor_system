#include <QApplication>
#include "util/winmanager.h"
#include "model/settinginfo.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //1. 加载配置文件
    SettingInfo::loadFromFile();
    //2. 如果摄像头已经更换 在主函数里要进行判断 用新摄像头进行显示
    SettingInfo::detectCamera();
    //3. 执行
    WinManager::getInstence()->setting_widget.show();
    return a.exec();
}
