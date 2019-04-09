#include "imglabel.h"
#include "tool.h"
#include "pixandcv.h"
#include <QPainter>
#include <QMouseEvent>
#include <limits>
#include <QListWidgetItem>
#include <QDebug>
#include <map>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


//#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTpION__<<"]"


ImgLabel::ImgLabel()
{
    this->setMouseTracking(true);
}

ImgLabel::ImgLabel(QWidget *)
{
    this->setMouseTracking(true);
}

ImgLabel::~ImgLabel()
{

}

void ImgLabel::reset()
{
    isMoving = false;
    maybeSelectdIndex =-1;
    selectedIndex = -1;
    isPress=false;
    isNearLabelFlag = false;
    addStatus = false;
}

QPixmap ImgLabel::drawRedDot(QPixmap &pixmap, double scale)
{
    QPainter painter(&pixmap);
    QPen pointPen(Qt::blue);
    pointPen.setWidth(3);
    painter.setPen(pointPen);
    for(auto rd = redDot.begin(); rd!=redDot.end();++rd)
    {
        int x = static_cast<int>(rd->x() * scale);
        int y = static_cast<int>(rd->y() * scale);
        painter.drawPoint(x, y);
    }

    return pixmap;
}

QPixmap ImgLabel::drawMaybeLabel(QPixmap &pixmap, double scale)
{
    QVector<QString>nameMap = {"G'", "N'", "Prn", "Sn", "Ls", "St", "Li", "Si", "Pog'", "Gn'", "Me'", "N", "Or", "ANS", "A", "UIA", "SPr", "UI", "LI", "Id", "LIA", "B", "Pog", "Gn", "Me", "U6", "L6", "Go", "PNS", "Ptm", "Ar", "Co", "S", "Ba", "P"};

    QPainter painter(&pixmap);
    QPen rPen(Qt::red);
    rPen.setWidth(5);
    painter.setPen(rPen);
    QFont font;
    font.setPointSize(10);
    font.setFamily("Microsoft YaHei");
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
    painter.setFont(font);
    int i =-1;
    if (realLabel.size() !=35)
    {
        realLabel = maybeLabel;

    }
    else
    {
    }
    for(auto ml:realLabel)
    {
        auto it = std::find(touchedIndex.begin(), touchedIndex.end(),++i);
        if (it == touchedIndex.end())
        {
            int x = static_cast<int>(ml.x() * scale);
            int y = static_cast<int>(ml.y() * scale);
            painter.drawPoint(x, y);

            painter.drawText(QPoint(x+5, y-5), nameMap.at(i));
        }
    }

    QPen gPen(Qt::green);
    gPen.setWidth(5);
    painter.setPen(gPen);
    for (auto i :touchedIndex)
    {
        if(i!= selectedIndex)
        {
            int x = static_cast<int>(realLabel.at(i).x() * scale);
            int y = static_cast<int>(realLabel.at(i).y() * scale);
            painter.drawPoint(x, y);
            painter.drawText(QPoint(x+5, y-5), nameMap.at(i));
        }
    }

    QPen bPen(QColor(255,125,50));
    bPen.setWidth(5);
    painter.setPen(bPen);
    if (selectedIndex!= -1)
    {
        int x = static_cast<int>(realLabel.at(selectedIndex).x() * scale);
        int y = static_cast<int>(realLabel.at(selectedIndex).y() * scale);
        painter.drawPoint(x, y);
        painter.drawText(QPoint(x+5, y-5), nameMap.at(selectedIndex));
    }
    auto beginIndex =savePath.lastIndexOf("/")+1;
    auto endIndex = savePath.lastIndexOf(".");
    painter.drawText(QPoint(0,pixmap.height()-20), QString("当前图片:") + savePath.mid(beginIndex, endIndex-beginIndex));
    QString statusName;
    if (addStatus)
        statusName = QString("当前状态:") + QString("添加");
    else if(isMoving)
        statusName =  QString("当前状态:") + QString("微调");
    painter.drawText(QPoint(0,pixmap.height()-40), statusName );
//    chendebug << taskInfo;
    painter.drawText(QPoint(0,0+40), "当前任务 " + taskInfo );

    return pixmap;
}

