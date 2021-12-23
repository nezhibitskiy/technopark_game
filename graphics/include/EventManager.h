//
// Created by ivan_lemon on 12/2/21.
//

#ifndef TEST_EVENTMANAGER_H
#define TEST_EVENTMANAGER_H


#include <vector>


#include "DrawHandlers.h"
#include "PreparationHandlers.h"

namespace Draw {

    class EventManager {
    public:
        EventManager() {

            moveHandler = new MoveHandler;
            mapHandler = new MapHandler;
            zoneHandler = new ZoneHandler;
            objectHandler = new ObjectHandler;
            healthHandler = new HealthHandler;
            deleteHandler = new DeleteHandler;
            playerHandler = new PlayerHandler;
            timeHandler = new TimeHandler;
            killsHandler = new KillsHandler;

            Register(moveHandler);
            Register(mapHandler);
            Register(zoneHandler);
            Register(objectHandler);
            Register(healthHandler);
            Register(deleteHandler);
            Register(playerHandler);
            Register(timeHandler);
            Register(killsHandler);
        }

        ~EventManager(){
            delete moveHandler;
            delete mapHandler;
            delete zoneHandler;
            delete objectHandler;
            delete deleteHandler;
            delete healthHandler;
            delete timeHandler;
            delete playerHandler;
            delete killsHandler;



        }
        void EventHandle(EventMessage *event, DrawMap *map) {
            for (auto i: mHandlers) {
                if (i->CanHandle(event)) {
                    i->Handler(event, map);
                }
            }
        }



        void Register(EventHandler *handler) {
            mHandlers.push_back(handler);
        }

    private:
        MoveHandler *moveHandler;
        MapHandler *mapHandler;
        ZoneHandler *zoneHandler;
        HealthHandler *healthHandler;
        ObjectHandler *objectHandler;
        DeleteHandler *deleteHandler;
        TimeHandler *timeHandler;
        PlayerHandler *playerHandler;
        KillsHandler *killsHandler;

        std::vector<EventHandler *> mHandlers;
    };

}






namespace Preparation {

    class EventManager {
    public:
        EventManager() {
            avaivableTeamsHandler = new AvaivableTeamsHandler;
            playerToTeamHandler = new PlayerToTeamHandler;
            Register(avaivableTeamsHandler);
            Register(playerToTeamHandler);
        }

        ~EventManager(){
            delete avaivableTeamsHandler;
            delete playerToTeamHandler;



        }
        void EventHandle(EventMessage *event, ContextPreparation *action) {
            for (auto i: mHandlers) {
                if (i->CanHandle(event)) {
                    i->Handler(event, action);
                }
            }
        }



        void Register(EventHandler *handler) {
            mHandlers.push_back(handler);
        }

    private:
        AvaivableTeamsHandler *avaivableTeamsHandler;
        PlayerToTeamHandler *playerToTeamHandler;
        std::vector<EventHandler *> mHandlers;
    };

}



#endif //TEST_EVENTMANAGER_H
