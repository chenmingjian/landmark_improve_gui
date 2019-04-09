#ifndef NAMELISTWIDGET_H
#define NAMELISTWIDGET_H

#include <QListWidget>
//#define chendebug qDeubug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"


class NameListWidget : public QListWidget
{
    Q_OBJECT
signals:
    void pressOneItem(int index);
    void cancelAdd();
public:
    NameListWidget(QWidget*);
    ~NameListWidget(){}
    void setListSelectable(QVector<QPoint> points);
    void reset();
    void keyPressEvent(QKeyEvent *e);
private slots:
    void addDone();
    void changedDotDyeGreen(QSet<int> index);
private:
    bool isPress = false;
    QSet<int> addedIndex;
private:
    void setUi();

    void mousePressEvent(QMouseEvent *e);

};

#endif // NAMELISTVIEW_H
