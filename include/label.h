#ifndef LABEL_H
#define LABEL_H

struct Label
{
    std::string name;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

Q_DECLARE_METATYPE(Label)

inline bool operator<(const Label& a, const Label& b) {
    return a.name < b.name;
}

#endif // LABEL_H
