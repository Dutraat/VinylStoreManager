#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   // void togglePasswordVisibility();
   // void toggleConfirmPasswordVisibility();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked(); //Debug
    void on_backButton_clicked();
    void on_toolButton_3_clicked();     // Olho da Senha
    void on_toolButton_2_clicked();     // Olho da Confirmação de Senha


private:
    Ui::MainWindow *ui;
    bool openDatabase();
};

#endif // MAINWINDOW_HPP
