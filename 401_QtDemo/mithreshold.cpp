#include "mithreshold.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QApplication>
#include <QSizePolicy>

#define MINAME "阈值化Threshold"

MIThreshold::MIThreshold()
{
    this->setModelItemName(MINAME);

    sliderMaxIterationNum = 255;
    thresholdTypeMaxVal = 4;
    subPage = new QWidget();


    vBoxLayout = new QVBoxLayout(subPage);
    hBoxLayout = new QHBoxLayout();
    label = new QLabel(subPage);
    label->setText("待处理图片:");
    hBoxLayout->addWidget(label);
    imagePath = new QLineEdit(subPage);
    imagePath->setText("../assert/image22.jpg");
    hBoxLayout->addWidget(imagePath);
    button = new QPushButton(subPage);
    button->setText("...");
    connect(button, &QPushButton::clicked, this, &MIThreshold::selectFile);
    hBoxLayout->addWidget(button);

    vBoxLayout->addLayout(hBoxLayout);

    imShowChechBox = new QCheckBox(subPage);
    imShowChechBox->setText("使用OpenCV的imshow");
    vBoxLayout->addWidget(imShowChechBox);


    thresholdTypeLayout = new QHBoxLayout();
    thresholdTypeLabel = new QLabel(subPage);
    thresholdTypeLabel->setText("类型:" + QString("%1").arg(thresholdTypeMaxVal));
    thresholdTypeLayout->addWidget(thresholdTypeLabel);
    thresholdTypeSlider = new QSlider(subPage);
    thresholdTypeSlider->setOrientation(Qt::Horizontal);
    thresholdTypeSlider->setMinimum(0);
    thresholdTypeSlider->setMaximum(thresholdTypeMaxVal);
    thresholdTypeSlider->setSingleStep(1);
    thresholdTypeSlider->setTickPosition(QSlider::TicksAbove);
    thresholdTypeSlider->setValue(thresholdTypeMaxVal);
    connect(thresholdTypeSlider, &QSlider::valueChanged, this, &MIThreshold::onSubmitClicked);
    thresholdTypeLayout->addWidget(thresholdTypeSlider);
    vBoxLayout->addLayout(thresholdTypeLayout);


    hBoxLayoutSlider = new QHBoxLayout();
    labelSlider = new QLabel(subPage);
    labelSlider->setText("内核值:" + QString("%1").arg(sliderMaxIterationNum));
    hBoxLayoutSlider->addWidget(labelSlider);
    horizontalSlider = new QSlider(subPage);
    horizontalSlider->setOrientation(Qt::Horizontal);
    horizontalSlider->setMinimum(0);
    horizontalSlider->setMaximum(sliderMaxIterationNum);
    horizontalSlider->setSingleStep(1);
    horizontalSlider->setTickPosition(QSlider::TicksAbove);
    horizontalSlider->setValue(sliderMaxIterationNum);
    connect(horizontalSlider, &QSlider::valueChanged, this, &MIThreshold::onSubmitClicked);
    hBoxLayoutSlider->addWidget(horizontalSlider);
    vBoxLayout->addLayout(hBoxLayoutSlider);



    scrollArea = new QScrollArea(subPage);
    scrollArea->setObjectName(QStringLiteral("scrollArea"));
    scrollArea->setWidgetResizable(false);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    scrollAreaWidgetContents->resize(2000,2000);
    scrollArea->setWidget(scrollAreaWidgetContents);


    gridLayoutWidget = new QWidget(scrollAreaWidgetContents);
    gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(1, 1, 1, 1);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));

    label_src = new QLabel(gridLayoutWidget);
    gridLayout->addWidget(label_src, 0, 0, 1, 1);
    image.load(imagePath->text()) ;
    imageLabel_src = new QLabel(gridLayoutWidget);
    imageLabel_src->setText("imageLabel_src");
    imageLabel_src->setPixmap(QPixmap::fromImage(image));
    imageLabel_src->resize(QSize(image.width(),image.height()));
    imageLabel_src->setScaledContents(true);
    label_src->setText(MINAME+QString("【原图】\n width:%1 \n height:%2").arg(image.width()).arg(image.height()));
    gridLayout->addWidget(imageLabel_src, 0, 1, 1, 1);

    label_dst = new QLabel(gridLayoutWidget);
    gridLayout->addWidget(label_dst, 1, 0, 1, 1);
    image.load(imagePath->text()) ;
    imageLabel_dst = new QLabel(gridLayoutWidget);
    imageLabel_dst->setText("imageLabel_dst");
    imageLabel_dst->setPixmap(QPixmap::fromImage(image));
    imageLabel_dst->resize(QSize(image.width(),image.height()));
    imageLabel_dst->setScaledContents(true);
    label_dst->setText(MINAME+QString("【效果图】\n width:%1 \n height:%2").arg(image.width()).arg(image.height()));
    gridLayout->addWidget(imageLabel_dst, 1, 1, 1, 1);

    vBoxLayout->addWidget(scrollArea);

    this->addSubPage(subPage);
    onSubmitClicked();
}

