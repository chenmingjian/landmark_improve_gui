#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#define chendebug qDeubug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *ev);
    void setPix();
private slots:
    void on_actionLoad_Dir_triggered();


private:
    QString imgDir;
    QString maybeLabelDir;
    QString redDotDir;
    QString realLabelDir;
    QStringList imgPathList;
    QString imgPath;
    int currentIndex = 0;
    QString configPath;
    int begin = 1;
    int end = 519;
    QVector<QPoint> maybeLabel;
    QVector<QPoint> redDot;
    QString savePath;

    QImage img;
    QString filename;
    typedef void (MainWindow::*func)();
    std::map<int,func> key;
    bool initIsOK = false;

private slots:
    void next();
    void setAmplifier(QPixmap pixmap);
    void previous();
    void on_actionReconfig_triggered();

    void init(QString path);
    void setTaskInfo(QString ti);
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    void setupUi();
    void initLoad(QString name = "");
    QPixmap drawRedDot(QPixmap &pixmap, float scale);
    QPixmap drawMaybeLabel(QPixmap &pixmap, float scale);
    void initKey();

    void rightPressed();
    void leftPressed();
    void downPressed();
    void upPressed();
    void enterPressed();
    void closeEvent(QCloseEvent *event);
    void loadConfigFile();
    void saveConfigFile();
};

#endif // MAINWINDOW_H
