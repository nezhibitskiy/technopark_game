#ifndef PROJECT_TP_PREPARATIONHANDLERS_H
#define PROJECT_TP_PREPARATIONHANDLERS_H

#include "Handler.h"
#include "message.h"
#include "ContextPreparation.h"

namespace Preparation {
    class EventHandler : public BaseEventHandler<EventMessage, ContextPreparation> {
    };

    class AvaivableTeamsHandler : public EventHandler {
    public:
        enum Type {
            AVAILABLE_TEAMS = 11
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == AvaivableTeamsHandler::AVAILABLE_TEAMS) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, ContextPreparation *action) override {
            action->ShowTeams(event->getData());
        }


    };

    class PlayerToTeamHandler : public EventHandler {
    public:
        enum Type {
            PLAYER_ADDED_TO_TEAM = 12
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == PlayerToTeamHandler::PLAYER_ADDED_TO_TEAM) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, ContextPreparation *action) override {
            if (event->getID() == 0)
                action->PlayerTeam(event->getID(), event->getData());
        }


    };
}


#endif //PROJECT_TP_PREPARATIONHANDLERS_H
