#ifndef NEWTASK_H
#define NEWTASK_H

#include <QDialog>

namespace Ui {
class NewTask;
}

class NewTask : public QDialog
{
    Q_OBJECT
signals:
    void litFileIs(QString);
    void taskInfoIs(QString);
public:
    explicit NewTask(QWidget *parent = nullptr);
    ~NewTask();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    QString savePath;
private:
    Ui::NewTask *ui;
    void init();
    void saveLIT(QString filePath, int b, int e, int c);
    void setAvailabel();
};

#endif // NEWTASK_H