void ImgLabel::setImg()
{
    auto h = static_cast<double>(img.height());
    auto screen_h = rightBottomPoint.y() - 120;
    auto scale = screen_h / h; // Here maybe have a bug.
    this->scale = scale;
    auto scaledImg = img.scaledToHeight(screen_h);
    QPixmap pixmap = QPixmap::fromImage(scaledImg);
    rawPixmap = pixmap;
    drawRedDot(pixmap, scale);
    drawMaybeLabel(pixmap, scale);
    this->setPixmap(pixmap);

    if (nameList)
        nameList->setListSelectable(realLabel);
}

template <typename T>
QVector <int> sortIndexes(const QVector<T> &v) {
    QVector<int> idx(v.size());
    for (int i = 0; i != idx.size(); ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(),
              [&v](int i1, int i2) {return v[i1] < v[i2];});
    return idx;
}


int ImgLabel::getNearestLoc(QPoint p, QVector<QPoint> pVector,float threshold = 10)
{
    QVector<int> distance = QVector<int>();
    auto x = p.x();
    auto y = p.y();
    for (auto it = pVector.begin(); it != pVector.end(); ++it)
    {

        int mLx = static_cast<int>(it->x() * scale);
        int mLy = static_cast<int>(it->y() * scale);
        auto dist = (mLx - x) * (mLx - x) + (mLy - y) * (mLy - y);
        distance << dist;
    }
    QVector<int> sorted_distance = QVector<int>();
    auto vIndex = sortIndexes(distance);
    std::sort(distance.begin(), distance.end());
    if (distance.at(0) <= threshold * threshold){
        return vIndex.at(0);
    }
    else
        return -1;
}

int ImgLabel::isNearLabel(QPoint p)
{
    return getNearestLoc(p, realLabel);
}

int ImgLabel::getParkLoc(QPoint p)
{
    QVector<int> distance = QVector<int>();
    auto x = p.x();
    auto y = p.y();
    for (auto it = redDot.begin(); it != redDot.end(); ++it)
    {
        auto findResult= std::find(realLabel.begin(), realLabel.end(), *it);
        if(findResult == realLabel.end())
        {
            int mLx = static_cast<int>(it->x() * scale);
            int mLy = static_cast<int>(it->y() * scale);
            auto dist = (mLx - x) * (mLx - x) + (mLy - y) * (mLy - y);
            distance << dist;
        }
        else
        {
            distance << std::numeric_limits<int>::max();
        }
    }
    auto vIndex = sortIndexes(distance);
    std::sort(distance.begin(), distance.end());
    return vIndex.at(0);
}

void ImgLabel::mouseMoveEvent(QMouseEvent *ev)
{
    auto mouseLoc = ev->pos();
    if(!addStatus && !isMoving)
    {
        auto index = this->isNearLabel(mouseLoc);
        if (!isPress)
        {
            if(index == -1)
            {
                isNearLabelFlag = false;
                this->setCursor(Qt::ArrowCursor);
            }
            else
            {
                isNearLabelFlag = true;
                maybeSelectdIndex = index;
                this->setCursor(Qt::OpenHandCursor);
            }
        }
        else
        {
            if (isNearLabelFlag)
            {
                selectedIndex = maybeSelectdIndex;
                QPoint p(static_cast<int>(mouseLoc.x()/scale), static_cast<int>(mouseLoc.y()/scale));
                this->refresh(selectedIndex, p);
            }
        }
    }
    else
    {

    }
    QPixmap pixmap = rawPixmap.copy(mouseLoc.x()-64, mouseLoc.y()-64, 128,128);
    auto amplifiedPixmap = pixmap.scaled(256, 256,Qt::KeepAspectRatio);
    histogramEqualization(amplifiedPixmap);
    QPainter painter(&amplifiedPixmap);
    painter.setPen(Qt::white);
    painter.drawLine(0,128,256,128);
    painter.drawLine(128,0,128,256);
    this->showAmplifier(amplifiedPixmap);
}

