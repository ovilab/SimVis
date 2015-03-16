#include "quickworker.h"
#include "billboards.h"
#include "points2d.h"
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

    Points2D* points = qobject_cast<Points2D*>(renderableObject);
    if(points) {
        points->setPositions(m_positions);
        return;
    }
}

