#include "QTQuestion.h"


CustomDialog::CustomDialog(QWidget *parent, EmitMode mode) : QDialog(parent), emitMode(mode) {
    layout = new QVBoxLayout(this);
    contentLabel = new QLabel(this);
    layout->addWidget(contentLabel);
}

void CustomDialog::setTitle(const QString &title) {
    setWindowTitle(title);
}

void CustomDialog::setContent(const QString &content) {
    contentLabel->setText(content);
}

void CustomDialog::addButton(const QString &label, int role) {
    QPushButton *button = new QPushButton(label, this);
    buttons.push_back(button);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, &CustomDialog::handleButtonClicked);
}

void CustomDialog::handleButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    int index = std::find(buttons.begin(), buttons.end(), clickedButton) - buttons.begin();

    if (emitMode == EmitMode::Index) {
        emit buttonClicked(index);
    } else if (emitMode == EmitMode::Label) {
        QString label = clickedButton->text();
        emit buttonClickedLabel(label);
    }
    accept(); // Close the dialog when a button is clicked
}