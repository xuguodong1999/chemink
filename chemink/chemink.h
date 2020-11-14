#pragma once

#include <QSize>
#include <QDir>
#include <QDebug>

int throwErrorBox(const char *windowTitle, const char *message);

QSize getDesktopSize();

bool releaseQrcFile(const QString &_qrcTopDir, const QString &_targetDir);

