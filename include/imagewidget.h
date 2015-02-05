#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QObject>
#include <QMouseEvent>

#include <opencv2/core/core.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgproc.hpp>

#include "convert.h"
#include "label.h"
#include "partitioning.h"
#include "image_label.h"

class ImageWidget : public QLabel
{
    Q_OBJECT

    public:
        explicit ImageWidget(QWidget *parent = 0) : QLabel(parent)
        {
            this->setMouseTracking(true);
            this->setFocusPolicy(Qt::StrongFocus);
            _ilabels = 0;
        }

        QSize sizeHint() const { return _image.size(); }
        QSize minimumSizeHint() const { return _image.size(); }

    public slots:

        void setImage(const cv::Mat & image)
        {
            using namespace cv;
            using namespace cv::ximgproc;

            _image = bgrToQImage(image);
            this->setFixedSize(_image.size());
            this->setFocus(Qt::OtherFocusReason);

            Ptr<SuperpixelSEEDS> seeds;
            int num_superpixels = 400;
            int num_levels = 4;
            int prior = 2;
            int num_histogram_bins = 5;
            int num_iterations = 4;

            seeds = createSuperpixelSEEDS(image.size().width, image.size().height,
                    image.channels(), num_superpixels,
                    num_levels, prior, num_histogram_bins, false);

            Mat img_conv;
            cvtColor(image, img_conv, COLOR_BGR2HSV);
            seeds->iterate(img_conv, num_iterations);

            Mat ls, cs;
            seeds->getLabels(ls);
            seeds->getLabelContourMask(cs);

            _partitioning.setIds(ls);
            _partitioning.setContours(cs);
            _contours = contoursToQImage(_partitioning.getContours(), _contour_color);

            if (_ilabels != 0)
                delete _ilabels;

            _ilabels = new ImageLabel(_image.height(), _image.width());


            QObject::connect(this, SIGNAL(changeLabel(const cv::Mat &, const Label &)),
					_ilabels, SLOT(labelArea(const cv::Mat &, const Label &)));

            QObject::connect(_ilabels, SIGNAL(labelChanged(const cv::Mat &)),
                    this, SLOT(setLabels(const cv::Mat &)));

            _ilabels->update();
            repaint();
        }

        void writeToFile(const std::string & f)
        {
            _ilabels->writeToFile(f);
        }

        void setContours(const cv::Mat & contours)
        {
            _contours = contoursToQImage(contours, _contour_color);
            repaint();
        }

        void setActive(const cv::Mat & active)
        {
            _active = contoursToQImage(active, _active_color);
            repaint();
        }

        void setLabels(const cv::Mat & labels)
        {
            _labels = bgrToQImage(labels);
            this->maskQImage(_labels, qRgb(0,0,0));
            repaint();
        }

        void setCurrentLabel(const Label & l)
        {
            _current_label = l;
        }

    signals:

        void mousePositionChanged(int x, int y);
        void changeLabel(const cv::Mat & mask, const Label & l);

    protected:

        void keyPressEvent(QKeyEvent * event)
        {
            if (event->key() == Qt::Key_Space)
            {
                _contours_visible = !_contours_visible;
                repaint();
            }
        }

        void mousePressEvent(QMouseEvent * event)
        {
            cv::Mat mask = _partitioning.getPartition(event->x(), event->y());

            if (event->buttons() == Qt::LeftButton)
                this->changeLabel(mask, _current_label);
            else if (event->buttons() == Qt::RightButton)
                this->changeLabel(mask, _background_label);

            repaint();
        }

        void mouseMoveEvent(QMouseEvent * event)
        {
            int x = event->x();
            int y = event->y();
            if ( x < 0 || x >= _image.width() || y < 0 || y >= _image.height() )
                return;

            cv::Mat mask = _partitioning.getPartition(x, y);
            this->setActive(mask);

            if (event->buttons() == Qt::LeftButton)
                this->changeLabel(mask, _current_label);
            else if (event->buttons() == Qt::RightButton)
                this->changeLabel(mask, _background_label);

            repaint();
        }

        void paintEvent(QPaintEvent * event)
        {
            QPainter painter(this);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawImage(QPoint(0,0), _image);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

            if (_contours_visible)
                painter.drawImage(QPoint(0,0), _contours);

            painter.setOpacity(0.5);
            painter.drawImage(QPoint(0,0), _labels);
            painter.drawImage(QPoint(0,0), _active);
            painter.end();
        }

    private:

        void maskQImage(QImage & img, const QRgb & color)
        {
            for (int i = 0; i < img.width(); ++i)
            {
                for (int j = 0; j < img.height(); ++j)
                {
                    if (img.pixel(i,j) == color)
                    {
                        QRgb c = img.pixel(i,j);
                        img.setPixel(i, j, qRgba(qRed(c),qGreen(c),qRed(c),0));
                    }
                }
            }
        }

        QImage _image;
        QImage _contours;
        QImage _active;
        QImage _labels;

        Label        _current_label;
        Label        _background_label = Label{"Background", 0, 0, 0};
        Partitioning _partitioning;
        ImageLabel * _ilabels;

        bool _contours_visible = true;

        const QColor _active_color  = QColor(255,255,255,150);
        const QColor _contour_color = QColor(255,255,255,150);
};

#endif // IMAGE_WIDGET_H
