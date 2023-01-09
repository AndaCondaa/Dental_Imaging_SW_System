#ifndef SEARCHENROLLMANAGER_H
#define SEARCHENROLLMANAGER_H

#include <QWidget>

namespace Ui {
class SearchEnrollManager;
}

class SearchEnrollManager : public QWidget
{
    Q_OBJECT

public:
    explicit SearchEnrollManager(QWidget *parent = nullptr);
    ~SearchEnrollManager();

private:
    Ui::SearchEnrollManager *ui;
};

#endif // SEARCHENROLLMANAGER_H
