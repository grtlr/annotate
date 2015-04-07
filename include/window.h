#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
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
    explicit AnnotationWindow( QString file, QString target, int num_superpixel, QWidget * parent = 0 )
        : _current( file ), _target( target )
    {
        _image_widget = new ImageWidget();
        _image_widget->setNumSuperpixels( num_superpixel );
        this->setCentralWidget( _image_widget );

        QObject::connect( this, SIGNAL( currentLabelChanged( const Label & ) ),
                          _image_widget, SLOT( setCurrentLabel( const Label & ) ) );

        QObject::connect( this, SIGNAL( saveButtonClicked( const std::string & ) ),
                          _image_widget, SLOT( writeToFile( const std::string & ) ) );

        this->setWindowTitle( "annotate - " + file );
        QToolBar * bar = new QToolBar();
        this->addToolBar( bar );

        _save_button = new QPushButton();
        _save_button->setText( "Save" );
        bar->addWidget( _save_button );

        // setup the widget for the labels
        QDockWidget * dockWidget = new QDockWidget( "Labels" );
        dockWidget->setAllowedAreas( Qt::LeftDockWidgetArea );
        dockWidget->setFeatures( QDockWidget::NoDockWidgetFeatures );

        // setup the list with the labels
        _label_list = new QListWidget();
        dockWidget->setWidget( _label_list );
        this->addDockWidget( Qt::RightDockWidgetArea, dockWidget );

        QObject::connect( _label_list, SIGNAL( currentRowChanged( int ) ),
                          this,      SLOT( labelListChanged( int ) ) );

        QObject::connect( _save_button, SIGNAL( clicked() ),
                          this,         SLOT( buttonClicked() ) );

        // This keeps this combobox from gaining focus. Otherwise
        // it would interfere with the other keyboard shortcuts.
        _label_list->setFocusPolicy( Qt::NoFocus );
        _save_button->setFocusPolicy( Qt::NoFocus );

    }

    void setLabels( std::set<Label> & labels )
    {
        for ( auto l : labels )
        {
            QPixmap ip( 10, 10 );
            ip.fill( qRgb( l.red, l.green, l.blue ) );
            QListWidgetItem * item = new QListWidgetItem( QIcon( ip ), QString( l.name.c_str() ) );
            item->setData(Qt::UserRole, QVariant::fromValue(l));
            _label_list->addItem(item);
        }
    }

public slots:

    void setImage( const cv::Mat & img, const QString & file, const QString & target )
    {
        this->setWindowTitle( "annotate - " + file );
        _current = file;
        _target = target;
        _image_widget->setImage( img );
    }

signals:

    void currentLabelChanged( const Label & l );
    void saveButtonClicked( const std::string & s );
    void nextImageClicked();

private slots:

    void labelListChanged( int index )
    {
        currentLabelChanged( _label_list->currentItem()->data( Qt::UserRole ).value<Label>() );
    }

    void buttonClicked()
    {
        saveButtonClicked( _target.toStdString() );
        nextImageClicked();
    }

private:

    QString _current;
    QString _target;

    QListWidget  *  _label_list;
    QPushButton * _save_button;
    ImageWidget * _image_widget;
};

#endif // WINDOW_H