void ImgLabel::mousePressEvent(QMouseEvent *ev)
{
    if (!isMoving)
    {
        if (!addStatus)
        {
            isPress = true;
            if (ev->button() == Qt::LeftButton && isNearLabelFlag)
            {
                this->setCursor(Qt::ClosedHandCursor);
            }
            this->mouseMoveEvent(ev);
            this->selectPoint();
        }
        else
        {
            if (ev->button() == Qt::RightButton)
            {
                addStatus = false;
            }
        }
    }

}

void ImgLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!isMoving){
        if(!addStatus)
        {
            isPress = false;
            this->setCursor(Qt::ArrowCursor);
            this->mouseMoveEvent(ev);
            if (isNearLabelFlag)
            {
                if (ev->modifiers() != Qt::ControlModifier)
                    this->park(selectedIndex, ev->pos());
                else
                    this->noPark(selectedIndex, ev->pos());
                this->changed(selectedIndex);
            }
        }
        else
        {
            if (selectedIndex != -1)
            {
                realLabel.remove(addIndex);
                QPoint p(static_cast<int>(ev->pos().x() / scale), static_cast<int>(ev->pos().y()/ scale));
                realLabel.insert(addIndex, p);
                this->setCursor(Qt::ArrowCursor);
                this->mouseMoveEvent(ev);
                addStatus = false;
                this->changed(addIndex);
                this->setImg();
                addDone();
            }

        }
    }

    this->save();
    this->setImg();
    this->isMoving = false;
}

void ImgLabel::selectPoint()
{
    nameList->setCurrentRow(selectedIndex);
}
void ImgLabel::refresh(int index, QPoint p)
{
    realLabel.remove(index);
    realLabel.insert(index, p);

    this->setImg();
}

void ImgLabel::noPark(int index, QPoint p)
{
    QPoint P(static_cast<int>(p.x()/scale), static_cast<int>(p.y()/scale));
    this->refresh(index, P);
}

void ImgLabel::park(int index, QPoint p)
{
    auto parkIndex = getParkLoc(p);
    this->refresh(index, redDot[parkIndex]);
}

void ImgLabel::save()
{
    savetxt(savePath, realLabel);
//    saveChangeIndex(saveChangePath, touchedIndex);

}

void ImgLabel::addNewPoint(QListWidgetItem* item)
{
    auto lastDotIndex = item->text().lastIndexOf(".");
    auto str = item->text();
    str.truncate(lastDotIndex);

    int index = str.toInt();
    addIndex = index - 1;
    if(realLabel.at(addIndex) == QPoint(0,0))
    {
        this->setCursor(Qt::CrossCursor);
        addStatus = true;

        selectedIndex = addIndex;

        this->changed(addIndex);
        this->setImg();
    }

}

void ImgLabel::changed(int index)
{
    if (index < 0 || index > 34)
    {
        return;
    }
    if (realLabel.at(index)!= maybeLabel.at(index))
    {
        touchedIndex<<index;

    }
    else {
        if (maybeLabel.at(index) == realLabel.at(index))
        {
            touchedIndex.remove(index);
        }
    }
    changedDot(touchedIndex);
    nameList->setListSelectable(realLabel);
}

void ImgLabel::pressOneItem(int index)
{
    this->selectedIndex = index;
    this->changed(index);
    this->setAmplifier();
    this->setImg();
    this->setFocus();
}

