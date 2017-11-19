#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main (int argc, char **argv)
{
    Mat image, image_gray, aa;

    //读取一张图片
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR );
    if (argc != 2 || !image.data)
    {
        cout << "No image data\n";
        return -1;
    }

    //将图片转为灰度图片
    cvtColor(image, image_gray, CV_RGB2GRAY);

    //创建并显示窗口
    namedWindow("image", CV_WINDOW_AUTOSIZE);
    namedWindow("image gray", CV_WINDOW_AUTOSIZE);
    imshow("image", image);
    imshow("image gray", image_gray);

    waitKey(0);
    return 0;
}
