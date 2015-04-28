#ifndef LOADFILES_H
#define LOADFILES_H

#include <opencv2/highgui.hpp>

#include <QObject>
#include <QString>
#include <QDir>

#include <vector>

class Files : public QObject
{
    Q_OBJECT

public:
    Files( QObject * parent = 0 ) : QObject( parent ) {}

    Files( const QString & folder, const QString & target, QObject * parent = 0 ) : QObject( parent )
    {
        _input_folder = QDir( folder );
        _output_folder = QDir( target );
        QStringList filters;
        filters << "*.jpeg" << "*.jpg" << "*.png" << "*.bmp";
        _input_folder.setNameFilters( filters );

        for ( auto e : _input_folder.entryList() )
            _files << _input_folder.absolutePath() + "/" + e;

        _current_file = 0;
    }

    void makeLabelDir()
    {
        _input_folder.mkdir( _output_folder.absolutePath() );
    }

    QStringList getImages() const
    {
        return _files;
    }

public slots:

    void nextFile()
    {
        if ( _current_file < _files.size() )
        {
            QString f = _files.at( _current_file );
            cv::Mat image = cv::imread( f.toStdString() );
            this->fileChanged( image, f, makeLabelPath( f ) );
            _current_file++;
        }
    }

signals:

    void fileChanged( const cv::Mat &, const QString &, const QString & );

private:

    QString makeLabelPath( const QString & original )
    {
        QFileInfo fi( original );
        QDir baseDir = fi.absoluteDir();
        baseDir.cd( _output_folder.absolutePath() );
        QString fn = fi.completeBaseName();
        QString ext = fi.completeSuffix();
        return baseDir.absolutePath() + "/" + fn + _label_suffix + "." + ext;
    }

    size_t _current_file;

    QDir        _input_folder;
    QDir        _output_folder;
    QString     _label_suffix = "_GT";
    QStringList _files;
};

#endif // LOADFILES_H
