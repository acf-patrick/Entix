#include "../components.h"
#include "../../renderer/renderer.h"

namespace Component {

    void spriteRenderer::Render()
    {
        if (!has<sprite>())
        {
            std::cerr << "SpriteRenderer-error : Entity must have a sprite component to render!" << std::endl;
            return;
        }
        
        auto& spriteComponent = get<sprite>();
        if (!spriteComponent.texture)
            return;         // no texture to draw

    Renderer->submit([&](SDL_Renderer* renderer)
    {
        if (!has<transform>())
            attach<transform>();    // default position, rotation and scale factor
        auto& t = get<transform>();
        auto& texture = spriteComponent.texture;
        auto pos = t.position;
        auto scale = t.scale;
        auto rotation = t.rotation;
        auto tSize = texture.getSize();
        SDL_Rect src;
        VectorI frameSize;
        int w, h;

        // check bounds
        if (spriteComponent.framesNumber.x <= 0 or spriteComponent.framesNumber.y <= 0 or spriteComponent.frame >= spriteComponent.framesNumber.x*spriteComponent.framesNumber.y)
            return;

        // select area
        if (spriteComponent.regionEnabled)
        {
            src.x = spriteComponent.region.x;
            src.y = spriteComponent.region.y;
            w = spriteComponent.region.w;
            h = spriteComponent.region.h;
        }
        // use whole texture
        else
        {
            src.x = src.y = 0;
            w = tSize.x;
            h = tSize.y;
        }

        // compute frame size
        frameSize.x = w / spriteComponent.framesNumber.x;
        frameSize.y = h / spriteComponent.framesNumber.y;

        // compute source rect
        src.x += (spriteComponent.frame % spriteComponent.framesNumber.x)*frameSize.x;
        src.y += (spriteComponent.frame / spriteComponent.framesNumber.x)*frameSize.y;
        src.w = frameSize.x;
        src.h = frameSize.y;

        // destination 
        pos += spriteComponent.offset;

        // center destination
        if (spriteComponent.centered)
            pos -= {src.w*0.5, src.h*0.5};
        
        // rotate around center for now
        texture.draw(src, {int(pos.x), int(pos.y)}, {src.w/2, src.h/2}, rotation, spriteComponent.flip, scale);
    });
    }

    void sprite::setTexture(const std::string& fileName)
    {
        texture.load(fileName);
    }

    void sprite::setFrame(int x, int y)
    {
        if (x >= framesNumber.x || y >= framesNumber.y)
            return;
        frame = x*framesNumber.x + y;
    }

}