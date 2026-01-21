#ifndef REGISTRO_HPP
#define REGISTRO_HPP
#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class registro; }
QT_END_NAMESPACE
class registro : public QMainWindow
{
    Q_OBJECT
public:
    registro(QWidget *parent = nullptr);
    ~registro();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked(); //Debug
    void on_backButton_clicked();
private:
    Ui::registro *ui;
    bool openDatabase();
};
#endif // REGISTRO_HPP
