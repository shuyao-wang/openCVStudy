#ifndef MIGAUSSIANBLUR_H
#define MIGAUSSIANBLUR_H

#include <mibase.h>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QSlider>

class MIGaussianBlur : public MIBase
{
public:
    MIGaussianBlur();
    QVBoxLayout *vBoxLayout;
    QHBoxLayout *hBoxLayout;
    QWidget *subPage;
    QLineEdit *imagePath;

    QPushButton *button;
    QLabel *label;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;

    QLabel *imageLabel_src;
    QLabel *imageLabel_dst;
    QCheckBox *imShowChechBox;
    QImage image;
    QLabel *label_src;
    QLabel *label_dst;
    QHBoxLayout *hBoxLayoutSlider;
    QSlider *horizontalSlider;
    QLabel *labelSlider;

public slots:
    void onSubmitClicked();
    void selectFile();
};

#endif // MIGAUSSIANBLUR_H
