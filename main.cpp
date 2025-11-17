#include "tcpfilesender.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpFileSender w;
    w.show();  // 顯示窗口
    return a.exec();
}
