#include "Burst Engine.hpp"

using namespace Burst;

struct PlayerMovement : ECS::Component {
    Vector3 velocity;

    float speed = 2.0f;
    float jumpHeight = 3.0f;

    virtual void Start() {
        Input::DisableCursor();
    }

    virtual void Update() {
        // Lock the cursor if the window is focused
        if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) Input::EnableCursor();
        if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) Input::DisableCursor();

        // Dampen x and z velocity
        float dampenForce = 0.05f;
        float i_Force = 1.0f - dampenForce;
        velocity.x *= i_Force;
        velocity.z *= i_Force;

        // Rotate the player based on input
        parent->transform.rotation.x = -Input::GetMousePosition().y / 1000.0f;
        parent->transform.rotation.y = Input::GetMousePosition().x / 1000.0f;

        // Change the player velocity based on rotation and input
        float h = Input::GetHorizontal() / 30.0f;
        float v = Input::GetVertical() / 30.0f;
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
            velocity.y /= norm;
        }

        // Try to jump
        if (Input::GetKeyDown(GLFW_KEY_SPACE) && parent->transform.position.y == 1.0f) {
            velocity.y = jumpHeight;
        }

        float deltaTime = 0.01f;

        // Add gravity
        float gravity = 9.81f / 2.0f;
        velocity.y -= gravity * deltaTime;
        
        // Move the player based on velocity
        parent->transform.position += (velocity * Vector3(speed, 1.0f, speed) * deltaTime);

        // Make sure that the player does not go below the ground
        if (parent->transform.position.y < 1.0f) {
            parent->transform.position.y = 1.0f;
            velocity.y = 0.0f;
        }
    }
};

int main() {
    // Setup the window and input systems

    if (!Window::Setup(1600, 900, "Burst Engine : Setup")) {
        std::cout << "Burst Engine could not run." << std::endl;
        return 1;
    }

    Input::Setup();

    // This is where to load all of the assets

    Shader shader("res/Shaders/basic_vertex.shader", "res/Shaders/basic_fragment.shader");

    // Load PBR Texture
    Material redMaterial(&shader);
    redMaterial.albedo = Color(255.0f, 0.0f, 0.0f);
    redMaterial.roughness = 0.2f;
    redMaterial.metallic = 0.4f;

    Material grassMaterial(&shader);
    Texture grass("res/Images/Grass.png");
    grassMaterial.albedoMap = &grass;
    grassMaterial.albedoTiling = Vector2(5.0f, 5.0f);
    grassMaterial.roughness = 0.9f;
    grassMaterial.metallic = 0.01f;

    Mesh sphereMesh("res/Models/Sphere.obj");
    Mesh planeMesh("res/Models/Plane.obj");

    // This is where to create your scene and entities

    ECS::Scene scene;

    ECS::Entity* player = scene.AddEntity();
    player->AddComponent<Camera>();
    player->AddComponent<PlayerMovement>();

    ECS::Entity* sphere = scene.AddEntity();
    sphere->transform.position.z = 2.0f;
    sphere->AddComponent<MeshRenderer>(&sphereMesh, &redMaterial);

    ECS::Entity* ground = scene.AddEntity();
    ground->transform.position = Vector3(0.0f, -0.5f, 0.0f);
    ground->transform.rotation = Vector3(3.14159 / 2.0f, 0.0f, 0.0f);
    ground->transform.scale = Vector3(10.0f, 10.0f, 1.0f);
    ground->AddComponent<MeshRenderer>(&planeMesh, &grassMaterial);

    // Enviornment Stuff
    Enviornment::Setup();
    Texture skybox("res/Images/Skybox.hdr", false, true, TEXTURE_HDR);
    Enviornment::SetSkybox(&skybox);

    // Start the scene
    scene.Start();

    while (!Window::WindowShouldClose() && App::running){
        Input::Update();

        Renderer::Clear();

        // Update Scene
        scene.Update();

        // Render Enviornment
        Enviornment::Render();

        Enviornment::SendSkyboxToShaders();

        scene.Render();

        Window::SwapBuffers();

        App::PollEvents();
    }

    App::Terminate();
    return 0;
}
