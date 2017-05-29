#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QComboBox>

// ColorComboBox
class ColorComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit ColorComboBox(QWidget *parent = Q_NULLPTR);

    QColor currentColor() const { return currentData().value<QColor>(); }
    void setCurrentColor(const QColor &color);

private:
    void addColor(const QColor &color, const QString &title = QString());

    int _index = 0;
};

#endif // COLORCOMBOBOX_H
