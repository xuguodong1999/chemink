#include "chemink.h"
#include <QErrorMessage>
#include <QApplication>
#include <QDesktopWidget>

int throwErrorBox(const char *windowTitle, const char *message) {
    QErrorMessage dialog;
    dialog.setModal(true);
    dialog.setWindowTitle(QObject::tr(windowTitle));
    dialog.showMessage(QObject::tr(message));
    return dialog.exec();
}

QSize getDesktopSize() {
    return QApplication::desktop()->size();
}

bool releaseQrcFile(const QString &_qrcTopDir, const QString &_targetDir) {
    QDir qrcTopDir(_qrcTopDir);
    if (!qrcTopDir.exists()) {
        qDebug() << "!qrcTopDir.exists()";
        return false;
    }
    QDir targetDir(_targetDir);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(_targetDir)) {
            qDebug() << "!targetDir.mkpath(_targetDir)";
            return false;
        }
    }
    QFileInfoList fileInfoList = qrcTopDir.entryInfoList();
    while (!fileInfoList.isEmpty()) {
        QFileInfo tempFileInfo = fileInfoList.last();
        if (tempFileInfo.isFile()) {
            const QString newFilePath = _targetDir + "/" + tempFileInfo.fileName();
            if (!QFile::exists(newFilePath)) {
                QFile::copy(tempFileInfo.absoluteFilePath(), newFilePath);
            }
            if (!QFile::exists(newFilePath)) {
                qDebug() << "!QFile::exists(newFilePath)";
                return false;
            }
            fileInfoList.removeLast();
        } else if (tempFileInfo.isDir()) {
            if (tempFileInfo.fileName() != "." && tempFileInfo.fileName() != "..") {
                QDir subDir(tempFileInfo.filePath());
                fileInfoList.removeLast();
                fileInfoList.append(subDir.entryInfoList());
            }
        } else {
            fileInfoList.removeLast();
        }
    }
    return true;
}