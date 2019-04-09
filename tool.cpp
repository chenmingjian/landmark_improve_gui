#include "tool.h"
#include <QVector>
#include <QDebug>
#include <QDir>
#include <QPoint>
#include <algorithm>
#include <QMessageBox>
#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"


bool compare(const QFileInfo &odd1,const QFileInfo &odd2)
{
//    chendebug << odd1.fileName() << odd2.fileName();
    QString s1 = odd1.fileName();
    auto dotIndex1 = s1.lastIndexOf(".");
    auto s1_pure = s1.mid(0, dotIndex1);

    QString s2 = odd2.fileName();
    auto dotIndex2 = s2.lastIndexOf(".");
    auto s2_pure = s2.mid(0, dotIndex2);
    return s1_pure.toInt() < s2_pure.toInt() ;
}

QStringList listDir(QString path,int begin, int end )
{
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug() << "dir is not exists!";
        return QStringList();
    }
    chendebug << path;
    QStringList filters;
    filters << "*.jpg";
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    uint dir_count = dir.count();
    if(dir_count <= 0)
    {
        qDebug() << "there is nothing in dir!";
        return QStringList();
    }

    QStringList string_list;
    //获取分隔符
    //QChar separator = QDir::separator();
    QChar separator = QChar('/');
    if(!path.contains(separator))
    {
        separator = QChar('\\');
    }
    QChar last_char = path.at(path.length()-1);
    if(last_char == separator)
    {
        separator = QChar();
    }
    auto list = dir.entryInfoList();
    std::sort(list.begin(), list.end(), compare);

    foreach(QFileInfo mfi ,list)
    {
        if(mfi.isFile())
        {

            QString name = mfi.fileName();
            auto dotIndex = name.lastIndexOf(".");
            auto name_pure = name.mid(0, dotIndex);
            auto name_int = name_pure.toInt();

            if (name_int >= begin && name_int <=end )
            {
                QString file_path = path + separator +  mfi.fileName();
                string_list.append(file_path);
            }
        }

    }
    return string_list;
}

QStringList listDir(QString path, QString f  )
{
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug() << "dir is not exists!";
        return QStringList();
    }

    QStringList filters;
    filters << f;
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    uint dir_count = dir.count();
    if(dir_count <= 0)
    {
        qDebug() << "there is nothing in dir!";
        return QStringList();
    }

    QStringList string_list;
    //获取分隔符
    //QChar separator = QDir::separator();
    QChar separator = QChar('/');
    if(!path.contains(separator))
    {
        separator = QChar('\\');
    }
    QChar last_char = path.at(path.length()-1);
    if(last_char == separator)
    {
        separator = QChar();
    }
    auto list = dir.entryInfoList();
    std::sort(list.begin(), list.end(), compare);

    foreach(QFileInfo mfi ,list)
    {
        if(mfi.isFile())
        {
            QString file_path = mfi.fileName();
            auto i = file_path.lastIndexOf(".");
            file_path.truncate(i);
            string_list.append(file_path);
        }

    }
    return string_list;
}

QStringList listPath(QString path, QString f)
{
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug() << "dir is not exists!";
        return QStringList();
    }

    QStringList filters;
    filters << f;
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    uint dir_count = dir.count();
    if(dir_count <= 0)
    {
        qDebug() << "there is nothing in dir!";
        return QStringList();
    }

    QStringList string_list;
    //获取分隔符
    //QChar separator = QDir::separator();
    QChar separator = QChar('/');
    if(!path.contains(separator))
    {
        separator = QChar('\\');
    }
    QChar last_char = path.at(path.length()-1);
    if(last_char == separator)
    {
        separator = QChar();
    }
    auto list = dir.entryInfoList();
    std::sort(list.begin(), list.end(), compare);

    foreach(QFileInfo mfi ,list)
    {
        if(mfi.isFile())
        {
            QString file_path = path + "/" + mfi.fileName();
            string_list.append(file_path);
        }
    }
    return string_list;
}

QVector<QPoint> loadtxt(QString path)
{
    QVector<QPoint> array = QVector<QPoint>();
    QFile coordinate_file(path);
    if(!coordinate_file.open(QIODevice::ReadOnly)){
        qDebug() << "can not open file, maybe wrong path." << path;
        return QVector<QPoint>();
    }
    while(!coordinate_file.atEnd())
    {
        QString sOnePoint = QString(coordinate_file.readLine()).trimmed();


        auto tmp =  sOnePoint.split(",");

        if (tmp.size() != 2 )
            continue;
        int x = tmp[0].toInt();
        int y = tmp[1].toInt();
        QPoint pOnePoint = QPoint(x, y);
        array << pOnePoint;
    }
    return array;

}

void savetxt(QString savePath, QVector<QPoint> vPoint)
{
    QFile saveFile(savePath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
     {
        chendebug << savePath;
        QMessageBox::critical(nullptr,"错误", "保存失败", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0);
        return;
    }
    QTextStream textStream(&saveFile);
    for (auto it = vPoint.begin(); it != vPoint.end(); ++it)
    {
        QString strX = QString::number(it->x());
        QString strY = QString::number(it->y());

        textStream << strX << ","<< strY <<"\n";
    }
    saveFile.close();
}
void saveChangeIndex(QString path, QSet<int> index)
{
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        return;
    }
    QTextStream textStream(&saveFile);

    for (auto it = index.begin(); it != index.end(); ++it)
    {
        QString i = QString::number(*it);
        textStream << i << "\n";
    }
    saveFile.close();
}
