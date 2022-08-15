#pragma once

namespace Burst {
    namespace Time {
        std::map<std::string, std::chrono::steady_clock::time_point> events;

        float programTime = 0.0f;
        float deltaTime = 0.0f;

        void StartEvent(std::string name) {
            events[name] = std::chrono::high_resolution_clock::now();
        }

        float StampEvent(std::string name) {
            if (!events.contains(name)) return 0.0f;
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(current - events[name]).count();
            events[name] = current;
            return delta / 1000000000.0f;
        }

        float SnapEvent(std::string name) {
            if (!events.contains(name)) return 0.0f;
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(current - events[name]).count();
            return delta / 1000000000.0f;
        }

        float EndEvent(std::string name) {
            if (!events.contains(name)) return 0.0f;
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(current - events[name]).count();
            return delta / 1000000000.0f;
        }

        void Setup() {
            StartEvent("Time");
            StartEvent("DeltaTime");
        }

        void Update() {
            programTime = SnapEvent("Time");
            deltaTime = StampEvent("Time");
        }
    }
}