#pragma once

#include <tuple>
#include <string>
#include <algorithm>
#include <SDL2/SDL.h>

#include <util/vector.h>
#include <event/event.h>

#include "baseScript.h"
#include "baseCamera.h"
#include "entity/entity.h"

class Group;

namespace Component {
    
    // ID
    // mostly used to identify Events
    struct tag
    { std::string content; };

    // Space specs
    struct transform
    {
        Vector<double> position;
        VectorF scale;
        double rotation;
    };

    // Entity Container
    // when we create Entity through a group, this components contening the group is attached to the entity
    struct group
    { Group* content; };

    class camera : public ICamera
    {
    public:
        // color applied before any rendering
        // default : black
        SDL_Color background = { 0, 0, 0, 255 };

        // stores used for background
        SDL_Texture* backgroundImage = nullptr;

        // clear the background?
        // default : false
        bool clear = false;

        // flip rendered view
        Vector<bool> flip = { false, false };

        // size of view
        // (1, 1) the whole screen
        Vector<float> size = { 1, 1 };

        // indicates where on the screen this camera view will be drawn.
        // (0, 0) top-left and (1, 1) bottom-right
        VectorF destination = { 1, 1 };

        // camera position in draw orders
        // cameras with larger value will be drawn on top of cameras with a smaller value.
        int depth = 0;

        static std::vector<camera*> instancies;

        camera()
        {
            instancies.push_back(this);
        }

        ~camera()
        {
            instancies.erase(std::remove(instancies.begin(), instancies.end(), this));
            SDL_DestroyTexture(backgroundImage);
        }

        bool isMain()
        { return instancies[0] == this; }
    };

    // Script Component
    class script : public BaseScript
    {
    protected:
        EventListner event;

    public:
        template<typename T>
        bool has() const
        { return entity->has<T>(); }

        template<typename... T>
        bool all_of() const
        { return entity->all_of<T...>(); }

        template<typename... T>
        bool any_of() const
        { return entity->any_of<T...>(); }

        template<typename... T>
        bool none_of() const
        { return entity->none_of<T...>(); }

        template<typename T>
        T& get()
        { return entity->get<T>(); }

        template<typename... T>
        std::tuple<T&...> retrieve()
        { return entity->retrieve<T...>(); }

        template<typename T, typename... TArgs>
        T& attach(TArgs&&... args)
        { return entity->attach<T>(std::forward<TArgs>(args)...); }

        template<typename T>
        void detach()
        { entity->detach<T>(); }
    };

};