#ifndef TOOL_H
#define TOOL_H

#include <QPoint>
#include <QString>
#include <QDebug>

QStringList listDir(QString path,int begin, int end);

QStringList listDir(QString path, QString f);

QStringList listPath(QString path, QString f);

QVector<QPoint> loadtxt(QString path);

void savetxt(QString savePath, QVector<QPoint> vPoint);

void saveChangeIndex(QString path, QSet<int> index);

#endif // TOOL_H
