#include <vector>
/**
 * An entity created to be able to add texts to entity containers
 * Also allows to have centered multi-lined text
 */

namespace platformer{
    class TextEntity : public gf::Entity, public gf::Text{
    private :
        std::vector<std::string> lines;
    public:
        void draw (RenderTarget &target, const RenderStates &states) = delete;

        TextEntity(std::string string, Font &font, unsigned characterSize=30);

        const std::string& getString () const override;
        void setString (std::string string) override;

        void addLine(std::string string);

        void render (RenderTarget &target, const RenderStates &states) override;
    };
}
