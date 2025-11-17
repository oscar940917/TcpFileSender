#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>  // 確保包含 QtWidgets 用於進度條等 UI 控制元件

class TcpFileSender : public QDialog
{
    Q_OBJECT

public:
    TcpFileSender(QWidget *parent = nullptr);
    ~TcpFileSender();

public slots:
    void start();                      // 開始傳送
    void startTransfer();              // 開始檔案傳輸
    void updateClientProgress(qint64 numBytes);  // 更新進度
    void openFile();                   // 開啟檔案選擇對話框

private:
    QProgressBar *clientProgressBar;   // 進度條
    QFile *file;                        // 用於讀取檔案的 QFile 物件
    QTcpSocket *socket;                 // 用於傳輸的 QTcpSocket
    QString fileName;                   // 儲存選擇的檔案名稱
    qint64 totalBytes;                  // 檔案總大小
    qint64 bytesSent;                   // 已經發送的檔案位元組
};

#endif // TCPFILESENDER_H
