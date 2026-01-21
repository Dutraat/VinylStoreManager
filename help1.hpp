#ifndef _HELP1_HPP
#define _HELP1_HPP

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class DebugMenu;

namespace Ui {
class Help1;
}
class Help1 : public QDialog
{
    Q_OBJECT

public:
    explicit Help1(QWidget *parent = nullptr);
    ~Help1();

private:
    void setupUI();
    Ui::Help1 *ui;
private slots:
    void on_pushButton_Debug_clicked();
};

#endif
