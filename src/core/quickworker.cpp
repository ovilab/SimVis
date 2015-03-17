#include "quickworker.h"
#include "../renderables/billboards/billboards.h"
#include "../renderables/points/points.h"

QuickWorker::QuickWorker()
{

}

QuickWorker::~QuickWorker()
{

}

void QuickWorker::synchronizeRenderer(Renderable *renderableObject)
{
    Billboards* billboards = qobject_cast<Billboards*>(renderableObject);
    if(billboards) {
        billboards->setPositions(m_positions);
        return;
    }

    Points* points = qobject_cast<Points*>(renderableObject);
    if(points) {
        points->setPositions(m_positions);
        return;
    }
}

