#ifndef WIDGET_H
#define WIDGET_H

#include <globel.h>
#include <solve.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent* e);

private:
    Ui::Widget *ui;
    Solve* solve = nullptr;
    QPushButton *start, *exit;

private slots:
    void _on_start_clicked();
    void swap_widgets();
    void __attribute__((noreturn)) _on_exit_clicked();
};

#endif // WIDGET_H
