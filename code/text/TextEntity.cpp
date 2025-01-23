
namespace platformer{
    TextEntity::TextEntity(std::string string, Font &font, unsigned characterSize=30){
        std::string current = "";
        for(char c : string){
            if(c=="\n"){

            }
        }
    }



    const std::string& getString () const override;
    void setString (std::string string) override;

    void addLine(std::string string);

    void render (RenderTarget &target, const RenderStates &states) override;
}
