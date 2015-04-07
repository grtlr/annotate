#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>
#include <QCommandLineParser>

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

    QCommandLineParser parser;
    parser.setApplicationDescription("Annotation of images using SEEDS superpixels.");
    parser.addHelpOption();
    parser.addPositionalArgument("labels", "File containing the labels");
    parser.addPositionalArgument("indir", "Folder containing the images");
    parser.addPositionalArgument("outdir", "Destination directory");

    QCommandLineOption numberSuperpixelOption("n", "Number of superpixels", "400");
    parser.addOption(numberSuperpixelOption);
    parser.process(app);

    const QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.size() != 3) {
        parser.showHelp();
    }

    fs = new Files(0, positionalArguments.at(1), positionalArguments.at(2));

    // TODO Folder exists
    fs->mkLabelDir();

    AnnotationWindow window(0, fs->getImages().at(0), "World", parser.value("n").toInt());

    QObject::connect(fs, SIGNAL(fileChanged(const cv::Mat &, const QString &, const QString &)),
            &window, SLOT(setImage(const cv::Mat &, const QString &, const QString &)));

    QObject::connect(&window, SIGNAL(nextImageClicked()),
            fs, SLOT(nextFile()));

    fs->nextFile();
    // read labels
    std::set<Label> labels = readLabelsFromFile(positionalArguments.at(0).toStdString());
    window.setLabels(labels);

    window.show();

    return app.exec();
}

