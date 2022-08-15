// ECS and ID
#include <vector>

// ID
#include <sstream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>
#include <ctime>

namespace Burst{
    namespace ID{
        const char characters[62] = {
            '0',
            '1',
            '2',
            '3',
            '4',
            '5',
            '6',
            '7',
            '8',
            '9',
            'a',
            'b',
            'c',
            'd',
            'e',
            'f',
            'g',
            'h',
            'i',
            'j',
            'k',
            'l',
            'm',
            'n',
            'o',
            'p',
            'q',
            'r',
            's',
            't',
            'u',
            'v',
            'w',
            'x',
            'y',
            'z',
            'A',
            'B',
            'C',
            'D',
            'E',
            'F',
            'G',
            'H',
            'I',
            'J',
            'K',
            'L',
            'M',
            'N',
            'O',
            'P',
            'Q',
            'R',
            'S',
            'T',
            'U',
            'V',
            'W',
            'X',
            'Y',
            'Z'
        };

        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> dis(0, 61);

        bool IDInitDone = false;

        void InitID(){
            gen.seed(time(NULL));
            IDInitDone = true;
        }

        struct ID{
            std::string data;

            ID(){
                this->data = "";
            }

            ID(std::string _data){
                this->data = _data;
            }
        };

        ID NewID(){
            if(!IDInitDone) InitID();

            std::string out;

            for(int i = 0; i < 32; i++){
                int index = static_cast<int>(dis(gen));
                out += characters[index];
            }

            return ID(out);
        }
    }

    namespace ECS{
        struct TagList{
            std::vector<std::string> tags;

            void AddTag(std::string _tag){
                for(std::string tag : this->tags){
                    if(tag == _tag){
                        return;
                    }
                }
                this->tags.push_back(_tag);
            }

            bool HasTag(std::string _tag){
                for(std::string tag : this->tags){
                    if(tag == _tag){
                        return true;
                    }
                }
                return false;
            }
        };

        class Entity;
        class Scene;

        struct Component{
            Entity* parent;

            ID::ID id;

            virtual void Start() {}
            virtual void Update() {}
            virtual void Render() {}
            virtual void UIRender() {}
        };

        class Entity{
        public:
            Scene* parent;

            std::vector<Component*> components;

            std::string name = "Entity";

            Transform transform;

            TagList tags;

            ID::ID id;

            template <typename T, typename... Args>
		    T* AddComponent(Args&&... args) {
		    	T* component = new T(std::forward<Args>(args)...);
                component->parent = this;
                component->id = ID::NewID();

                this->components.push_back(component);

		    	return component;
		    }

            void RemoveComponent(Component* remove){
                int index = -1;
                int i = 0;
                for(Component* component : this->components){
                    if(component == remove){
                        index = i;
                    }
                    i++;
                }
                if(index != -1){
                    delete this->components[index];
                    this->components.erase(this->components.begin() + index);
                }
            }

            void Start(){
                for(Component* component : this->components){
                    component->Start();
                }
            }

            void Update(){
                for(Component* component : this->components){
                    component->Update();
                }
            }

            void Render() {
                for (Component* component : this->components) {
                    component->Render();
                }
            }

            void UIRender() {
                for (Component* component : this->components) {
                    component->UIRender();
                }
            }

            void Remove(){
                for(Component* component : this->components){
                    delete component;
                }
                this->components.clear();
            }
        };

        class Scene{
        public:
            std::vector<Entity*> entities;

            std::string name = "Scene";

            ID::ID id;

            Scene(std::string _name="Scene"){
                name = _name;
                id = ID::NewID();
            }

            Entity* GetEntityById(ID::ID _id){
                for(Entity* entity : this->entities){
                    if(entity->id.data == _id.data){
                        return entity;
                    }
                }
                return nullptr;
            }

            Entity* GetEntityByName(std::string _name){
                for(Entity* entity : this->entities){
                    if(entity->name == _name){
                        return entity;
                    }
                }
                return nullptr;
            }

            Entity* GetEntityByTag(std::string _tag){
                for(Entity* entity : this->entities){
                    if(entity->tags.HasTag(_tag)) return entity;
                }
                return nullptr;
            }

            std::vector<Entity*> GetEntitiesByTag(std::string _tag){
                std::vector<Entity*> out;
                for(Entity* entity : this->entities){
                    if(entity->tags.HasTag(_tag)){
                        out.push_back(entity);
                    }
                }
                return out;
            }

            Entity* AddEntity(std::string _name="Entity", std::string _mainTag="Entity"){
                Entity* entity = new Entity;
                entity->name = _name;
                entity->id = ID::NewID();
                entity->tags.AddTag(_mainTag);

                this->entities.push_back(entity);

                return entity;
            }

            void RemoveEntity(Entity* remove){
                int index = -1;
                int i = 0;
                for(Entity* entity : this->entities){
                    if(entity == remove){
                        index = i;
                    }
                    i++;
                }
                if(index != -1){
                    this->entities[index]->Remove();
                    delete this->entities[index];
                    this->entities.erase(this->entities.begin() + index);
                }
            }

            void Start(){
                for(Entity* entity : this->entities){
                    entity->Start();
                }
            }

            void Update(){
                for(Entity* entity : this->entities){
                    entity->Update();
                }
            }

            void Render() {
                for (Entity* entity : this->entities) {
                    entity->Render();
                }
            }

            void UIRender() {
                for (Entity* entity : this->entities) {
                    entity->UIRender();
                }
            }

            void Remove(){
                unsigned int length = this->entities.size();
                while(length > 0){
                    this->entities[0]->Remove();
                    delete this->entities[0];
                    this->entities.erase(this->entities.begin());
                    length--;
                }
            }
        };
    }
}