struct Platform {
    Vector3 position;
    Vector3 scale = Vector3(5.0f, 0.5f, 5.0f);
    Vector3 trigger;
    Vector3 triggerPos;
    AI::ActionList actionList;
    bool hasTrigger = false;
    float max = 0.0f;

    bool animated = false;

    Platform(Vector3 _position, Vector3 _scale, std::vector<unsigned int> actions = { AI_ACTION_NONE }, Vector3 triggerBoxSize=Vector3(-1.0f, -1.0f, -1.0f), Vector3 triggerBoxPos=Vector3(0.0f, 0.0f, 0.0f)) {
        position = _position;
        scale = _scale;
        actionList = AI::ActionList(actions);

        if (actions.size() > 1 || actions[0] != AI_ACTION_NONE) hasTrigger = true;

        if (triggerBoxSize.x == -1.0f) {
            trigger = Vector3(0.1f, 0.1f, 0.1f);
            hasTrigger = false;
        }
        else {
            trigger = triggerBoxSize;
        }

        triggerPos = triggerBoxPos;

        max = scale.x;
        if (max < scale.y) max = scale.y;
        if (max < scale.z) max = scale.z;
    }
};

struct Level {
    std::vector<Platform> platforms;
    Vector3 goalPosition;

    std::vector<std::string> spawnLines;
    std::vector<std::string> dieLines;

    void AddPlatform(Platform platform) {
        // platforms.emplace_back(platform.position, platform.scale, platform.actionList.actions, (platform.hasTrigger ? platform.trigger : Vector3(-1.0f, -1.0f, -1.0f)));
        platforms.push_back(platform);
    }

    void SetGoal(Vector3 position) {
        goalPosition = position;
    }

    std::string GetSpawnLine() {
        if (spawnLines.size() == 0) return "!";
        return spawnLines[rand() % spawnLines.size()];
    }

    std::string GetDieLine() {
        if (dieLines.size() == 0) return "!";
        return dieLines[rand() % dieLines.size()];
    }

    Level() {
        return;
    }

    Level(const Level& obj) {
        for (const Platform& platform : obj.platforms) {
            platforms.push_back(platform);
        }
        goalPosition = obj.goalPosition;
        spawnLines = obj.spawnLines;
        dieLines = obj.dieLines;
    }
};

struct PlatformComponent : ECS::Component {
    Platform* platform;

    Material* material;

    float alpha = 1.0f;

    PlatformComponent(Platform* _platform, Material* _material) {
        platform = _platform;
        material = _material;
    }

    virtual void Update() {
        parent->transform.position = platform->position;
        parent->transform.scale = platform->scale;
    }

    virtual void Render() {
        material->albedo.a = alpha;
    }
};

float Lerp(float a, float b, float t, float t_d) {
    t /= t_d;
    return (a * (1.0 - t)) + (b * t);
}

float MapVar(float a, float b, float t, float t_d) {
    t /= t_d;
    float t1 = t * t;
    float t2 = -(1 - t) * (1 - t) + 1;
    t = Lerp(t1, t2, t, 1.0f);
    return (a * (1.0 - t)) + (b * t);
}

Vector3 MapVar(Vector3 a, Vector3 b, float t, float t_d) {
    return Vector3(MapVar(a.x, b.x, t, t_d), MapVar(a.y, b.y, t, t_d), MapVar(a.z, b.z, t, t_d));
}

unsigned int animCount = 0;
struct PlatformAnimation {
    float time = 0.0f;

    PlatformComponent* component;
    Platform* platform;

    unsigned int action;

    unsigned int index = 0;

    Vector3 ogpos;
    Vector3 ogsize;
    float ogalpha;

    Vector3 pos;
    Vector3 size;
    float alpha;

    void StartAnim() {
        index = animCount;
        Time::StartEvent("Platform Animation " + std::to_string(index));
        animCount++;

        ogpos = platform->position;
        ogsize = platform->scale;
        ogalpha = component->alpha;

        pos = platform->position;
        size = platform->scale;
        alpha = component->alpha;

        if (action == AI_ACTION_HIDE) {
            alpha = 0.0f;
        }
        else if (action == AI_ACTION_TRIGGER_LINE_23) {
            pos.y -= 4.0f;
        }
        else if (action == AI_ACTION_SHOW) {
            alpha = 1.0f;
        }
        else if (action == AI_ACTION_POSITIVE_X || action == AI_ACTION_OPEN_DOOR) {
            pos.x += platform->max;
        }
        else if (action == AI_ACTION_NEGATIVE_X || action == AI_ACTION_MOVE_X) {
            pos.x -= platform->max;
        }
        else if (action == AI_ACTION_POSITIVE_Y) {
            pos.y += platform->max;
        }
        else if (action == AI_ACTION_NEGATIVE_Y || action == AI_ACTION_LOWER_PLATFORM || action == AI_ACTION_PLATFORM_DESTROY) {
            pos.y -= platform->max;
        }
        else if (action == AI_ACTION_POSITIVE_Z) {
            pos.z += platform->max;
        }
        else if (action == AI_ACTION_NEGATIVE_Z) {
            pos.z -= platform->max;
        }
        else if (action == AI_ACTION_GROW_X) {
            size.x *= 2.0f;
        }
        else if (action == AI_ACTION_SHRINK_X) {
            size.x /= 2.0f;
        }
        else if (action == AI_ACTION_GROW_Y) {
            size.y *= 2.0f;
        }
        else if (action == AI_ACTION_SHRINK_Y) {
            size.y /= 2.0f;
        }
        else if (action == AI_ACTION_GROW_Z) {
            size.z *= 2.0f;
        }
        else if (action == AI_ACTION_SHRINK_Z) {
            size.z /= 2.0f;
        }
    }

