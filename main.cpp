#include "Burst Engine.hpp"

#define RELEASE

// Game crashed when touching orb
// Always at same place in disassembly
// Happens after Load function in level manager

// Collaberate With AI

using namespace Burst;

#include "ai.hpp"
#include "level.hpp"

Vector3 lastGoalPoint = Vector3(0.0f, 1.5f, 0.0f);
bool gamePlayed = false;
struct PlayerMovement : ECS::Component {
    Vector3 velocity;

    Vector3 startPosition = Vector3(0.0f, 1.5f, 0.0f);

    float speed = 4.0f;
    float jumpHeight = 5.0f;

    float speedDampen = 4.0f;

    float resetY = -5.0f;

    float timeSinceGrounded = 0.0f;
    float foxTime = 0.2f;

    bool jumped = false;

#ifdef RELEASE
    float introLength = 15.0f;
#endif
#ifndef RELEASE
    float introLength = 1.0f;
#endif

    float xRotAdd = 0.0f;

    virtual void Start() {
        Input::DisableCursor();

        parent->transform.position = startPosition;

        Time::StartEvent("Intro Time");

        gamePlayed = true;
    }

    virtual void Update() {
        // Lock the cursor if the window is focused
        if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) Input::EnableCursor();
        if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) Input::DisableCursor();

        // Dampen x and z velocity
        float dampenForce = speedDampen * Time::deltaTime;
        float i_Force = 1.0f - dampenForce;
        velocity.x *= i_Force;
        velocity.z *= i_Force;

        // Rotate the player based on input
        parent->transform.rotation.x = (-Input::GetMousePosition().y / 1000.0f) - xRotAdd;
        parent->transform.rotation.y = Input::GetMousePosition().x / 1000.0f;

        // Change the player velocity based on rotation and input
        float h = Input::GetHorizontal() * Time::deltaTime * 5.0f;
        float v = Input::GetVertical() * Time::deltaTime * 5.0f;
        float c = cos(parent->transform.rotation.y);
        float s = sin(parent->transform.rotation.y);
        velocity.x += s * v;
        velocity.z += c * v;
        velocity.x += cos(-parent->transform.rotation.y) * h;
        velocity.z += sin(-parent->transform.rotation.y) * h;

        // Limit speed
        float norm = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
        if (norm > 1.0f) {
            velocity.x /= norm;
            velocity.z /= norm;
        }

        // See if the player is on the ground
        timeSinceGrounded += Time::deltaTime;
        bool grounded = false;

        Vector3 spherePosition = this->parent->transform.position;
        spherePosition.y -= 1.0f;

        float groundY = resetY;

        int platformIndex = 0;
        for (const Platform& platform : LevelManager::currentLevel.platforms) {
            if (Time::SnapEvent("Intro Time") > introLength && platform.hasTrigger && Physics::AABB_Sphere(platform.position + platform.triggerPos, platform.trigger, parent->transform.position, 1.0f)) {
                if(!platform.animated) LevelManager::PlayerTrigger(platformIndex);
            }

            if (Physics::AABB_Sphere(platform.position, platform.scale, spherePosition, 0.25f)) {
                grounded = true;

                timeSinceGrounded = 0.0;

                groundY = platform.position.y + platform.scale.y / 2.0f;
            }

            platformIndex++;
        }

        // Try to jump
        if (Input::GetKeyDown(GLFW_KEY_SPACE) && timeSinceGrounded < foxTime && !jumped) {
            velocity.y = jumpHeight;

            Audio::Play("res/Audio/Jump.wav");

            jumped = true;
        }

        // Add gravity
        float gravity = 9.81f;
        velocity.y -= gravity * Time::deltaTime;
        
        // Move the player based on velocity
        bool moveX = true;
        for (const Platform& platform : LevelManager::currentLevel.platforms) {
            if (Physics::AABB_Sphere(platform.position, platform.scale, parent->transform.position + Vector3(velocity.x * speed * Time::deltaTime, 0.0f, 0.0f), 1.0f)) {
                moveX = false;
            }
        }
        if (moveX) {
            parent->transform.position.x += velocity.x * speed * Time::deltaTime;
        }
        else {
            velocity.x = 0.0f;
        }

        parent->transform.position.y += velocity.y * Time::deltaTime;

        bool moveZ = true;
        for (const Platform& platform : LevelManager::currentLevel.platforms) {
            if (Physics::AABB_Sphere(platform.position, platform.scale, parent->transform.position + Vector3(0.0f, 0.0f, velocity.z * speed * Time::deltaTime), 1.0f)) {
                moveZ = false;
            }
        }
        if (moveZ) {
            parent->transform.position.z += velocity.z * speed * Time::deltaTime;
        }
        else {
            velocity.z = 0.0f;
        }

        // Make sure that the player does not go below the ground
        if (grounded && velocity.y < 0.0f && parent->transform.position.y < groundY + 1.0f) {
            parent->transform.position.y = groundY + 1.0f;
            velocity.y = 0.0f;
            if(jumped) Audio::Play("res/Audio/Land.wav");
            jumped = false;
        }

        // Check if the player lost
        if (parent->transform.position.y < resetY) {
            if(LevelManager::currentLevelIndex < 3){
                parent->transform.position = lastGoalPoint;
            }
            else {
                xRotAdd += parent->transform.rotation.x;
                parent->transform.position = startPosition;
            }

            std::string linepath = LevelManager::currentLevel.GetDieLine();
            if (linepath != "!") Audio::PlaySoundEvent(linepath, "AI");

            velocity = Vector3(0.0f, 0.0f, 0.0f);

            jumped = false;

            LevelManager::ResetLevel();
        }
    }
};

