#ifndef IMGLABEL_H
#define IMGLABEL_H

#include "namelistwidget.h"

#include <QFile>
#include <QLabel>
#include <QListWidgetItem>
//#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"

class ImgLabel : public QLabel
{
    Q_OBJECT
signals:
    void addDone();
    void showAmplifier(QPixmap);
    void changedDot(QSet<int>);
public:
    ImgLabel();
    ~ImgLabel();
    ImgLabel(QWidget *);
    void setImg();
public:
    QImage img;
    QVector<QPoint> maybeLabel;
    QVector<QPoint> realLabel;
    QVector<QPoint> redDot;
    QString savePath;
    QString saveChangePath;
    QPoint rightBottomPoint;
    NameListWidget *nameList = nullptr;
    QSet<int> touchedIndex;
    bool isMoving = false;
    bool addStatus = false;
    int maybeSelectdIndex =-1;
    int selectedIndex = -1;
    QString taskInfo;
    void upSelectedPoint();
    void leftSelectedPoint();
    void rightSelectedPoint();
    void downSelectedPoint();
    void reset();
    void setAmplifier();
    void loadTouchedIndex(QFile &f);
    void loadTouchedIndex();
public slots:
    void addNewPoint(QListWidgetItem *item);
private slots:
    void pressOneItem(int index);
    void cancelAdd();
private:
    double scale;
    bool isPress=false;
    bool isNearLabelFlag = false;


    int addIndex;
    QPixmap rawPixmap;
    int pointMoveStride;



private:
    QPixmap drawMaybeLabel(QPixmap &pixmap, double scale);
    QPixmap drawRedDot(QPixmap &pixmap, double scale);

    void drawName(QPixmap &pixmap);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    int isNearLabel(QPoint p);
    void mouseReleaseEvent(QMouseEvent *ev);
    void refresh(int index, QPoint p);
    int getNearestLoc(QPoint p, QVector<QPoint> pVector, float threshold);
    int getParkLoc(QPoint p);
    void park(int index, QPoint p);
    void save();
    void changed(int index);
    void getPointMoveStride();
    void movePoint(QPoint newPoint);

    void selectPoint();
    void leaveEvent(QEvent *e);
    void noPark(int index, QPoint p);

    void histogramEqualization(QPixmap &p);
};

#endif // IMGLABEL_H
