#pragma once
#ifndef COLORCOMBOX_H
#define COLORCOMBOX_H

#include <QToolButton>

class ColorCombox : public QToolButton
{
    Q_OBJECT

public:
    ColorCombox(QWidget *parent);
    ~ColorCombox();

    void setButtonIcon(const QString &imageFile, QColor color);

	//主菜单
	QMenu* Pmenu;

private:
    QMenu* createColorMenu(const char *slot, const char *slotColorBoard);
    QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);
    QIcon createColorIcon(QColor color);

signals:
    void sigColorChanged(QColor color);

    private slots:
        void OnColorChanged();           // 文本颜色设置
        void OnShowColorBoard();         // 颜色板
};

#endif // COLORCOMBOX_H

