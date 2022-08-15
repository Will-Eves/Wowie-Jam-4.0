#define AI_ACTION_NONE 0

#define AI_ACTION_HIDE 1
#define AI_ACTION_SHOW 2

#define AI_ACTION_NEGATIVE_X 3
#define AI_ACTION_POSITIVE_X 4
#define AI_ACTION_NEGATIVE_Y 5
#define AI_ACTION_POSITIVE_Y 6
#define AI_ACTION_NEGATIVE_Z 7
#define AI_ACTION_POSITIVE_Z 8

#define AI_ACTION_SHRINK_X 9
#define AI_ACTION_GROW_X 10
#define AI_ACTION_SHRINK_Y 11
#define AI_ACTION_GROW_Y 12
#define AI_ACTION_SHRINK_Z 13
#define AI_ACTION_GROW_Z 14

#define AI_ACTION_OPEN_DOOR 15
#define AI_ACTION_LOWER_PLATFORM 16
#define AI_ACTION_MOVE_X 17

#define AI_ACTION_TRIGGER_LINE_20 18
#define AI_ACTION_TRIGGER_LINE_21 19
#define AI_ACTION_TRIGGER_LINE_22 20
#define AI_ACTION_TRIGGER_LINE_23 21

#define AI_ACTION_PLATFORM_DESTROY 22

#define AI_ACTION_EPIC_LINE 23
#define AI_ACTION_EPIC_LINE_2 24

namespace Burst {
    namespace AI {
        std::map<unsigned int, std::vector<std::string>> aiText = {
            { AI_ACTION_NONE, { "!" }},

            { AI_ACTION_HIDE, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_SHOW, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},

            { AI_ACTION_NEGATIVE_X, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_POSITIVE_X, { "!" }},
            { AI_ACTION_NEGATIVE_Y, { "!" }},
            { AI_ACTION_POSITIVE_Y, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_NEGATIVE_Z, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_POSITIVE_Z, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},

            { AI_ACTION_SHRINK_X, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_GROW_X, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_SHRINK_Y, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_GROW_Y, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_SHRINK_Z, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},
            { AI_ACTION_GROW_Z, { "res/Audio/Voice Lines/Line_9.wav", "res/Audio/Voice Lines/Line_10.wav", "res/Audio/Voice Lines/Line_12.wav" }},

            { AI_ACTION_OPEN_DOOR, { "res/Audio/Voice Lines/Line_2.wav" }},
            { AI_ACTION_LOWER_PLATFORM, { "res/Audio/Voice Lines/Line_4.wav" }},
            { AI_ACTION_MOVE_X, { "res/Audio/Voice Lines/Line_6.wav" }},

            { AI_ACTION_TRIGGER_LINE_20, { "res/Audio/Voice Lines/Line_20.wav" }},
            { AI_ACTION_TRIGGER_LINE_21, { "res/Audio/Voice Lines/Line_21.wav" }},
            { AI_ACTION_TRIGGER_LINE_22, { "res/Audio/Voice Lines/Line_22.wav" }},
            { AI_ACTION_TRIGGER_LINE_23, { "res/Audio/Voice Lines/Line_23.wav" }},

            { AI_ACTION_PLATFORM_DESTROY, { "res/Audio/Voice Lines/Line_14.wav", "res/Audio/Voice Lines/Line_15.wav", "res/Audio/Voice Lines/Line_16.wav"}},
            
            { AI_ACTION_EPIC_LINE, { "res/Audio/Voice Lines/Line_18.wav" }},
            { AI_ACTION_EPIC_LINE_2, { "res/Audio/Voice Lines/Line_19.wav" }}
        };

        struct ActionList {
            std::vector<unsigned int> actions;
            std::map<unsigned int, bool> actionsUsed;

            ActionList() {
                actions.push_back(AI_ACTION_NONE);
                actionsUsed[AI_ACTION_NONE] = false;
            }

            ActionList(std::vector<unsigned int> list) {
                actions = list;

                for (unsigned int action : actions) {
                    actionsUsed[action] = false;
                }
            }

            unsigned int GetAction() {
                bool resetActions = true;
                for (unsigned int action : actions) {
                    if (!actionsUsed[action]) {
                        resetActions = false;
                        break;
                    }
                }

                if (resetActions) {
                    for (unsigned int action : actions) {
                        actionsUsed[action] = false;
                    }
                }

                unsigned int L = actions.size();

                unsigned int actionIndex = rand() % L;

                while (actionsUsed[actions[actionIndex]]) {
                    actionIndex = rand() % L;
                }

                return actions[actionIndex];
            }
        };
    }
}