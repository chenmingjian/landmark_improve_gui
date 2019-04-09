#ifndef OPENTASK_H
#define OPENTASK_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class openTask;
}

class OpenTask : public QDialog
{
    Q_OBJECT
signals:
    void litFileIs(QString);
    void taskInfoIs(QString);
public:
    explicit OpenTask(QWidget *parent = nullptr);
    ~OpenTask();

private slots:
    void on_buttonBox_accepted();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    QString rootPath;
    QString loadPath;
    int b, e, c;
private:
    Ui::openTask *ui;
    void loadLIT(QString loadPath);

    void getList();
};

#endif // OPENTASK_H