void MIThreshold::selectFile()
{
    qDebug() << "selectFile: " << this->modelItemName;
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog();
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开图片"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    //fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }

    if( fileNames.length() > 0 )
    {
        imagePath->setText(fileNames[0]);
        image.load(fileNames[0]) ;
        imageLabel_src->setPixmap(QPixmap::fromImage(image));
        imageLabel_src->resize(QSize(image.width(),image.height()));
        label_src->setText(MINAME+QString("【原图】\n width:%1 \n height:%2")
                           .arg(image.width()).arg(image.height()));
        scrollAreaWidgetContents->resize(gridLayout->sizeHint());
        onSubmitClicked();
    }
    else
    {
        qDebug() << " 未选择文件 ";
    }
}


void MIThreshold::onSubmitClicked()
{
    QImage qimage;
    Mat out;
    Mat g_grayImage;
    qDebug() << "onSubmitClicked: " << this->modelItemName;
    // 载入原图

    Mat image=imread(imagePath->text().toStdString());
    if( !image.data )
    {
        qDebug() << "onSubmitClicked: 读取图像错误";
        return;
    }

    qDebug() << "image: path=" << imagePath->text();
    qDebug() << "image: rows=" << image.rows;
    qDebug() << "image: cols=" << image.cols;

    //获取自定义核

    qDebug() << "内核值: "+ QString("%1").arg(horizontalSlider->value());
    labelSlider->setText("内核值: "+ QString("%1").arg(horizontalSlider->value()) );
    qDebug() << "类型: "+ QString("%1").arg(thresholdTypeSlider->value());
    thresholdTypeLabel->setText("类型:" + QString("%1").arg(thresholdTypeSlider->value()) );

    //进行操作

    // 存留一份原图的灰度图
    cvtColor( image, g_grayImage, COLOR_RGB2GRAY );

    //调用阈值函数
    threshold(g_grayImage, out, horizontalSlider->value(), 255, thresholdTypeSlider->value());

    //创建窗口
    if( imShowChechBox->isChecked() == true )
    {
        namedWindow( (QString(MINAME)+QString("【原图】")).toStdString() );
        namedWindow( (QString(MINAME)+QString("【效果图】")).toStdString() );
    }

    //显示原图
    if( imShowChechBox->isChecked() == true )
    {
        imshow( (QString(MINAME)+QString("【原图】")).toStdString(), image );
    }
    //显示效果图
    if( imShowChechBox->isChecked() == true )
    {
        imshow( (QString(MINAME)+QString("【效果图】")).toStdString() ,out );
    }


    qimage = cvMat2QImage(image);
    imageLabel_src->setPixmap(QPixmap::fromImage(qimage));  // 将图片显示到label上
    label_src->setText(MINAME+QString("【原图】\n width:%1 \n height:%2")
                       .arg(qimage.width()).arg(qimage.height()));
    qimage = cvMat2QImage(out);
    imageLabel_dst->setPixmap(QPixmap::fromImage(qimage));  // 将图片显示到label上
    label_dst->setText(MINAME+QString("【效果图】\n width:%1 \n height:%2")
                       .arg(qimage.width()).arg(qimage.height()));
}
