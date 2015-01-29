#include <QDialog>
#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ximgproc.hpp>

#include <iostream>

#include "window.h"
#include "imagewidget.h"
#include "image_label.h"
#include "convert.h"
#include "partitioning.h"

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

int main(int argc, char **argv)
{

    // setup Qt application
    QApplication app(argc, argv);
    AnnotationWindow window;

    QFileInfo finfo;

    if (argc == 2)
    {
        QString fn(argv[1]);
        finfo = QFileInfo(fn);
    } else {
        QFileDialog dlg( NULL, "Open Image");
        dlg.setFileMode( QFileDialog::ExistingFile );
        if (dlg.exec())
                finfo = dlg.selectedFiles().at(0);
        else
            return 0;
    }

    cv::Mat image = cv::imread(finfo.absoluteFilePath().toStdString());

    // extract superpixels
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

    Partitioning p;
    p.setIds(ls);
    p.setContours(cs);

    ImageWidget w; 
    w.setPartitioning(&p);
    w.setImage(bgrToQImage(image));
    w.setContours(p.getContours());
    window.setCentralWidget(&w);

    QObject::connect(&window, SIGNAL(currentLabelChanged(const Label &)),
                     &w,      SLOT(setCurrentLabel(const Label &))); 

    ImageLabel l(image.rows, image.cols);
    
    QObject::connect(&w, SIGNAL(changeLabel(const cv::Mat &, const Label &)),
                     &l, SLOT(labelArea(const cv::Mat &, const Label &)));

    QObject::connect(&l, SIGNAL(labelChanged(const cv::Mat &)),
                     &w, SLOT(setLabels(const cv::Mat &)));

    QObject::connect(&window, SIGNAL(saveButtonClicked(const std::string &)),
                     &l,      SLOT(writeToFile(const std::string &)));

    // create labels
    std::set<Label> labels = { {"Instrument", 0, 128 ,255}
        , {"Blood", 0, 255, 128}
        , {"Background", 0, 0, 0}};
    window.setLabels(labels);

    window.show();

    return app.exec();
}