void ImgLabel::setAmplifier()
{
    if (selectedIndex != -1 && realLabel.at(selectedIndex) != QPoint(0,0))
    {
        auto p = realLabel.at(selectedIndex);
        QPoint pScaled(static_cast<int>(p.x()*scale), static_cast<int>(p.y()*scale));

        QPixmap pixmap = rawPixmap.copy(QRect(pScaled.x()-64, pScaled.y()-64, 128,128));
        auto amplifiedPixmap = pixmap.scaled(256, 256,Qt::KeepAspectRatio);
        histogramEqualization(amplifiedPixmap);
        QPainter painter(&amplifiedPixmap);
        painter.setPen(Qt::white);
        painter.drawLine(0,128,256,128);
        painter.drawLine(128,0,128,256);

        this->showAmplifier(amplifiedPixmap);
    }
    else {
        QPixmap pixmap(QSize(128,128));
        auto amplifiedPixmap = pixmap.scaled(256, 256,Qt::KeepAspectRatio);
        histogramEqualization(amplifiedPixmap);
        QPainter painter(&amplifiedPixmap);
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::gray);
        painter.drawRect(0,0,256,256);
        if (selectedIndex!=-1 && realLabel.at(selectedIndex) == QPoint(0,0)&& !addStatus)
        {
            painter.setPen(Qt::black);
            painter.drawText(QPoint(60,132), "双击此条目进入添加点模式");
        }
        this->showAmplifier(amplifiedPixmap);
    }
}
void ImgLabel::getPointMoveStride()
{
   pointMoveStride = static_cast<int> (2.0 / this->scale);
}

void ImgLabel:: movePoint(QPoint newPoint)
{
    realLabel.remove(selectedIndex);
    realLabel.insert(selectedIndex, newPoint);

    auto pos = this->mapToGlobal(newPoint * scale);
    QCursor::setPos(pos);
    this->setCursor(Qt::BlankCursor);
    this->isMoving = true;
    this->addStatus = false;
    this->save();
    changed(selectedIndex);
    this->setImg();
}


void ImgLabel::upSelectedPoint()
{
    if (selectedIndex != -1)
    {
        getPointMoveStride();
        auto p = realLabel.at(selectedIndex);
        QPoint newPoint(p.x(), p.y() - pointMoveStride);
        movePoint(newPoint);
    }
}

void ImgLabel::downSelectedPoint()
{
    if (selectedIndex != -1)
    {
        getPointMoveStride();
        auto p = realLabel.at(selectedIndex);
        QPoint newPoint(p.x(), p.y() + pointMoveStride);
        movePoint(newPoint);
    }
}
void ImgLabel::leftSelectedPoint()
{
    if (selectedIndex != -1)
    {
        getPointMoveStride();
        auto p = realLabel.at(selectedIndex);
        QPoint newPoint(p.x() - pointMoveStride, p.y());
        movePoint(newPoint);
    }
}

void ImgLabel::rightSelectedPoint()
{
    if (selectedIndex != -1)
    {
        getPointMoveStride();
        auto p = realLabel.at(selectedIndex);
        QPoint newPoint(p.x() + pointMoveStride, p.y());
        movePoint(newPoint);
    }
}

void ImgLabel::cancelAdd()
{
    addStatus = false;
}

void ImgLabel::leaveEvent(QEvent *)
{
    setAmplifier();
}

void ImgLabel::loadTouchedIndex(QFile &f)
{
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QSet<int> index;
    while(!f.atEnd()) {
        QByteArray line = f.readLine();
        QString str(line);
        index << str.toInt();
    }
    this->touchedIndex = index;
    changedDot(index);

    nameList->setListSelectable(realLabel);
}

void ImgLabel::loadTouchedIndex()
{
    if(realLabel.size() == 35)
    {
        for(int i = 0; i != 35; ++i)
        {
            changed(i);
        }
    }
    else
    {
        touchedIndex = QSet<int>();
    }
}


void ImgLabel::histogramEqualization(QPixmap &p)
{
    using namespace cv;
    using namespace CVS;

    Mat image = QPixmapToCvMat(p);
    Mat gray;
    cvtColor(image,gray,CV_BGR2GRAY);
//    if (gray.empty())
//    {

////        chendebug << "打开图片失败,请检查";
//        return;
//    }
//    equalizeHist(gray, gray);
//    p = cvMatToQPixmap(gray);

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(4);

    Mat dst;
    clahe->apply(gray,dst);
    p = cvMatToQPixmap(dst);

//    p = cvMatToQPixmap(image);
//    Mat src = image;
//    Mat dst;
//    Mat kernel = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
//    filter2D(src, dst, src.depth(), kernel);


}

