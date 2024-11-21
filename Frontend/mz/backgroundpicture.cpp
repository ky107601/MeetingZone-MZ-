#include "backgroundpicture.h"

backgroundPicture::backgroundPicture()
{
    picture.push_back(":/resources/default_background.jpg");
    picture.push_back(":/resources/basketball.jpg");
    picture.push_back(":/resources/penguin.png");

    cur = 1;
}

string backgroundPicture::getNextPicture()
{
    if(cur >= picture.size())
        cur = 0;

    return picture[cur++];
}
