#include "opentask.h"
#include "ui_opentask.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>

#include "tool.h"
#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"

#pragma execution_character_set("utf-8")
OpenTask::OpenTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openTask)
{
    ui->setupUi(this);
    getList();
}

OpenTask::~OpenTask()
{
    delete ui;
}

void OpenTask::on_buttonBox_accepted()
{
    QString taskInfo;
    QString taskName = ui->listWidget->currentItem()->text();
    if (taskName == tr("空"))
        return ;
    taskInfo += taskName + ":" + QString::number(b) + "~" +QString::number(e);
    taskInfoIs(taskInfo);

    litFileIs(loadPath);
    this->deleteLater();
}

void OpenTask::loadLIT(QString loadPath)
{
    QFile f(loadPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        chendebug << "open file fail.";
    }
    QTextStream textStream(&f);

    textStream >> b;
    textStream >> e;
    textStream >> c;

    auto str = f.fileName();
    auto i = str.lastIndexOf('/');
    auto strm = str.mid(i +1 );

    ui->taskName->setText(strm);

    chendebug << b << e;
    ui->BandE->setText(QString::number(b)+"~" + QString::number(e));
    ui->currentIndex->setText(QString::number(c));

    f.close();
}

void OpenTask::getList()
{
#ifndef chen
    QString rootDir = "D:/ws/code/landmark_improve_gui/img_to_improve";
#else
    QString rootDir = "./img_to_improve";
#endif
    rootPath = rootDir;
    auto tmp = listDir(rootDir, "*.lit");
    chendebug << tmp;
    QFont font("Consolas");
    if (tmp.isEmpty())
    {
        QListWidgetItem *item= new QListWidgetItem("空");
        item->setFont(font);
        ui->listWidget->addItem(item);
    }
    else{
        for(auto str:tmp)
        {
            QListWidgetItem *item= new QListWidgetItem(str);
            item->setFont(font);
            ui->listWidget->addItem(item);
        }
    }
}



void OpenTask::on_listWidget_itemClicked(QListWidgetItem *item)
{
    auto filename = item->text() + ".lit";
    loadPath = rootPath + "/" +  filename;
    loadLIT(loadPath);

}

void OpenTask::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    auto filename = current->text() + ".lit";
    loadPath = rootPath + "/" +  filename;
    loadLIT(loadPath);
}
