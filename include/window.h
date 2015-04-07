#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QPushButton>

#include <opencv2/core/core.hpp>

#include <set>

#include "label.h"
#include "loadfiles.h"
#include "imagewidget.h"

class QString;

class AnnotationWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit AnnotationWindow(QWidget *parent, QString file, QString target, int num_superpixel = 400)
            : _current(file), _target(target)
        {
            _image_widget = new ImageWidget();
            _image_widget->setNumSuperpixels(num_superpixel);
            this->setCentralWidget(_image_widget);

            QObject::connect(this, SIGNAL(currentLabelChanged(const Label &)),
                    _image_widget, SLOT(setCurrentLabel(const Label &)));

            QObject::connect(this, SIGNAL(saveButtonClicked(const std::string &)),
                    _image_widget, SLOT(writeToFile(const std::string &)));

            this->setWindowTitle("annotate - " + file);
            QToolBar * bar = new QToolBar();
            this->addToolBar(bar);

            _combobox = new QComboBox();
            bar->addWidget(_combobox);

            _save_button = new QPushButton();
            _save_button->setText("Save");
            bar->addWidget(_save_button);

            QObject::connect(_combobox, SIGNAL(currentIndexChanged(int)),
                             this,      SLOT(comboboxChanged(int)));

            QObject::connect(_save_button, SIGNAL(clicked()),
                             this,         SLOT(buttonClicked()));

            // This keeps this combobox from gaining focus. Otherwise
            // it would interfere with the other keyboard shortcuts.
            _combobox->setFocusPolicy(Qt::NoFocus);
            _save_button->setFocusPolicy(Qt::NoFocus);
           
        }

        void setLabels(std::set<Label> & labels)
        {
            for (auto l : labels)
            {
                QPixmap ip(10, 10);
                ip.fill(qRgb(l.red, l.green, l.blue));
                _combobox->addItem(QIcon(ip), QString(l.name.c_str()), QVariant::fromValue(l));
            }
        }

    public slots:

        void setImage(const cv::Mat & img, const QString & file, const QString & target)
        {
            this->setWindowTitle("annotate - " + file);
            _current = file;
            _target = target;
            _image_widget->setImage(img);
        }

    signals:

        void currentLabelChanged(const Label & l);
        void saveButtonClicked(const std::string & s);
        void nextImageClicked();

    private slots:

        void comboboxChanged(int index)
        {
            currentLabelChanged(_combobox->currentData().value<Label>());
        }

        void buttonClicked()
        {
            saveButtonClicked(_target.toStdString());
            nextImageClicked();    
        }

    private:

        QString _current;
        QString _target;

        QComboBox   * _combobox;
        QPushButton * _save_button;
        ImageWidget * _image_widget;
};

#endif // WINDOW_H
