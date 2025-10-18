#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QVBoxLayout>

class Calculator : public QMainWindow {
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();
    void decimalClicked();

private:
    QLineEdit *display;
    QString currentValue;
    QString pendingOperator;
    double leftOperand;
    bool waitingForOperand;

    QPushButton* createButton(const QString &text, const char *member);
    void calculate(double rightOperand, const QString &op);
};

#endif