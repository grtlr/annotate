#ifndef IMAGE_LABEL_H
#define IMAGE_LABEL_H

#include <QObject>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

#include "label.h"

class ImageLabel : public QObject
{
    Q_OBJECT

    public:
        explicit ImageLabel(size_t rows, size_t cols)
        {
            _labels = cv::Mat(rows, cols, CV_8UC3, cv::Scalar::all(0));
        }

        cv::Mat getLabeledImage() const { return _labels; }

    public slots:

        void writeToFile(const std::string & file)
        {
            cv::imwrite(file, _labels);
        }
        
        void labelArea(const cv::Mat & mask, const Label & label)
        {
            _labels.setTo(cv::Scalar(label.blue, label.green, label.red), mask);
            this->labelChanged(_labels);
        }

    signals:

        void labelChanged(const cv::Mat & labels) const;

    private:
        cv::Mat _labels;
};

#endif // IMAGE_LABEL_H