bool won = false;
struct Goal : ECS::Component {
    ECS::Entity* player;

    Goal(ECS::Entity* _player) {
        player = _player;
    }

    virtual void Update() {
        if (Vector3::Distance(parent->transform.position, player->transform.position) < 0.5f) {
            lastGoalPoint = parent->transform.position;

            if (LevelManager::NextLevel()) {
                won = true;
                App::Quit();
            }
            else {

                if (LevelManager::currentLevelIndex > 2) {
                    player->transform.position = Vector3(0.0f, 1.5f, 0.0f);
                }

                if (LevelManager::currentLevelIndex == 5) {
                    Audio::SetSoundEventSpeed("Theme", 1.15f);
                }
                else {
                    Audio::SetSoundEventSpeed("Theme", 1.0f);
                }

                std::string linepath = LevelManager::currentLevel.GetSpawnLine();
                if (linepath != "!") Audio::PlaySoundEvent(linepath, "AI");

                Audio::Play("res/Audio/Win.wav");
            }
        }
    }
};

struct Crasher : ECS::Component {

    float crashTime = 2.5f;

    virtual void Start() {
        Input::EnableCursor();
        Time::StartEvent("Crash");
    }

    virtual void Update() {
        if (Time::SnapEvent("Crash") > crashTime) {
            App::Quit();
        }
    }
};

void LoadGameScene() {
    Engine::LoadScene(1);

#ifdef RELEASE
    Audio::PlaySoundEvent("res/Audio/Voice Lines/Line_1.wav", "AI");
#endif
}

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#define WINDOWS_EXISTS
#endif

