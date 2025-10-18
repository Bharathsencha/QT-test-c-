#include "calculator.h"
#include <QApplication>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), leftOperand(0.0), waitingForOperand(true) {
    
    setWindowTitle("Calculator");
    setFixedSize(300, 400);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Display
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);
    QFont font = display->font();
    font.setPointSize(20);
    display->setFont(font);
    display->setStyleSheet("QLineEdit { padding: 10px; background-color: #f0f0f0; }");
    mainLayout->addWidget(display);

    // Buttons grid
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(5);

    // Create buttons
    QString buttons[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", ".", "=", "+"}
    };

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            QString text = buttons[row][col];
            QPushButton *btn;

            if (text == "=") {
                btn = createButton(text, SLOT(equalClicked()));
            } else if (text == "+" || text == "-" || text == "*" || text == "/") {
                btn = createButton(text, SLOT(operatorClicked()));
                btn->setStyleSheet("QPushButton { background-color: #ff9500; color: white; }");
            } else if (text == ".") {
                btn = createButton(text, SLOT(decimalClicked()));
            } else {
                btn = createButton(text, SLOT(digitClicked()));
            }

            if (text == "0") {
                gridLayout->addWidget(btn, row, col, 1, 2);
            } else if (text != "." || row != 3) {
                gridLayout->addWidget(btn, row, col);
            }
        }
    }

    // Clear button
    QPushButton *clearBtn = createButton("C", SLOT(clearClicked()));
    clearBtn->setStyleSheet("QPushButton { background-color: #d4d4d2; }");
    gridLayout->addWidget(clearBtn, 4, 0, 1, 4);

    mainLayout->addLayout(gridLayout);
}

Calculator::~Calculator() {}

QPushButton* Calculator::createButton(const QString &text, const char *member) {
    QPushButton *btn = new QPushButton(text);
    btn->setMinimumSize(60, 60);
    QFont font = btn->font();
    font.setPointSize(16);
    btn->setFont(font);
    btn->setStyleSheet("QPushButton { background-color: #e0e0e0; border: 1px solid #999; border-radius: 5px; }"
                       "QPushButton:pressed { background-color: #d0d0d0; }");
    connect(btn, SIGNAL(clicked()), this, member);
    return btn;
}

void Calculator::digitClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString digit = btn->text();

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }

    if (display->text() == "0" && digit != "0") {
        display->setText(digit);
    } else {
        display->setText(display->text() + digit);
    }
}

void Calculator::decimalClicked() {
    if (waitingForOperand) {
        display->setText("0.");
        waitingForOperand = false;
        return;
    }

    if (!display->text().contains('.')) {
        display->setText(display->text() + ".");
    }
}

void Calculator::operatorClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString op = btn->text();
    double operand = display->text().toDouble();

    if (!pendingOperator.isEmpty()) {
        calculate(operand, pendingOperator);
        display->setText(QString::number(leftOperand));
    } else {
        leftOperand = operand;
    }

    pendingOperator = op;
    waitingForOperand = true;
}

void Calculator::equalClicked() {
    double operand = display->text().toDouble();

    if (!pendingOperator.isEmpty()) {
        calculate(operand, pendingOperator);
        pendingOperator.clear();
        display->setText(QString::number(leftOperand));
    }

    waitingForOperand = true;
}

void Calculator::clearClicked() {
    display->setText("0");
    leftOperand = 0.0;
    pendingOperator.clear();
    waitingForOperand = true;
}

void Calculator::calculate(double rightOperand, const QString &op) {
    if (op == "+") {
        leftOperand += rightOperand;
    } else if (op == "-") {
        leftOperand -= rightOperand;
    } else if (op == "*") {
        leftOperand *= rightOperand;
    } else if (op == "/") {
        if (rightOperand != 0.0) {
            leftOperand /= rightOperand;
        }
    }
}
