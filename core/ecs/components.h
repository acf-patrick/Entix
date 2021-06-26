#pragma once

#include <tuple>
#include <string>

#include <util/vector.h>
#include <event/event.h>

#include "baseScript.h"
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

