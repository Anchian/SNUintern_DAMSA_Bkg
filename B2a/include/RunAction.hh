#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "SteppingActions.hh"


class RunAction : public G4UserRunAction
{
    public:
        RunAction();
        virtual ~RunAction();

        //virtual void BeginOfRunAction(const G4Run* run);
        virtual void EndOfRunAction(const G4Run* run);
        virtual void BeginOfRunAction(const G4Run* run);
        
    private:
        SteppingAction* fSteppingAction;
        // stepping action.hh에서 변수 설정한 걸로 steppingAction.cc에서 데이터를 쌓고 여기서 stepAction.hh의 객체 타입을 불러옴. 
};

#endif