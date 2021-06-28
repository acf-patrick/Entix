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
        Vector<double> position = { 0, 0 };
        VectorF scale = { 1, 1 };
        double rotation = 0;
    };

    // Entity Container
    // when we create Entity through a group, this components contening the group is attached to the entity
    struct group
    { Group* content; };

    class camera : public ICamera
    {
    public:
        // position of the camera
        // default : (0, 0)
        Vector<double> position = { 0, 0 };

        // scale factor used on final rendering
        // default : (1, 1)
        Vector<float> scale = { 1, 1 };

        // rotation around z-axis
        // default : 0
        double rotation = 0;

        // size of view
        // (1, 1) the whole screen
        Vector<float> size = { 1, 1 };

        // indicates where on the screen this camera view will be drawn.
        // (0, 0) top-left and (1, 1) bottom-right
        // default : (0, 0)
        Vector<float> destination = { 0, 0 };

        // color applied before any rendering
        // default : black
        SDL_Color background = { 0, 0, 0, 255 };
        SDL_Texture* _colorTexture = nullptr;

        // stores used for background
        // don't forget to update the 'clear' property after setting this variable
        SDL_Texture* backgroundImage = nullptr;

        enum ClearMode { 
            NONE,                       // Don't clear the background
            TEXTURE,                    // Clear the background with texture set in backgroundImage property
            SOLID_COLOR,                // Clear the background with color set in background property
            TEXTURE_AND_SOLID_COLOR     // Useless if the texture doesn't support transparency
        };

        // clear the background?
        // default : SOLID_COLOR
        ClearMode clear = NONE;

        // flip rendered view
        Vector<bool> flip = { false, false };

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
            SDL_DestroyTexture(_colorTexture);
            _colorTexture = backgroundImage = nullptr;
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