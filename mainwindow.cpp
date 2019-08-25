#include "imglabel.h"
#include "mainwindow.h"
#include "newtask.h"
#include "opentask.h"
#include "tool.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <iostream>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QListWidgetItem>
#include <QImage>
#include<QKeyEvent>
#include <QMessageBox>
#include <QInputDialog>
#define chendebug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->hide();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->label_2->hide();


    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), ui->label, SLOT(addNewPoint(QListWidgetItem*)));
    connect(ui->label, SIGNAL(addDone()),ui->listWidget, SLOT(addDone()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(previous()));
    connect(ui->label,SIGNAL(showAmplifier(QPixmap)), this, SLOT(setAmplifier(QPixmap)));
    connect(ui->label, SIGNAL(changedDot(QSet<int>)), ui->listWidget, SLOT(changedDotDyeGreen(QSet<int>)));
    connect(ui->listWidget, SIGNAL(pressOneItem(int)), ui->label, SLOT(pressOneItem(int)));
    connect(ui->listWidget, SIGNAL(cancelAdd()), ui->label, SLOT(cancelAdd()));

    this->setupUi();
    this->setFocusPolicy(Qt::ClickFocus);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

//    this->showMaximized();
    QPixmap pixmap(QSize(128,128));
    auto amplifiedPixmap = pixmap.scaled(256, 256,Qt::KeepAspectRatio);
    QPainter painter(&amplifiedPixmap);
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::gray);
    painter.drawRect(0,0,256,256);
    ui->label_2->setPixmap(amplifiedPixmap);

#ifndef chen
//    this->on_actionLoad_Dir_triggered();
//    on_actionReconfig_triggered();
#endif
}
//void MainWindow::on_actionReconfig_triggered()
//{
//    QFile config(configPath);
//    if (!config.remove())
//        chendebug << "fail";
//    on_actionLoad_Dir_triggered();
//    currentIndex = 0;
//}
void MainWindow::on_actionReconfig_triggered()
{
    auto ot = new OpenTask();
    ot->show();
    connect(ot, SIGNAL(taskInfoIs(QString)), this, SLOT(setTaskInfo(QString)));
    connect(ot, SIGNAL(litFileIs(QString)), this, SLOT(init(QString)));


}
void MainWindow::on_actionLoad_Dir_triggered()
{
    auto nt = new NewTask();
    nt->show();
    connect(nt, SIGNAL(taskInfoIs(QString)), this, SLOT(setTaskInfo(QString)));
    connect(nt, SIGNAL(litFileIs(QString)), this, SLOT(init(QString)));

}
void MainWindow::init(QString path)
{
    chendebug << path;
    if (!path.isEmpty())
        configPath = path;
    else {
        return;
    }
#ifdef chen
//    QString rootDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//    if (rootDir == "")
//        return;
    QString rootDir = "./img_to_improve";
#else
//    QString rootDir = "C:/Users/chen/Desktop/重标记打包Demo v1.5/img_to_improve";
    QString rootDir = "D:/ws/code/landmark_improve_gui/img_to_improve";
#endif

    QFile configFile(configPath);
    if (configFile.exists())
    {

        loadConfigFile();
    }
    else {

    }

    QString imgDirName = QString("img");
    QString maybeLabelDirName = QString("maybe_label");
    QString redDotDirName = QString("red_dot");
    QString realLabelDirName = QString("real_label");

    QString imgDir = rootDir + "/" + imgDirName;
    QString maybeLabelDir = rootDir + "/" + maybeLabelDirName;
    QString redDotDir = rootDir + "/" + redDotDirName;
    QString realLabelDir = rootDir + "/" + realLabelDirName;

    QDir dir0(imgDir);
    if(!dir0.exists())
    {
        QMessageBox::critical(nullptr,"错误", "请检测img_to_improve文件路径是否正确。",QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0);
        return;
    }
    QDir dir1(maybeLabelDir);
    if(!dir1.exists())
    {
        QMessageBox::critical(nullptr,"错误", "请检测img_to_improve文件路径是否正确。",QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0);
        return;
    }
    QDir dir2(redDotDir);
    if(!dir2.exists())
    {
        QMessageBox::critical(nullptr,"错误", "请请检测img_to_improve文件路径是否正确。",QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0);
        return;
    }
    QDir dir3(realLabelDir);
    if(!dir3.exists())
    {
        QMessageBox::critical(nullptr,"错误", "请检测img_to_improve文件路径是否正确。",QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0);
        return;
    }
    this->imgDir = imgDir;
    this->maybeLabelDir = maybeLabelDir;
    this->redDotDir = redDotDir;

    // get config file name
    auto tmp = configPath;
    auto i = tmp.lastIndexOf("/");
    auto j = tmp.lastIndexOf(".");
    auto configName = tmp.mid(i+1, j-i-1);
    // end get config file name

    this->realLabelDir = realLabelDir+ '/' + configName;
    QDir d(this->realLabelDir);

    QDir *temp = new QDir;
    bool exist = temp->exists(this->realLabelDir);
    if(exist)
        ;
    else
    {
        bool ok = temp->mkdir(this->realLabelDir);
        if( !ok )
            QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹创建失败！"));

    }

    QStringList pathList = listDir(imgDir, begin, end);
    chendebug << pathList;
    this->imgPathList = pathList;
    if (pathList.isEmpty())
    {
          QMessageBox::critical(nullptr , "错误" , "范围错误", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0 );
          return;
    }
    this->imgPath = pathList.at(currentIndex);
    this->initLoad();

    ui->listWidget->show();
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->label_2->show();
    initIsOK = true;
    saveConfigFile();
}


