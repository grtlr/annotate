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
        Files (QObject *parent=0) : QObject(parent) {}

        Files (QObject *parent, const QString & folder, const QString & target) : QObject(parent)
        {
            _wfolder = QDir(folder);
            _lfolder = QDir(target);
            QStringList filters;
            filters << "*.png" << "*.bmp";
            _wfolder.setNameFilters(filters);

            for (auto e : _wfolder.entryList())
                _files << _wfolder.absolutePath() + "/" + e;

            _current_file = 0;
        }

        void mkLabelDir()
        {
            _wfolder.mkdir(_lfolder.absolutePath());
        }

        QStringList getImages() const
        {
            return _files;
        }

    public slots:

        void nextFile()
        {
            if (_current_file < _files.size())
            {
                QString f = _files.at(_current_file);
                cv::Mat image = cv::imread(f.toStdString());
                this->fileChanged(image, f, makeLabelPath(f));
                _current_file++;
            }
        }


    signals:

        void fileChanged(const cv::Mat & img, const QString & file, const QString & target);

    private:

        QString makeLabelPath(const QString & original)
        {
            QFileInfo fi(original);
            QDir baseDir = fi.absoluteDir();
            baseDir.cd(_lfolder.absolutePath());
            QString fn = fi.completeBaseName();
            QString ext = fi.completeSuffix();
            return baseDir.absolutePath() + "/" + fn + _lsuff + "." + ext;
        }

        size_t _current_file;

        QDir        _wfolder;
        QDir        _lfolder;
        QString     _lsuff   = "_GT";
        QStringList _files;
};

#endif // LOADFILES_H
