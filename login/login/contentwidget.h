// contentwidget.h
#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class ContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContentWidget(const QString &title, QWidget *parent = nullptr);
    ~ContentWidget();

    void setContentWidget(QWidget *widget);

private:
    QLabel *titleLabel;
    QVBoxLayout *mainLayout;
};

#endif // CONTENTWIDGET_H