QPoint getRightBottomPoint()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    auto w = desktopWidget->width();
    auto h = desktopWidget->height();
    return QPoint(w, h);
}

void MainWindow::initLoad(QString name )
{
    auto tmpString = imgPath;
    chendebug << imgPath;
    QFile f(imgPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         QMessageBox::critical(nullptr , "错误" , "范围错误", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0 );
         return;
    }
    if (name == "")
    {
        auto nameEnd = tmpString.lastIndexOf(".");
        tmpString.truncate(nameEnd);
        auto nameBegin = tmpString.lastIndexOf("/");
        name = tmpString.mid(nameBegin+1);
    }
    else
    {

    }
    this->filename = name;
    auto redDotPath = this->redDotDir + "/" + name + ".txt";

    auto realLabelPath  = this->realLabelDir + "/" + name + ".txt";

    QString maybeLabelPath = this->maybeLabelDir + "/" + name + ".txt";


    auto saveChangePath = realLabelDir + "/" + name +"_change_index.txt";

    ui->label->nameList = ui->listWidget;
    ui->listWidget->reset();



    QImage img = QImage(tmpString);
    ui->label->reset();
    ui->label->img = img;
    ui->label->redDot = loadtxt(redDotPath);

    ui->label->maybeLabel = loadtxt(maybeLabelPath);

    ui->label->savePath = realLabelPath;

    QFile realLabelFile (realLabelPath);
    if (realLabelFile.exists())
    {
        ui->label->realLabel = loadtxt(realLabelPath);
    }
    else
    {
        ui->label->realLabel = QVector<QPoint>();
    }


    ui->label->saveChangePath = saveChangePath;


    ui->label->loadTouchedIndex();

//    ui->label->touchedIndex = QSet<int>();


    ui->label->rightBottomPoint = getRightBottomPoint();


    ui->label->selectedIndex = -1;


    ui->label->setAmplifier();
    ui->label->setImg();
}

void MainWindow::next()
{
    auto pointVector = ui->label->realLabel;
    auto isHaveNonePoint = false;
    auto findRes = std::find(pointVector.begin(), pointVector.end(), QPoint(0, 0));
    if (findRes != pointVector.end())
    {
        isHaveNonePoint = true;
        QMessageBox::critical(nullptr , "错误" , "请标记列表中全部黄色未标记点", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 	0 );

        return;
    }
    if(currentIndex != imgPathList.count() - 1 && !isHaveNonePoint)
    {
        imgPath = imgPathList.at(++currentIndex);
        this->initLoad();
        this->saveConfigFile();
    }
}

void MainWindow::previous()
{
    if (currentIndex != 0 )
    {
        imgPath = imgPathList.at(--currentIndex);
        this->initLoad();
        this->saveConfigFile();
    }

}

void MainWindow::setAmplifier(QPixmap pixmap)
{
    ui->label_2->setPixmap(pixmap);
}



void MainWindow::upPressed()
{
    ui->label->upSelectedPoint();
}

void MainWindow::downPressed()
{
    ui->label->downSelectedPoint();
}

void MainWindow::leftPressed()
{
    ui->label->leftSelectedPoint();
}
void MainWindow::rightPressed()
{
    ui->label->rightSelectedPoint();
}

void MainWindow::enterPressed()
{
    ui->label->isMoving = false;
    ui->label->addStatus = false;
    ui->label->selectedIndex = -1;
    ui->label->setImg();
    ui->label->setAmplifier();
    ui->label->nameList->setCurrentRow(-1);
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(initIsOK){
        auto k = ev->key();
        if (ev->modifiers() == Qt::ControlModifier)
        {
            switch (k)
            {
            case Qt::Key_Up:upPressed(); break;
            case Qt::Key_Down:downPressed();break;
            case Qt::Key_Left:leftPressed();break;
            case Qt::Key_Right:rightPressed();break;
            }
        }
        else
        {
            ui->listWidget->keyPressEvent(ev);
            switch (k)
            {
            case Qt::Key_Return:
            case Qt::Key_Enter: enterPressed();break;
            }
        }
    }

}


void MainWindow::closeEvent(QCloseEvent *e)
{
    if (e != nullptr)
    {

        saveConfigFile();
    }

}

void MainWindow::saveConfigFile()
{
    QFile config(configPath);
    if (!config.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        chendebug << "file open fail" << configPath;
    }
    else
    {
        QTextStream textStream(&config);

        textStream << begin << "\n";

        textStream << end << "\n";
        if (begin+currentIndex > end)
        {
            textStream<< end;
        }
        else
        {
            textStream << begin+currentIndex << "\n";
        }


        config.close();
    }
}
void MainWindow::loadConfigFile()
{
    QFile config(configPath);
    if (!config.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        chendebug << "file open fail" << configPath;

    }
    else
    {
        QTextStream textStream(&config);

        textStream >> begin ;
        textStream >> end ;
        textStream >> currentIndex ;
        currentIndex -= begin;

        if(currentIndex < 0)
            currentIndex = begin;
        chendebug << begin << end << currentIndex;
        config.close();
    }

}


void MainWindow::setTaskInfo(QString ti)
{
    chendebug << ti;
    ui->label->taskInfo = ti;
}





void MainWindow::on_actionExit_triggered()
{
    this->close();
}