int main() {
    srand(time(0));
#ifndef RELEASE
    std::filesystem::current_path("../../../../CG Engine/");
#endif

#ifdef RELEASE
#ifdef WINDOWS_EXISTS
    HWND myConsole = GetConsoleWindow();
    ShowWindow(myConsole, 0);
#endif
#endif

    // Setup the engine

    if(!Engine::Setup(1600, 900, "Burst", false, false)) return 1;

    // This is where to load all of the assets

    Shader shader("res/Shaders/basic_vertex.shader", "res/Shaders/basic_fragment.shader");
    Shader unlitShader("res/Shaders/unlit_vertex.shader", "res/Shaders/unlit_fragment.shader");
    Shader platformShader("res/Shaders/platform_vertex.shader", "res/Shaders/platform_fragment.shader");

    Texture lavaAlbedo("res/Images/Lava/albedo.jpg");
    Texture lavaEmissive("res/Images/Lava/emissive.jpg");

    // Load PBR Texture
    Material platformMaterial(&platformShader);
    Texture grass("res/Images/Ground.png");
    platformMaterial.albedoMap = &grass;
    platformMaterial.emissiveMap = &lavaAlbedo;
    platformMaterial.reflectanceMap = &lavaEmissive;
    platformMaterial.roughness = 0.9f;
    platformMaterial.metallic = 0.01f;

    Material goalMat(&unlitShader);
    goalMat.albedo = Color(1.0f, 0.85f, 0.2f);

    Material lavaMat(&shader);
    lavaMat.albedoMap = &lavaAlbedo;
    lavaMat.albedoTiling = Vector2(50.0f, 100.0f);
    Texture lavaRoughness("res/Images/Lava/roughness.jpg");
    lavaMat.roughnessMap = &lavaRoughness;
    lavaMat.roughnessTiling = Vector2(50.0f, 100.0f);
    lavaMat.emissiveMap = &lavaEmissive;
    lavaMat.emissiveTiling = Vector2(50.0f, 100.0f);

    Mesh sphereMesh("res/Models/Sphere.obj");
    Mesh planeMesh("res/Models/Plane.obj");
    Mesh cubeMesh("res/Models/Cube.obj");

    // This is where to create your scene and entities

    // Main Menu Scene
    ECS::Scene menu;

    {
        ECS::Entity* camera = menu.AddEntity();
        camera->AddComponent<Camera>();

        ECS::Entity* title = menu.AddEntity();
        RectTransform* titleTransform = title->AddComponent<RectTransform>(Anchors::TopMiddle);
        titleTransform->scale = Vector2(600.0f, 200.0f);
        TextRenderer* titleRenderer = title->AddComponent<TextRenderer>();
        titleRenderer->text = "Burst";
        titleRenderer->fontSize = 170.0f;

        ECS::Entity* playButton = menu.AddEntity();
        RectTransform* playTransform = playButton->AddComponent<RectTransform>(Anchors::MiddleMiddle);
        playTransform->scale = Vector2(400.0f, 150.0f);
        Button* button = playButton->AddComponent<Button>();
        button->text = "Play";
        button->radius = 20.0f;
        button->fontSize = 120.0f;
        button->SetClickFunction(LoadGameScene);
    }

    // Game Scene
    ECS::Scene scene;

    ECS::Entity* player = scene.AddEntity();
    player->AddComponent<Camera>();
    player->AddComponent<PlayerMovement>();

    ECS::Entity* light = scene.AddEntity();
    light->transform.position = Vector3(0.75f, 1.0f, -0.75f);
    Light* lightComponent = light->AddComponent<Light>();
    lightComponent->intensity = 1.5f;
    lightComponent->isDirectional = true;

    ECS::Entity* goal = scene.AddEntity();
    goal->transform.scale /= 2.0f;
    goal->AddComponent<MeshRenderer>(&sphereMesh, &goalMat);
    goal->AddComponent<Goal>(player);

    ECS::Entity* lava = scene.AddEntity();
    lava->transform.position = Vector3(0.0f, -5.5f, 0.0f);
    lava->transform.rotation = Vector3(3.14159 / 2.0, 0.0f, 0.0f);
    lava->transform.scale = Vector3(100.0f, 200.0f, 1.0f);
    lava->AddComponent<MeshRenderer>(&planeMesh, &lavaMat);

    ECS::Entity* text = scene.AddEntity();
    RectTransform* textTransform = text->AddComponent<RectTransform>(Anchors::TopMiddle);
    textTransform->position.y += 20.0f;
    textTransform->scale.y = 80.0f;
    TextRenderer* textR = text->AddComponent<TextRenderer>();
    textR->text = "Level 1";
    textR->fontSize = 160.0f;

    ECS::Entity* panel = scene.AddEntity();
    RectTransform* panelTransform = panel->AddComponent<RectTransform>(Anchors::TopMiddle);
    panelTransform->scale = Vector2(600.0f, 100.0f);
    PanelRenderer* panelRenderer = panel->AddComponent<PanelRenderer>();
    panelRenderer->color = Color(1.0f, 1.0f, 1.0f, 0.5f);

    // Levels
    LevelManager::Setup(&scene, &cubeMesh, &platformMaterial, textR, player, goal);

    Level tutoriala;

    // Outer Walls
    tutoriala.AddPlatform(Platform(Vector3(0.0f, 9.0f, 20.0f), Vector3(50.0f, 30.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(20.0f, 9.0f, 15.0f), Vector3(1.0f, 30.0f, 25.0f)));
    tutoriala.AddPlatform(Platform(Vector3(-1.5f, 9.0f, 15.5f), Vector3(1.0f, 30.0f, 25.0f)));
    tutoriala.AddPlatform(Platform(Vector3(10.0f, 20.0f, 10.0f), Vector3(30.0f, 1.0f, 30.0f)));
    tutoriala.AddPlatform(Platform(Vector3(0.0f, -4.75f, 3.5f), Vector3(25.0f, 10.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(0.0f, 13.75f, 3.5f), Vector3(25.0f, 20.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(-3.5f, 0.0f, 3.5f), Vector3(2.0f, 10.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(1.5f, 9.0f, 10.5f), Vector3(1.0f, 30.0f, 14.0f)));

    // Starting area
    tutoriala.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(5.0f, 0.5f, 5.0f)));
    tutoriala.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    tutoriala.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    tutoriala.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    tutoriala.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));
    tutoriala.AddPlatform(Platform(Vector3(1.0f, 2.0f, 3.5f), Vector3(4.0f, 5.0f, 0.2f), { AI_ACTION_OPEN_DOOR }, Vector3(5.0, 5.0f, 5.0f), Vector3(0.0f, 0.0f, -3.5f)));

    // The actuall level
    tutoriala.AddPlatform(Platform(Vector3(0.0f, 0.0f, 13.0f), Vector3(2.0f, 0.5f, 20.0f)));
    tutoriala.AddPlatform(Platform(Vector3(4.0f, 0.0f, 18.5f), Vector3(6.0f, 0.5f, 2.0f)));

    tutoriala.SetGoal(Vector3(0.0f, 1.0f, 15.0f));

    Level tutorialb;
    tutorialb.spawnLines.push_back("res/Audio/Voice Lines/Line_3.wav");

    // Outer Walls
    tutorialb.AddPlatform(Platform(Vector3(0.0f, 9.0f, 20.0f), Vector3(50.0f, 30.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(20.0f, 9.0f, 15.0f), Vector3(1.0f, 30.0f, 25.0f)));
    tutorialb.AddPlatform(Platform(Vector3(-1.5f, 9.0f, 15.5f), Vector3(1.0f, 30.0f, 25.0f)));
    tutorialb.AddPlatform(Platform(Vector3(10.0f, 20.0f, 10.0f), Vector3(30.0f, 1.0f, 30.0f)));
    tutorialb.AddPlatform(Platform(Vector3(0.0f, -4.75f, 3.5f), Vector3(25.0f, 10.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(0.0f, 13.75f, 3.5f), Vector3(25.0f, 20.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(-3.5f, 0.0f, 3.5f), Vector3(2.0f, 10.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(1.5f, 9.0f, 10.5f), Vector3(1.0f, 30.0f, 14.0f)));

    // Starting area
    tutorialb.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(5.0f, 0.5f, 5.0f)));
    tutorialb.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    tutorialb.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    tutorialb.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    tutorialb.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));

    // The actuall level
    tutorialb.AddPlatform(Platform(Vector3(0.0f, 0.0f, 13.0f), Vector3(2.0f, 0.5f, 20.0f)));
    tutorialb.AddPlatform(Platform(Vector3(4.0f, 0.0f, 18.5f), Vector3(6.0f, 0.5f, 2.0f)));
    tutorialb.AddPlatform(Platform(Vector3(6.5f, 1.75f, 18.5f), Vector3(1.0f, 0.5f, 1.0f), { AI_ACTION_LOWER_PLATFORM }, Vector3(3.0f, 1.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f)));
    tutorialb.AddPlatform(Platform(Vector3(13.25f, -2.75f, 18.5f), Vector3(12.5f, 10.0f, 2.0f)));
    tutorialb.AddPlatform(Platform(Vector3(8.5f, 9.0f, 10.0f), Vector3(13.0f, 30.0f, 15.0f)));

    tutorialb.SetGoal(Vector3(15.0f, 3.0f, 18.5f));

    Level tutorialc;
    tutorialc.spawnLines.push_back("res/Audio/Voice Lines/Line_5.wav");

    // Outer Walls
    tutorialc.AddPlatform(Platform(Vector3(0.0f, 9.0f, 20.0f), Vector3(50.0f, 30.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(20.0f, 9.0f, 15.0f), Vector3(1.0f, 30.0f, 25.0f)));
    tutorialc.AddPlatform(Platform(Vector3(-1.5f, 9.0f, 15.5f), Vector3(1.0f, 30.0f, 25.0f)));
    tutorialc.AddPlatform(Platform(Vector3(10.0f, 20.0f, 10.0f), Vector3(30.0f, 1.0f, 30.0f)));
    tutorialc.AddPlatform(Platform(Vector3(0.0f, -4.75f, 3.5f), Vector3(25.0f, 10.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(0.0f, 13.75f, 3.5f), Vector3(25.0f, 20.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(-3.5f, 0.0f, 3.5f), Vector3(2.0f, 10.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(1.5f, 9.0f, 10.5f), Vector3(1.0f, 30.0f, 14.0f)));

    // Starting area
    tutorialc.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(5.0f, 0.5f, 5.0f)));
    tutorialc.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    tutorialc.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    tutorialc.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));

    // The actuall level
    tutorialc.AddPlatform(Platform(Vector3(0.0f, 0.0f, 13.0f), Vector3(2.0f, 0.5f, 20.0f)));
    tutorialc.AddPlatform(Platform(Vector3(4.0f, 0.0f, 18.5f), Vector3(6.0f, 0.5f, 2.0f)));
    tutorialc.AddPlatform(Platform(Vector3(6.5f, 0.75f, 18.5f), Vector3(1.0f, 0.5f, 1.0f)));
    tutorialc.AddPlatform(Platform(Vector3(13.25f, -2.75f, 18.5f), Vector3(12.5f, 10.0f, 2.0f)));
    tutorialc.AddPlatform(Platform(Vector3(8.5f, 9.0f, 10.0f), Vector3(13.0f, 30.0f, 15.0f)));

    tutorialc.AddPlatform(Platform(Vector3(37.0f, -7.75f, 12.0f), Vector3(5.0f, 20.0f, 6.0f), { AI_ACTION_MOVE_X }, Vector3(20.0f, 1.0f, 20.0f), Vector3(-20.0f, 3.0f, 0.0f)));
    tutorialc.AddPlatform(Platform(Vector3(17.0f, -7.75f, 5.0f), Vector3(5.0f, 20.0f, 2.0f)));

    tutorialc.SetGoal(Vector3(17.25f, 3.0f, 5.0f));

    Level labyrinth;
    labyrinth.spawnLines.push_back("res/Audio/Voice Lines/Line_7.wav");

    // Walls
    labyrinth.AddPlatform(Platform(Vector3(0.0f, 20.0f, 0.0f), Vector3(100.0f, 1.0f, 100.0f)));
    labyrinth.AddPlatform(Platform(Vector3(1.5f, -0.25f, 7.5f), Vector3(1.0f, 50.0f, 8.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-1.5f, -0.25f, 7.5f), Vector3(1.0f, 50.0f, 8.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-1.0f, 0.0f, 14.0f), Vector3(18.0f, 50.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-5.0f, 0.0f, 11.0f), Vector3(6.0f, 50.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(6.0f, 0.0f, 11.0f), Vector3(8.0f, 50.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-9.0f, 0.0f, 8.0f), Vector3(2.0f, 50.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-7.5f, -0.25f, 7.5f), Vector3(1.0f, 50.0f, 8.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-10.5f, -0.25f, 10.5f), Vector3(1.0f, 50.0f, 8.0f)));
    labyrinth.AddPlatform(Platform(Vector3(10.5f, -0.25f, 17.5f), Vector3(1.0f, 50.0f, 12.0f)));
    labyrinth.AddPlatform(Platform(Vector3(7.5f, -0.25f, 23.0f), Vector3(1.0f, 50.0f, 19.0f)));
    labyrinth.AddPlatform(Platform(Vector3(10.5f, -0.25f, 31.5f), Vector3(1.0f, 50.0f, 12.0f)));
    labyrinth.AddPlatform(Platform(Vector3(5.0f, 0.0f, 35.0f), Vector3(18.0f, 50.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(3.5f, -0.25f, 31.5f), Vector3(1.0f, 50.0f, 12.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-1.0f, 0.0f, 32.0f), Vector3(18.0f, 50.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(19.0f, 0.0f, 26.5f), Vector3(16.0f, 50.0f, 2.0f)));
    labyrinth.AddPlatform(Platform(Vector3(19.0f, 0.0f, 22.5f), Vector3(16.0f, 50.0f, 2.0f)));
    labyrinth.AddPlatform(Platform(Vector3(30.0f, 0.0f, 30.5f), Vector3(6.0f, 50.0f, 6.0f)));
    labyrinth.AddPlatform(Platform(Vector3(30.0f, 0.0f, 18.5f), Vector3(6.0f, 50.0f, 6.0f)));
    labyrinth.AddPlatform(Platform(Vector3(36.0f, 0.0f, 24.5f), Vector3(6.0f, 50.0f, 6.0f)));
    labyrinth.AddPlatform(Platform(Vector3(30.0f, 25.0f, 24.5f), Vector3(6.0f, 50.0f, 6.0f)));
    labyrinth.AddPlatform(Platform(Vector3(0.0f, 19.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));

    // Starting area
    labyrinth.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(5.0f, 0.5f, 5.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    labyrinth.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    labyrinth.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    labyrinth.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));
    labyrinth.AddPlatform(Platform(Vector3(1.0f, 2.0f, 3.5f), Vector3(4.0f, 5.0f, 0.2f), { AI_ACTION_POSITIVE_X }, Vector3(3.0f, 5.0f, 1.0f)));

    labyrinth.AddPlatform(Platform(Vector3(0.0f, -0.25f, 8.5f), Vector3(2.0f, 1.0f, 10.0f)));
    labyrinth.AddPlatform(Platform(Vector3(0.0f, -0.25f, 12.5f), Vector3(20.0f, 1.0f, 2.0f), { AI_ACTION_TRIGGER_LINE_20 }, Vector3(20.0f, 10.0f, 3.5f)));
    labyrinth.AddPlatform(Platform(Vector3(-9.0f, -0.25f, 11.0f), Vector3(2.0f, 1.0f, 5.0f), { AI_ACTION_TRIGGER_LINE_21 }, Vector3(2.0f, 10.0f, 5.0f)));
    labyrinth.AddPlatform(Platform(Vector3(9.0f, -0.25f, 18.5f), Vector3(2.0f, 1.0f, 10.0f)));
    labyrinth.AddPlatform(Platform(Vector3(9.0f, -5.25f, 24.5f), Vector3(2.0f, 10.0f, 2.0f), { AI_ACTION_TRIGGER_LINE_22 }, Vector3(2.0f, 20.0f, 2.0f)));
    labyrinth.AddPlatform(Platform(Vector3(9.0f, -0.25f, 28.5f), Vector3(2.0f, 1.0f, 10.0f)));
    labyrinth.AddPlatform(Platform(Vector3(5.0f, -0.25f, 33.5f), Vector3(10.0f, 1.0f, 2.0f)));
    // y -= 4
    labyrinth.AddPlatform(Platform(Vector3(20.0f, -0.25f, 24.5f), Vector3(20.0f, 1.0f, 2.0f), { AI_ACTION_TRIGGER_LINE_23 }, Vector3(15.0f, 10.0f, 2.0f)));
    labyrinth.AddPlatform(Platform(Vector3(30.0f, -4.25f, 24.5f), Vector3(6.0f, 1.0f, 6.0f)));

    labyrinth.SetGoal(Vector3(30.0f, -2.75f, 24.5f));

    Level platforms;
    platforms.spawnLines.push_back("res/Audio/Voice Lines/Line_8.wav");

    // Walls
    platforms.AddPlatform(Platform(Vector3(0.0f, 20.0f, 0.0f), Vector3(100.0f, 1.0f, 100.0f)));
    platforms.AddPlatform(Platform(Vector3(0.0f, 9.0f, 20.0f), Vector3(50.0f, 30.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(-20.0f, 9.0f, 0.0f), Vector3(1.0f, 30.0f, 50.0f)));
    platforms.AddPlatform(Platform(Vector3(15.0f, 9.0f, 0.0f), Vector3(1.0f, 30.0f, 50.0f)));
    platforms.AddPlatform(Platform(Vector3(13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(-13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(0.0f, 19.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(0.0f, -15.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));

    // Starting area
    platforms.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(5.5f, 0.5f, 5.5f)));
    platforms.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    platforms.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    platforms.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));
    platforms.AddPlatform(Platform(Vector3(1.0f, 2.0f, 3.5f), Vector3(4.0f, 5.0f, 0.2f), { AI_ACTION_POSITIVE_X }, Vector3(3.0f, 5.0f, 1.0f)));

    platforms.AddPlatform(Platform(Vector3(0.0f, 0.0f, 5.0f), Vector3(2.5f, 0.5f, 2.5f)));
    platforms.AddPlatform(Platform(Vector3(0.0f, 1.0f, 10.0f), Vector3(2.5f, 0.5f, 2.5f)));
    platforms.AddPlatform(Platform(Vector3(4.0f, 2.0f, 11.0f), Vector3(2.5f, 0.5f, 2.5f), { AI_ACTION_SHRINK_X }, Vector3(2.5f, 2.3f, 2.5f)));
    platforms.AddPlatform(Platform(Vector3(2.0f, 3.0f, 15.0f), Vector3(2.5f, 0.5f, 2.5f)));
    platforms.AddPlatform(Platform(Vector3(-4.0f, 4.0f, 15.0f), Vector3(5.0f, 0.5f, 2.5f), { AI_ACTION_SHRINK_Z }, Vector3(5.5f, 3.0f, 3.0f)));
    platforms.AddPlatform(Platform(Vector3(-8.5f, 5.0f, 13.0f), Vector3(2.5f, 0.5f, 2.5f)));
    platforms.AddPlatform(Platform(Vector3(-0.5f, 6.0f, 10.0f), Vector3(10.0f, 0.5f, 2.5f), { AI_ACTION_HIDE }, Vector3(9.0f, 2.0f, 2.5f), Vector3(0.5f, 1.0f, 0.0f)));
    platforms.AddPlatform(Platform(Vector3(7.0f, 7.0f, 11.0f), Vector3(1.0, 0.25f, 1.0f)));
    platforms.AddPlatform(Platform(Vector3(-16.5f, 8.0f, 13.0f), Vector3(2.5f, 0.5f, 2.5f)));
    platforms.AddPlatform(Platform(Vector3(-4.5f, 7.5f, 13.0f), Vector3(21.0f, 0.25f, 0.5f), { AI_ACTION_HIDE }, Vector3(18.0f, 2.0f, 1.0f), Vector3(1.0f, 1.0f, 0.0f)));

    platforms.SetGoal(Vector3(-16.5f, 9.25f, 13.0f));

    Level crumble;
    crumble.spawnLines.push_back("res/Audio/Voice Lines/Line_13.wav");

    // Walls
    crumble.AddPlatform(Platform(Vector3(0.0f, 20.0f, 0.0f), Vector3(100.0f, 1.0f, 200.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, 9.0f, 80.0f), Vector3(50.0f, 30.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(-15.0f, 9.0f, 50.0f), Vector3(1.0f, 30.0f, 100.0f)));
    crumble.AddPlatform(Platform(Vector3(15.0f, 9.0f, 50.0f), Vector3(1.0f, 30.0f, 100.0f)));
    crumble.AddPlatform(Platform(Vector3(13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(-13.0f, 9.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, 19.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, -15.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));

    // Starting area
    crumble.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(6.0f, 0.5f, 6.0f)));
    crumble.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    crumble.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));
    crumble.AddPlatform(Platform(Vector3(1.0f, 2.0f, 3.5f), Vector3(4.0f, 5.0f, 0.2f), { AI_ACTION_POSITIVE_X }, Vector3(3.0f, 5.0f, 1.0f)));

    crumble.AddPlatform(Platform(Vector3(0.0f, -6.25f, 7.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(-3.0f, -5.75f, 13.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(1.0f, -7.25f, 19.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_PLATFORM_DESTROY }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(4.0f, -6.75f, 25.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(2.0f, -6.0f, 31.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, -5.25f, 37.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(1.0f, -4.5f, 43.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_PLATFORM_DESTROY }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(-2.0f, -6.0f, 49.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(-5.0f, -5.25f, 55.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_PLATFORM_DESTROY }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(-3.0f, -4.5f, 61.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, -3.5f, 66.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_NEGATIVE_Y }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(1.0f, -2.5f, 72.0f), Vector3(3.0f, 13.0f, 4.0f), { AI_ACTION_PLATFORM_DESTROY }, Vector3(5.0f, 5.0f, 2.0f), Vector3(0.0f, 7.5f, 2.0f)));
    crumble.AddPlatform(Platform(Vector3(0.0f, 4.0f, 78.0f), Vector3(7.5f, 1.0f, 5.0f)));

    crumble.SetGoal(Vector3(0.0f, 5.5f, 78.0f));

    Level horizon;
    horizon.spawnLines.push_back("res/Audio/Voice Lines/Line_17.wav");

    // Starting area
    horizon.AddPlatform(Platform(Vector3(0.0f, 0.0f, 1.0f), Vector3(6.0f, 0.5f, 6.0f)));
    horizon.AddPlatform(Platform(Vector3(-2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    horizon.AddPlatform(Platform(Vector3(2.5f, 2.0f, 1.0f), Vector3(1.0f, 5.0f, 5.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, 2.0f, -1.5f), Vector3(5.0f, 5.0f, 1.0f)));
    horizon.AddPlatform(Platform(Vector3(-2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    horizon.AddPlatform(Platform(Vector3(2.0f, 2.0f, 3.5f), Vector3(2.0f, 5.0f, 1.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 1.0f, 6.0f)));

    horizon.AddPlatform(Platform(Vector3(0.0f, 9.0f, 151.0f), Vector3(50.0f, 50.0f, 100.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, 19.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, -15.0f, 3.5f), Vector3(20.0f, 30.0f, 1.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, 20.0f, 0.0f), Vector3(100.0f, 1.0f, 200.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, -0.25f, 53.5f), Vector3(2.0f, 1.0f, 100.0f)));
    horizon.AddPlatform(Platform(Vector3(3.0f, -0.25f, 53.5f), Vector3(1.0f, 50.0f, 100.0f)));
    horizon.AddPlatform(Platform(Vector3(-3.0f, -0.25f, 53.5f), Vector3(1.0f, 50.0f, 100.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, -0.25f, 50.0f), Vector3(1.0f, 1.0f, 1.0f), { AI_ACTION_EPIC_LINE }, Vector3(5.0f, 10.0f, 5.0f)));
    horizon.AddPlatform(Platform(Vector3(0.0f, -0.25f, 95.0f), Vector3(1.0f, 1.0f, 1.0f), { AI_ACTION_EPIC_LINE_2 }, Vector3(5.0f, 10.0f, 1.0f)));

    horizon.SetGoal(Vector3(0.0f, 1.5f, 100.0f));

    LevelManager::AddLevel(&tutoriala);
    LevelManager::AddLevel(&tutorialb);
    LevelManager::AddLevel(&tutorialc);
    LevelManager::AddLevel(&labyrinth);
    LevelManager::AddLevel(&platforms);
    LevelManager::AddLevel(&crumble);
    LevelManager::AddLevel(&horizon);

#ifdef RELEASE
    LevelManager::Load(0);
#endif
#ifndef RELEASE
    LevelManager::Load(3);
#endif

    Engine::AddSystemUpdate(PlatformAnimator::Update);

    Engine::AddScene(&menu);
    Engine::AddScene(&scene);

    Audio::PlaySoundEvent("res/Audio/Theme.mp3", "Theme", true);

    if (!Engine::Run()) return 1;

#ifdef RELEASE
#ifdef WINDOWS_EXISTS
    if (!gamePlayed) {
        int result = MessageBox(NULL, "Please play my game!", "User Error", MB_ICONERROR | MB_YESNO);
        if (result == 6) system("start CMakeTarget.exe");
    }
    else if(won) {
        // Do epic windows error message
        int result = MessageBox(NULL, "Did you really escape?", NULL, MB_ICONERROR | MB_YESNO);
        if (result != 7) {
            result = MessageBox(NULL, "Did YOU really escape?", NULL, MB_ICONERROR | MB_YESNO);
        }
        else {
            MessageBox(NULL, ":)", "I did", MB_ICONERROR | MB_OK);
            return 0;
        }
        if (result != 7) {
            MessageBox(NULL, "DID", NULL, MB_ICONERROR | MB_OK);
            MessageBox(NULL, "YOU", NULL, MB_ICONERROR | MB_OK);
            MessageBox(NULL, "ESCAPE", NULL, MB_ICONERROR | MB_OK);
            result = MessageBox(NULL, "?", NULL, MB_ICONERROR | MB_YESNO);
        }
        else {
            MessageBox(NULL, ":)", "I did", MB_ICONERROR | MB_OK);
            return 0;
        }
        if (result != 7) {
            system("start END_USER_EXISTANCE.exe");
            system("start LOOK_BEHIND_YOU.exe");
            MessageBox(NULL, "Error", "Error", MB_ICONERROR | MB_OK);
        }
        else {
            MessageBox(NULL, ":)", "I did", MB_ICONERROR | MB_OK);
            return 0;
        }
    }
    // yes 6
    // no 7
#endif
#endif

    return 0;
}