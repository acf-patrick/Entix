#include "../../components.h"
#include "../../../renderer/renderer.h"

#include <algorithm>

namespace Component
{

    tson::Tileson Tilemap::tileson;

    Tilemap::Tilemap(const std::string &rsc) : file(rsc)
    {
        _map = tileson.parse(fs::path(rsc));
        if (_map->getStatus() == tson::ParseStatus::OK)
        {
        }
        else
            std::cerr << "Tilemap-error : " << _map->getStatusMessage() << std::endl;
    }

    Tilemap::~Tilemap()
    {
        // if this tilemap use the default drawer,
        // the 'drawer' instance was not registered into component manager
        // hence, must be destroyed manually
        if (_defaultDrawer)
            delete _drawer;
    }

    void Tilemap::Render()
    {
        // Creating default Drawer if no Tilemap::Drawer component added
        auto eID = entity->id();
        _drawer = Drawer::instances[eID];
        if (!_drawer)
        {
            _defaultDrawer = true;
            _drawer = new Drawer;
        }

        Renderer->submit([&](SDL_Renderer *renderer)
                         { eachLayer([&](tson::Layer &layer)
                                     { _drawLayer(layer, renderer); }); });
    }

    void Tilemap::eachLayer(const std::function<void(tson::Layer &)> &process, const std::vector<tson::LayerType> &layType)
    {
        for (auto &layer : _map->getLayers())
            if (layType.empty() or std::find(layType.begin(), layType.end(), layer.getType()) != layType.end())
                process(layer);
    }

    void Tilemap::_drawLayer(tson::Layer& layer, SDL_Renderer* renderer)
    {
        using type = tson::LayerType;
        switch (layer.getType())
        {
        case type::Group:
            for (auto& lay: layer.getLayers())
                _drawLayer(lay, renderer);
            break;

        case type::ImageLayer:
            _drawer->drawImage(layer.getImage());
            break;

        // case type::ObjectGroup:
        //     layer.
        }
    }

};