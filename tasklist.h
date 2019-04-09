#ifndef TASKLIST_H
#define TASKLIST_H
#include <QListWidget>

class TaskList : public QListWidget
{
    Q_OBJECT
private:
    void setupUI();
public:
    TaskList(QWidget *parent = nullptr);
};

#endif // TASKLIST_H
