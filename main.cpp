#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#include "window.h"
#include "imagewidget.h"
#include "image_label.h"
#include "convert.h"
#include "partitioning.h"
#include "parse_config.h"
#include "loadfiles.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{

    // setup Qt application
    QApplication app(argc, argv);

    Files * fs;
    if (argc == 2)
    {
        fs = new Files(0,argv[1]);
    } else {
        QFileDialog dlg( NULL, "Select Folder");
        dlg.setFileMode( QFileDialog::Directory );
        if (dlg.exec())
                fs = new Files(0,dlg.selectedFiles().at(0).toStdString());
        else
            return 0;
    }

    fs->mkLabelDir();
    
    AnnotationWindow window(0, fs->getImages().at(0), "World");
    
    QObject::connect(fs, SIGNAL(fileChanged(const cv::Mat &, const QString &, const QString &)),
            &window, SLOT(setImage(const cv::Mat &, const QString &, const QString &)));
    
    QObject::connect(&window, SIGNAL(nextImageClicked()),
            fs, SLOT(nextFile()));

    fs->nextFile();
    // read labels
    std::set<Label> labels = readLabelsFromFile("labels.txt");
    window.setLabels(labels);

    window.show();

    return app.exec();
}