    void UpdateAnim() {
        time = Time::SnapEvent("Platform Animation " + std::to_string(index));

        platform->position = MapVar(ogpos, pos, time, 1.0f);
        platform->scale = MapVar(ogsize, size, time, 1.0f);
        component->alpha = MapVar(ogalpha, alpha, time, 1.0f);
    }
};

namespace PlatformAnimator {
    std::vector<PlatformAnimation> animations;
    unsigned int animationCount = 0;

    float animationLength = 1.0f;

    void AddAnimation(unsigned int action, Platform* platform, PlatformComponent* comp, ECS::Entity* entity) {
        animations.emplace_back();
        PlatformAnimation* animation = &animations[animations.size() - 1];

        animation->index = animationCount;
        animationCount++;

        animation->action = action;
        animation->platform = platform;
        animation->component = comp;

        animation->StartAnim();
    }

    void Update() {
        int finishedAnims = 0;
        for (PlatformAnimation& animation : animations) {
            animation.UpdateAnim();
            if (animation.time > animationLength) {
                finishedAnims++;
            }
        }

        for (int i = 0; i < finishedAnims; i++) {
            animations.erase(animations.begin());
        }
    }
}

namespace LevelManager {
    std::vector<Burst::ECS::Entity*> entities;
    std::vector<PlatformComponent*> entityPlatforms;

    Burst::ECS::Scene* scene;

    Burst::Mesh* platformMesh;
    Burst::Material* platformMaterial;

    std::vector<Level*> levels;
    Level currentLevel;
    unsigned int currentLevelIndex = 0;

    ECS::Entity* player;

    ECS::Entity* goalEntity;

    TextRenderer* levelText;

    void Setup(Burst::ECS::Scene* _scene, Burst::Mesh* _platformMesh, Burst::Material* _platformMaterial, Burst::TextRenderer* _levelText, ECS::Entity* _player, ECS::Entity* _goalEntity) {
        scene = _scene;

        platformMesh = _platformMesh;
        platformMaterial = _platformMaterial;
        levelText = _levelText;
        player = _player;
        goalEntity = _goalEntity;
    }

    void AddLevel(Level* level) {
        levels.push_back(level);
    }

    void Generate() {
        goalEntity->transform.position = currentLevel.goalPosition;

        int index = 0;
        for (Platform& platform : currentLevel.platforms) {
            ECS::Entity* entity = scene->AddEntity();

            entity->transform.position = platform.position;
            entity->transform.scale = platform.scale;

            PlatformComponent* comp = entity->AddComponent<PlatformComponent>(&platform, platformMaterial);
            entity->AddComponent<MeshRenderer>(platformMesh, platformMaterial);

            entities.push_back(entity);
            entityPlatforms.push_back(comp);

            index++;
        }
    }

    void Destroy() {
        entityPlatforms.clear();
        for (Burst::ECS::Entity* entity : entities) {
            scene->RemoveEntity(entity);
        }
        entities.clear();
    }

    void Load(unsigned int levelIndex) {
        if (levelIndex < 3) {
            levelText->text = "Tutorial";
        }
        else if (levelIndex == 3) {
            levelText->text = "Labyrinth";
        }
        else if (levelIndex == 4) {
            levelText->text = "Platforms";
        }
        else if (levelIndex == 5) {
            levelText->text = "Collapse";
        }
        else if (levelIndex == 6) {
            levelText->text = "Event Horizon";
        }
        else {
            levelText->text = "Burst";
        }

        Destroy();

        currentLevel = Level(*levels[levelIndex]);
        currentLevelIndex = levelIndex;

        Generate();
    }

    void ResetLevel() {
        Load(currentLevelIndex);
    }

    bool NextLevel() {
        if (currentLevelIndex + 1 >= levels.size()) {
            return true;
        }

        Load(currentLevelIndex + 1);
        return false;
    }

    void PlayerTrigger(int platformIndex) {
        ECS::Entity* triggeredPlatform = entities[platformIndex];

        Platform* platform = &currentLevel.platforms[platformIndex];

        unsigned int action = platform->actionList.GetAction();

        int textIndex = rand() % AI::aiText[action].size();
        std::string audioPath = AI::aiText[action][textIndex];
        if (audioPath != "!") {
            Audio::PlaySoundEvent(audioPath, "AI");
        }

        if (action != AI_ACTION_NONE && !platform->animated) {
            PlatformAnimator::AddAnimation(action, platform, entityPlatforms[platformIndex], triggeredPlatform);
            platform->animated = true;
        }
    }
}