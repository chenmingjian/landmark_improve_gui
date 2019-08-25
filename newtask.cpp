#include "newtask.h"
#include "ui_newtask.h"
#include "tool.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QDebug>
#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"
#pragma execution_character_set("utf-8")

NewTask::NewTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTask)
{
    ui->setupUi(this);
    this->init();
}

NewTask::~NewTask()
{
    delete ui;
}

void NewTask::init()
{
#ifndef chen
    savePath = "D:/ws/code/landmark_improve_gui/img_to_improve";
#else
    savePath = "./img_to_improve";
#endif
    QRegExp begin_rx("[1-9][0-9]{0,2}");
    ui->beginLineEdit->setValidator(new QRegExpValidator(begin_rx,this));
    QRegExp end_rx("[1-9][0-9]{0,2}");
    ui->endLineEdit->setValidator(new QRegExpValidator(end_rx,this));
    QRegExp taskName_rx("^\\w+$");
    ui->taskNameLineEdit->setValidator(new QRegExpValidator(taskName_rx,this));
}

void NewTask::setAvailabel()
{
    auto pathList = listPath(savePath, "*.lit");
    chendebug << pathList << savePath;
    QString s("未占用范围：");
    for(auto tmp : pathList)
    {
        s += tmp;
    }
}

void NewTask::on_buttonBox_accepted()
{
    QString begin = ui->beginLineEdit->text();
    QString end = ui->endLineEdit->text();
    QString taskName = ui->taskNameLineEdit->text();

    if (begin.isEmpty() || end.isEmpty() || taskName.isEmpty())
    {
        QMessageBox::critical(nullptr , "错误" , "请填写信息", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape, 0);
        return;
    }
    int b = begin.toInt();
    int e = end.toInt();
    if (b > e)
    {
        QMessageBox::critical(nullptr , "错误" , "开始位置大于结束位置", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape, 0);
        return;
    }

    int currentIndex = b;

    QString taskInfo;
    taskInfo += taskName + ":" + QString::number(b)  + "~" + QString::number(e);
    taskInfoIs(taskInfo);


    QString filePath = savePath + "/" + taskName + ".lit";

    saveLIT(filePath, b, e, currentIndex);
    litFileIs(filePath);


}

void NewTask::saveLIT(QString filePath, int b, int e, int c)
{
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        chendebug << "open file fail.";
    }

    QTextStream textStream(&f);

    textStream << b << "\n";

    textStream << e << "\n";

    textStream << c << "\n";

    f.close();
}


void NewTask::on_buttonBox_rejected()
{
    this->deleteLater();
}
