#include "namelistwidget.h"
#include "selectitem.h"

#include <QStringListModel>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QtCore>
#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"

NameListWidget::NameListWidget(QWidget*)
{
    this->setUi();
    this->setAcceptDrops(true);
}

void NameListWidget::setUi()
{
    QStringList strList({"1.  G'  ","2.  N'  ","3.  Prn ","4.  Sn  ","5.  Ls  ","6.  St  ","7.  Li  ","8.  Si  ","9.  Pog'","10. Gn' ","11. Me' ","12. N   ","13. Or  ","14. ANS ","15. A   ","16. UIA ","17. SPr ","18. UI  ","19. LI  ","20. Id  ","21. LIA ","22. B   ","23. Pog ","24. Gn  ","25. Me  ","26. U6  ","27. L6  ","28. Go  ","29. PNS ","30. Ptm ","31. Ar  ","32. Co  ","33. S   ","34. Ba  ","35. P   "});
    QFont font("Consolas");
    for(auto str:strList)
    {
        SelectItem *item= new SelectItem(str);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(font);
        this->addItem(item);
    }
}

void NameListWidget::reset()
{
     this->addedIndex = QSet<int>();
}

void NameListWidget::setListSelectable(QVector<QPoint> points)
{
    for(int i = 0; i < points.count(); ++i)
    {
        if(points.at(i).x() != 0){
            this->item(i)->setBackgroundColor(Qt::white);
        }
        else
        {
            this->item(i)->setBackgroundColor(Qt::yellow);
        }
    }
    if(!addedIndex.isEmpty())
    {
        for(int i : addedIndex)
        {
            this->item(i)->setBackgroundColor(Qt::green);

        }
    }

}

void NameListWidget::addDone()
{
    this->currentItem()->setSelected(false);
}
void NameListWidget::changedDotDyeGreen(QSet<int> index)
{
    addedIndex = index;
}

void NameListWidget::keyPressEvent(QKeyEvent *e)
{
    QListWidget::keyPressEvent(e);
    auto index = this->currentRow();
    this->pressOneItem(index);

}

void NameListWidget::mousePressEvent(QMouseEvent *e)
{
    QListWidget::mousePressEvent(e);
    auto index = this->currentRow();
    if (e->button() == Qt::LeftButton)
        this->pressOneItem(index);
    else if (e->button() == Qt::RightButton){
        this->cancelAdd();
    }

}

