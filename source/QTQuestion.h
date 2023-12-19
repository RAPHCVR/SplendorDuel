#ifndef QTQUESTION_H
#define QTQUESTION_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>

class CustomDialog : public QDialog {
    Q_OBJECT

public:
    enum class EmitMode { Index, Label }; // Added mode enumeration

    explicit CustomDialog(QWidget *parent = nullptr, EmitMode mode = EmitMode::Index);
    void setTitle(const QString &title);
    void setContent(const QString &content);
    void addButton(const QString &label, int role);

    signals:
        void buttonClicked(int index);        // Signal for index
    void buttonClickedLabel(const QString &label); // Signal for label

    private slots:
        void handleButtonClicked();

private:
    QVBoxLayout *layout;
    QLabel *contentLabel;
    std::vector<QPushButton*> buttons;
    EmitMode emitMode; // Added member to store the mode
};

#endif // QTQUESTION_H
