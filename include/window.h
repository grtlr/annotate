#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QPushButton>

#include <set>

#include "label.h"

class QString;

class AnnotationWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit AnnotationWindow(QWidget *parent = 0)
        {
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

    signals:

        void currentLabelChanged(const Label & l);
        void saveButtonClicked(const std::string & s);

    private slots:

        void comboboxChanged(int index)
        {
            currentLabelChanged(_combobox->currentData().value<Label>());
        }

        void buttonClicked()
        {
            saveButtonClicked("test.png");
        }

    private:
        QComboBox   * _combobox;
        QPushButton * _save_button;
};

#endif // WINDOW_H
