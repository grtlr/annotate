#ifndef CONVERT_H
#define CONVERT_H

#include <opencv2/core/core.hpp>
#include <QImage>

inline QImage bgrToQImage(const cv::Mat & image)
{
    QImage res(image.cols, image.rows, QImage::Format_ARGB32);
    cv::Vec3b tmp;

    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            tmp = image.at<cv::Vec3b>(i,j);
            res.setPixel(j, i, qRgb(tmp[2], tmp[1], tmp[0]));
        }
    }
    return res;
}

inline QImage contoursToQImage(const cv::Mat & contours, const QColor & color)
{
    QImage res(contours.cols, contours.rows, QImage::Format_ARGB32);
    unsigned char tmp;

    for (int i = 0; i < contours.rows; ++i)
    {
        for (int j = 0; j < contours.cols; ++j)
        {
            tmp = contours.at<unsigned char>(i,j);
            if (tmp == 255)
                res.setPixel(j, i, color.rgba());
            else
                res.setPixel(j, i, QColor(0, 0, 0, 0).rgba());
        }
    }
    return res;
}



#endif // CONVERT_H
