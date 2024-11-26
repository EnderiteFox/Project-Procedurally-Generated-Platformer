#include <gf/Entity.h>
#include <gf/Vector.h>
#include <gf/Sprite.h>

namespace platformer{
    class Block : public gf::Entity{
    public:
        Block(gf::Vector2f position, const gf::Texture& texture);
        virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    private:
        gf::Vector2f position;
        gf::Sprite sprite;
    };
}
