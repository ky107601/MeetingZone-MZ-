#ifndef BACKGROUNDPICTURE_H
#define BACKGROUNDPICTURE_H

#include <QObject>
#include <QWidget>
#include <QIcon>

#include <string>
#include <vector>

using namespace std;

class backgroundPicture : public QWidget
{
    Q_OBJECT
public:
    backgroundPicture();

    string getNextPicture();
    void addPicture(string path);

private:
    vector<string> picture; //imge path storage
    int cur; //current img index

};


#endif // BACKGROUNDPICTURE_H
