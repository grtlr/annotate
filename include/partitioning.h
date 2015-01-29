#ifndef PARTITIONING_H
#define PARTITIONING_H

#include <QObject>

#include <opencv2/core/core.hpp>

class Partitioning : public QObject
{
    Q_OBJECT

    public:
        Partitioning(QObject * parent = 0) : QObject(parent) {}

        void setIds(const cv::Mat & ids) { _ids = ids; }
        void setContours(const cv::Mat & contours) { _contours = contours; }

        cv::Mat getContours() const { return _contours; } 

        cv::Mat getPartition(int x, int y) const
        {
            int tmp_id = _ids.at<int>(y,x);
            cv::Mat res(_ids.rows, _ids.cols, CV_8UC1, cv::Scalar::all(0));
            for (int i = 0; i < _ids.rows; ++i)
            {
                for (int j = 0; j < _ids.cols; ++j)
                {
                    if (_ids.at<int>(i,j) == tmp_id)
                        res.at<uchar>(i,j) = 255;
                }
            }
            return res;
        }

    private:
        cv::Mat _ids;
        cv::Mat _contours;
};

#endif // PARTITIONING_H
