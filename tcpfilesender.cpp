#include "tcpfilesender.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>

TcpFileSender::TcpFileSender(QWidget *parent)
    : QDialog(parent),
    clientProgressBar(new QProgressBar(this)),  // 初始化進度條
    socket(new QTcpSocket(this))                 // 初始化 TCP Socket
{
    // 設置進度條的最大值為 100%
    clientProgressBar->setRange(0, 100);
    clientProgressBar->setValue(0);

    // 當 TCP 連接成功時，開始傳送
    connect(socket, &QTcpSocket::connected, this, &TcpFileSender::startTransfer);

    // 當 TCP 發送進度有更新時，更新進度條
    connect(socket, &QTcpSocket::bytesWritten, this, &TcpFileSender::updateClientProgress);
}

TcpFileSender::~TcpFileSender() {
    delete clientProgressBar;
    delete socket;
}

void TcpFileSender::start()
{
    // 顯示文件選擇對話框
    fileName = QFileDialog::getOpenFileName(this, "選擇檔案", "", "所有檔案 (*)");

    if (!fileName.isEmpty()) {
        // 顯示選擇的檔案名稱
        QMessageBox::information(this, "選擇檔案", "選擇的檔案是：" + fileName);

        // 連接到服務器
        socket->connectToHost("127.0.0.1", 12345);  // 這是服務器的 IP 和端口
    }
}

void TcpFileSender::startTransfer()
{
    // 開啟選擇的檔案
    file = new QFile(fileName, this);
    if (!file->open(QFile::ReadOnly)) {
        QMessageBox::critical(this, "錯誤", "無法開啟檔案");
        return;
    }

    totalBytes = file->size();  // 獲取檔案總大小
    bytesSent = 0;               // 初始已發送位元組數

    // 設置進度條的最大值為檔案總大小
    clientProgressBar->setMaximum(totalBytes);
    clientProgressBar->setValue(0);

    // 每次傳送的數據塊大小
    const qint64 chunkSize = 64 * 1024;  // 64 KB

    // 在檔案結束前，循環讀取並發送
    while (!file->atEnd()) {
        QByteArray buffer = file->read(chunkSize);  // 讀取檔案的一塊
        socket->write(buffer);  // 發送檔案塊
        bytesSent += buffer.size();  // 更新已發送的大小

        // 等待資料寫入完成
        socket->waitForBytesWritten();
    }

    file->close();  // 檔案發送完成後關閉檔案

    // 傳送完成，顯示消息
    QMessageBox::information(this, "完成", "檔案傳送完成");
}

void TcpFileSender::updateClientProgress(qint64 numBytes)
{
    bytesSent += numBytes;
    clientProgressBar->setValue(bytesSent);  // 更新進度條
}

void TcpFileSender::openFile()
{
    // 顯示檔案選擇對話框
    fileName = QFileDialog::getOpenFileName(this, "選擇檔案", "", "所有檔案 (*)");

    if (!fileName.isEmpty()) {
        // 顯示選擇的檔案名稱
        QMessageBox::information(this, "選擇檔案", "選擇的檔案是：" + fileName);
    }
}
