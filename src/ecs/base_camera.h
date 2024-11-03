/**
 * @author acf-patrick (miharisoap@gmail.com)
 *
 * Interface for the camera
 */

#pragma once

namespace entix {
namespace ecs {

class Entity;

class ICamera {
   public:
    Entity* entity;
    virtual ~ICamera() = default;

   protected:
    virtual void _attachTransform() = 0;
    friend class Entity;
};

}  // namespace ecs
}  // namespace entix
