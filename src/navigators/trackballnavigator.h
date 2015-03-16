#ifndef TRACKBALLNAVIGATOR_H
#define TRACKBALLNAVIGATOR_H
#include "../core/navigator.h"

class TrackballNavigator : public Navigator
{
public:
    explicit TrackballNavigator(Camera *camera);
    ~TrackballNavigator();
};

#endif // TRACKBALLNAVIGATOR_H
