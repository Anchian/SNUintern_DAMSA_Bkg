#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    // 이벤트가 끝날 때 호출되는 함수
    virtual void EndOfEventAction(const G4Event* event) override;
};

#endif