#include "ColorComboBox.h"

#include <QColorDialog>

// ColorComboBox

ColorComboBox::ColorComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addColor(Qt::transparent, "Custom...");
    for (auto colorName : QColor::colorNames())
        addColor(colorName, colorName);
    setCurrentIndex(_index);

    connect(this, QOverload<int>::of(&ColorComboBox::currentIndexChanged), [this] (int index) {
        if (count() - 1 == index) {
            setCurrentIndex(_index);
            QColorDialog dialog(this);
            dialog.setCurrentColor(currentColor());
            if (QDialog::Accepted == dialog.exec())
                setCurrentColor(dialog.selectedColor());
        } else {
            _index = index;
        }
    });
}

void ColorComboBox::setCurrentColor(const QColor &color)
{
    Q_ASSERT(color.isValid());

    auto index = findData(color);
    if (index < 0) {
        index = count() - 1;
        addColor(color);
    }
    setCurrentIndex(index);
}

void ColorComboBox::addColor(const QColor &color, const QString &title)
{
    Q_ASSERT(color.isValid());

    QPixmap pixmap(iconSize());
    pixmap.fill(color);

    insertItem(count() - 1, pixmap, title.isEmpty() ? color.name() : title, QVariant::fromValue(color));
}
