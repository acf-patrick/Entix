#include <algorithm>

#include "../../../logger/logger.h"
#include "../../../renderer/renderer.h"
#include "../../components.h"

namespace entix {
namespace ecs {
namespace component {

tson::Tileson Tilemap::tileson;

namespace fs = std::filesystem;

Tilemap::Tilemap(const Path& path) {
    setSource(path);
}

void Tilemap::setSource(const Path &path) {
    _map = tileson.parse(path);

    if (_map->getStatus() == tson::ParseStatus::OK) {
        _drawer = nullptr;
        _defaultDrawer = false;
        _textures.clear();

        _source = (fs::path)path;
        loadTilesets(fs::path(path).parent_path());

        Logger::info("Component", "Tilemap") << path << " loaded";
    } else
        Logger::error("Component", "Tilemap")
            << "Tilemap-error : " << _map->getStatusMessage();

    Logger::endline();
}

Tilemap::~Tilemap() {
    // if this tilemap uses the default drawer,
    // the 'drawer' instance was not registered into component manager
    // hence, must be destroyed manually
    if (_defaultDrawer) delete _drawer;
}

void Tilemap::Render() {
    // Creating default Drawer if no Tilemap::Drawer component added
    auto eID = entity->id();
    if (auto drawer = Drawer::instances[eID]; drawer) {
        _drawer = drawer;
    } else {
        _defaultDrawer = true;
        if (!_drawer) _drawer = new Drawer;
    }

    core::RenderManager::Get()->submit(
        [&](SDL_Renderer *renderer, Entity *camera) {
            eachLayer([&](tson::Layer &layer) {
                drawLayer(layer, renderer, camera);
            });
        });
}

void Tilemap::eachLayer(const std::function<void(tson::Layer &)> &process,
                        const std::vector<tson::LayerType> &layType) {
    for (auto &layer : _map->getLayers())
        if (layType.empty() or std::find(layType.begin(), layType.end(),
                                         layer.getType()) != layType.end())
            process(layer);
}

void Tilemap::loadTilesets(const fs::path &mapFolder) {
    for (auto &tileset : _map->getTilesets()) {
        auto imagePath = tileset.getImagePath();
        _textures.try_emplace(imagePath.string(), mapFolder / imagePath);
    }
}

void Tilemap::drawLayer(tson::Layer &layer, SDL_Renderer *renderer,
                        Entity *cameraEntity) {
    using type = tson::LayerType;
    switch (layer.getType()) {
        case type::Group:
            for (auto &layer : layer.getLayers())
                drawLayer(layer, renderer, cameraEntity);
            break;

        case type::ImageLayer:
            _drawer->drawImage(layer.getImage(), layer.getOffset(), renderer,
                               cameraEntity);
            break;

        case type::TileLayer: {
            auto tileSize = _map->getTileSize();
            auto mapSize = _map->getSize();
            auto camera = cameraEntity->get<Camera>().getBoundingBox();

            int startCol = std::max(0, (int)std::floor(camera.x / tileSize.x));
            int endCol =
                std::min(mapSize.x - 1,
                         (int)std::ceil((camera.x + camera.w) / tileSize.x));
            int startRow = std::max(0, (int)std::floor(camera.y / tileSize.y));
            int endRow =
                std::min(mapSize.y - 1,
                         (int)std::ceil((camera.y + camera.h) / tileSize.y));

            for (int i = startRow; i <= endRow; ++i) {
                for (int j = startCol; j <= endCol; ++j) {
                    if (auto tile = layer.getTileData(j, i); tile) {
                        auto tileset = tile->getTileset();
                        auto tileSize = _map->getTileSize();
                        auto &texture =
                            _textures[tileset->getImagePath().string()];

                        auto tileId = tile->getId() - tileset->getFirstgid();
                        SDL_Rect src = {
                            int(tileId % (uint32_t)tileset->getColumns() *
                                (uint32_t)tileSize.x),
                            int(tileId / (uint32_t)tileset->getColumns() *
                                (uint32_t)tileSize.y),
                            tileSize.x, tileSize.y};

                        VectorI tilePosition(tileSize.x * j - camera.x,
                                             tileSize.y * i - camera.y);
                        texture.draw(src, tilePosition, Vector(false, false),
                                     Vector(1.0, 1.0));
                    }
                }
            }
        } break;

        case type::ObjectGroup:
            for (auto &object : layer.getObjects()) {
                auto objPos = object.getPosition();
                auto objSize = object.getSize();
                SDL_Rect objBoundingRect = {objPos.x, objPos.y, objSize.x,
                                            objSize.y};

                switch (object.getObjectType()) {
                    case tson::ObjectType::Ellipse:
                        _drawer->drawEllipse(objBoundingRect, renderer,
                                             cameraEntity);
                        break;

                    case tson::ObjectType::Point:
                        _drawer->drawPoint(objPos, renderer, cameraEntity);
                        break;

                    case tson::ObjectType::Polygon:
                        _drawer->drawPolyline(objPos, object.getPolygons(),
                                              renderer, cameraEntity);
                        break;

                    case tson::ObjectType::Polyline:
                        _drawer->drawPolyline(objPos, object.getPolylines(),
                                              renderer, cameraEntity);
                        break;

                    case tson::ObjectType::Rectangle:
                        _drawer->drawRectangle(objBoundingRect, renderer,
                                               cameraEntity);
                        break;

                    case tson::ObjectType::Text:
                        _drawer->drawText(object.getText(), objPos, renderer,
                                          cameraEntity);
                        break;

                    default:
                        _drawer->drawObject(object, renderer, cameraEntity);
                        break;
                }
            }
            break;

        default:;
    }
}

std::string Tilemap::getSource() const { return _source.string(); }

}  // namespace component
}  // namespace ecs
}  // namespace entix
