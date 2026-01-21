#ifndef FIRSTLOGIN_HPP
#define FIRSTLOGIN_HPP
#include"mainwindow.hpp"

#include <QMainWindow>

namespace Ui { class firstloginUI; }

class DebugMenu;
class firstlogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit firstlogin(QWidget *parent = nullptr);
   virtual ~firstlogin() override;

private slots:
    void on_pushButton_Debug_clicked();
    void on_pushButton_clicked();
    void togglePasswordVisibility();
    void on_backButton_clicked();
    void on_pushButton_Debug_2_clicked();
private:
    Ui::firstloginUI *ui;
    bool openDatabase();
};

#endif // FIRSTLOGINUI_HPP



