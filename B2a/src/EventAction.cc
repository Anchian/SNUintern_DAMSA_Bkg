#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include <iostream>

// 진행률 바 표시 함수
void ShowProgressBar(G4int currentEventID, G4int totalEvents) 
{
    const int barWidth = 50;  // 진행률 바의 너비
    G4double progress = double(currentEventID + 1) / totalEvents;
    int pos = barWidth * progress;

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();

    if (currentEventID + 1 == totalEvents) {
        std::cout << std::endl;  // 모든 이벤트가 끝났을 때 줄바꿈
    }
}

// EventAction 클래스 정의
EventAction::EventAction() : G4UserEventAction() {}

EventAction::~EventAction() {}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // 총 이벤트 수와 현재 이벤트 ID 가져오기
    G4int totalEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    G4int currentEventID = event->GetEventID();

    // 진행률 바를 터미널에 표시
    ShowProgressBar(currentEventID, totalEvents);
}