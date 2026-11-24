QT += widgets sql multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    model/camerainfo.cpp \
    model/exceptioninfo.cpp \
    model/featureimage.cpp \
    model/imagemodel.cpp \
    model/operationinfo.cpp \
    model/operationmodel.cpp \
    model/settinginfo.cpp \
    model/usermodel.cpp \
    model/video.cpp \
    model/videomodel.cpp \
    service/detecttask.cpp \
    service/imagetask.cpp \
    service/monitortask.cpp \
    service/operlogservice.cpp \
    service/recordtask.cpp \
    service/userservice.cpp \
    service/videotask.cpp \
    util/dbconnect.cpp \
    util/dbconnectionpool.cpp \
    util/file.cpp \
    util/globaluser.cpp \
    util/thumblabel.cpp \
    util/winmanager.cpp \
    view/captcha.cpp \
    view/imagewidget.cpp \
    view/loginwidget.cpp \
    view/logwidget.cpp \
    view/mainwidget.cpp \
    view/monitorwidget.cpp \
    view/motionreviewwidget.cpp \
    view/registerwidget.cpp \
    view/reviewwidget.cpp \
    view/settingwidget.cpp \
    yolo/Detect.cpp \
    yolo/YOLO.cpp

HEADERS += \
    model/camerainfo.h \
    model/exceptioninfo.h \
    model/featureimage.h \
    model/imagemodel.h \
    model/operationinfo.h \
    model/operationmodel.h \
    model/settinginfo.h \
    model/usermodel.h \
    model/video.h \
    model/videomodel.h \
    service/detecttask.h \
    service/imagetask.h \
    service/monitortask.h \
    service/operlogservice.h \
    service/recordtask.h \
    service/userservice.h \
    service/videotask.h \
    util/dbconnect.h \
    util/dbconnectionpool.h \
    util/file.h \
    util/globaluser.h \
    util/thumblabel.h \
    util/winmanager.h \
    view/captcha.h \
    view/imagewidget.h \
    view/loginwidget.h \
    view/logwidget.h \
    view/mainwidget.h \
    view/monitorwidget.h \
    view/motionreviewwidget.h \
    view/registerwidget.h \
    view/reviewwidget.h \
    view/settingwidget.h \
    yolo/Detect.h \
    yolo/YOLO.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 配置opencv路径
OPENCV_ROOT = D:/OpenCV/opencv
# OpenCV 头文件
INCLUDEPATH += $${OPENCV_ROOT}/include
# OpenCV库文件
LIBS += -L$${OPENCV_ROOT}/x64/mingw/lib

# 必选核心库(基础功能+视频读写，依赖FFmpeg)
LIBS += -llibopencv_core4100 # 核心模块（内存管理、数据结构）
LIBS += -llibopencv_imgproc4100 # 图像处理（滤波、缩放等）
LIBS += -llibopencv_videoio4100 # 视频 I/O（调用 FFmpeg 读写视频）
LIBS += -llibopencv_highgui4100 # 简单 GUI 显示（如 imshow）
LIBS += -llibopencv_imgcodecs4100 # 图像编解码（读写图片）
LIBS += -llibopencv_dnn4100         # OpenCV访问Onnx模型的接口模块
# 可选扩展库（根据项目需求添加）
LIBS += -llibopencv_features2d4100 # 特征检测（如 SIFT/SURF）
LIBS += -llibopencv_calib3d4100 # 相机标定、3D 重建
LIBS += -llibopencv_objdetect4100 # 目标检测（如 Haar 级联）
LIBS += -llibopencv_video4100   # 背景减除专用


DESTDIR = $$PWD/bin

RESOURCES += \
    resources.qrc

TARGET = MonitorSys