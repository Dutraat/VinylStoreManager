#ifndef _HELP2_HPP
#define _HELP2_HPP
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
class DebugMenu;
namespace Ui {
class Help2;
}
class Help2 : public QDialog
{
    Q_OBJECT
public:
    explicit Help2(QWidget *parent = nullptr);
    ~Help2();
private:
    void setupUI();
    Ui::Help2 *ui;
private slots:
    void on_pushButton_Debug_clicked();
};
#endif
