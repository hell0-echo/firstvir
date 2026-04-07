/* ------------------------------------------------------------------
//  文件名    :   main.cpp
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   程序入口，显示主菜单
// ----------------------------------------------------------------*/
#include <QApplication>
#include "tg_main_window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("TypeGame");
    app.setStyle("Fusion");

    TgMainWindow window;
    window.show();

    return app.exec();
}