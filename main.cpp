#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<box2d/box2d.h>
class background {
private:
    sf::Texture bgtexture;
    sf::Sprite bgsprite;
    
public:
    background(const sf::Vector2u& windowSize, const std::string& imgpath)
        :bgtexture(),bgsprite(bgtexture)
    {
        if (!bgtexture.loadFromFile(imgpath))
        {
            std::cout << "����" << std::endl;
            exit(0);
        }
        bgsprite = sf::Sprite(bgtexture);
        bgsprite.setScale(sf::Vector2f(1.0f,1.0f));
    }
    void setPosition(float x, float y) {
        bgsprite.setPosition(sf::Vector2f(x, y));
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(bgsprite);
    }
    sf::Vector2f getSize() const {
        return bgsprite.getGlobalBounds().size;
    }
};

struct Animframe {
    Animframe(float time = 0.0f,sf::Texture texture = sf::Texture())
        :time(time),texture(texture)
    {
    }
    float time;
    sf::Texture texture;
};

class Animation {
public:
    Animation(float length = 0.0f, std::vector<Animframe>frames = {})
        :length(length),frames(std::move(frames))
    {
        if (this->frames.empty() && length > 0)
        {
            exit(0);
        }
    }
    void update(float deltatime)
    {
        if (frames.empty())
        {
            return;
        }
        time += deltatime;
        totaltime += deltatime;
        while (time > frames[currentframe].time)
        {
            time -= frames[currentframe].time;
            currentframe = (currentframe + 1) % frames.size();
        }
    }
    void reset()
    {
        time = 0.0f;
        currentframe = 0;
        totaltime = 0.0f;
    }
    float getplaytime()
    {
        return totaltime;
    }
    sf::Texture& gettexture()
    {
        static sf::Texture emptyTex; // ������
        if (this->frames.empty())
        {
            return emptyTex;
        }
        else
            return frames[currentframe].texture;
    }
private:
    float length;
    std::vector<Animframe>frames;
    float time = 0.0f;
    size_t currentframe = 0;
    float totaltime = 0.0f;
};

enum class Animstate {
    idle,
    run,
    jump,
    attack1,
    attack2,
    quick
};

enum class gamestate
{
    game,
    begin
};

class sasuke 
{
public:
    sasuke(b2WorldId& world, const sf::Vector2f& positionx, sf::Sound& sound, sf::Sound& sound2)
        :sprite(idletex[0]),firesprite(idletex[0]),clothsprit(clothtex[0]),sound(sound),sound2(sound2),world(world)
    {
        b2BodyDef b2bodydef = b2DefaultBodyDef();
        b2bodydef.type = b2_dynamicBody;
        b2bodydef.fixedRotation = true;
        b2bodydef.position.x = positionx.x / 32.0f;
        b2bodydef.position.y = positionx.y / 32.0f;
        body = b2CreateBody(world, &b2bodydef);
        b2Polygon dynamicBox = b2MakeBox(2.1875f, 2.1875f);
        b2ShapeDef shapedef = b2DefaultShapeDef();
        b2ShapeDef firedef = b2DefaultShapeDef();
        b2ShapeDef clothdef = b2DefaultShapeDef();
        firedef.isSensor = true;
        clothdef.isSensor = true;
        clothdef.material.friction = 0.0f;
        firedef.material.friction = 0.0f;
        b2Polygon fireBoxR = b2MakeOffsetBox(3.0f, 1.5f, { 4.0f, 0.0f }, b2Rot_identity);
        b2Polygon fireBoxL = b2MakeOffsetBox(3.0f, 1.5f, { -4.0f, 0.0f }, b2Rot_identity);
        b2Polygon clothBoxR = b2MakeOffsetBox(2.0f, 1.0f, { 2.0f, 0.0f }, b2Rot_identity);
        b2Polygon clothBoxL = b2MakeOffsetBox(2.0f, 1.0f, { -2.0f, 0.0f }, b2Rot_identity);
        b2Polygon clothbox = b2MakeOffsetBox(1.0f, 1.0f, { 0.0f,0.0f }, b2Rot_identity);
        shapedef.density = 1.0f;
        shapedef.material.friction = 0.1f;  
        b2CreatePolygonShape(body, &shapedef, &dynamicBox);
        fireShapes[0] = b2CreatePolygonShape(body, &firedef, &fireBoxR);
        fireShapes[1] = b2CreatePolygonShape(body, &firedef, &fireBoxL);
        clothShapes[0] = b2CreatePolygonShape(body, &clothdef, &clothBoxR);
        clothShapes[1] = b2CreatePolygonShape(body, &clothdef, &clothBoxL);
        //���˸����������
        const std::string heropath[3] = { "resources/1.png", "resources/2.png", "resources/3.png" };
        const std::string quick[5] = { "resources/1.png", "resources/A1.png", "resources/A2.png","resources/A3.png","resources/1.png"  };
        const std::string run[6] = { "resources/4.png","resources/5.png", "resources/6.png","resources/7.png", "resources/8.png", "resources/9.png" };
        const std::string jump[2] = {"resources/JUMP2.png","resources/JUMP3.png"};
        const std::string attack[8] = { 
    "resources/fire1.png", "resources/fire2.png", "resources/fire3.png",
    "resources/fire4.png", "resources/fire5.png", "resources/fire6.png",
    "resources/fire6.png", "resources/fire6.png" 
};
        const std::string fire[7] = { "resources/A.png","resources/B.png", "resources/C.png","resources/D.png", "resources/E.png", "resources/F.png","resources/G.png" };
        const std::string cloth[8] = { "resources/11.png","resources/12.png", "resources/13.png","resources/14.png", "resources/15.png", "resources/16.png","resources/17.png" ,"resources/18.png" };
        for (int i = 0;i < 3; ++i)
        {
            if (!idletex[i].loadFromFile(heropath[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        for (int i = 0; i < 6; ++i)
        {
            if (!runtex[i].loadFromFile(run[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        for (int i = 0; i < 5; ++i)
        {
            if (!quicktex[i].loadFromFile(quick[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        for (int i = 0; i < 2; ++i)
        {
            if (!jumptex[i].loadFromFile(jump[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        for (int i = 0; i < 8; ++i)
        {
            if (!attacktex[i].loadFromFile(attack[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        for (int i = 0; i < 7; ++i)
        {
            if (!firetex[i].loadFromFile(fire[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        for (int i = 0; i < 8; ++i)
        {
            if (!clothtex[i].loadFromFile(cloth[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }

        std::vector<Animframe>idleframes;
        std::vector<Animframe>runframes;
        std::vector<Animframe>jumpframes;
        std::vector<Animframe>attackframes;
        std::vector<Animframe>fireframes;
        std::vector<Animframe>clothframes;
        std::vector<Animframe>quickframes;


        for (auto& tex : idletex)
        {
            idleframes.emplace_back(0.1f, tex);
        }
        for (auto& run : runtex)
        {
            runframes.emplace_back(0.1f, run);
        }
        for (auto& jump : jumptex)
        {
            jumpframes.emplace_back(0.1f, jump);
        }
        for (auto& attack : attacktex)
        {
            attackframes.emplace_back(0.2f, attack);
        }
        for (auto& fire : firetex)
        {
            fireframes.emplace_back(0.6f / 7.0f, fire);
        }
        for (auto& cloth : clothtex)
        {
            clothframes.emplace_back(0.15f,cloth );
        }
        for (auto& son : quicktex)
        {
            quickframes.emplace_back(0.1f, son);
        }
        sprite = sf::Sprite(idletex[0]);
        clothsprit = sf::Sprite(clothtex[6]);
        firesprite = sf::Sprite(firetex[0]);
        idleanim = Animation(0.4f, idleframes);
        runanim = Animation(0.6f, runframes);
        jumpanim = Animation(0.2f, jumpframes);
        attack1 = Animation(1.6f, attackframes);
        fireanim = Animation(0.6f, fireframes);
        clothanim = Animation(1.2f, clothframes);
        quickanim = Animation(0.5f, quickframes);
        //�����������
        sprite.setPosition(positionx);
        updateSpriteOrigin(idletex[0]);
        updateSpriteOrigin(firetex[0], firesprite);
        sf::Vector2u size = clothtex[0].getSize();
        clothsprit.setOrigin(sf::Vector2f(0.0f, static_cast<float>(size.y) / 2.0f));
        quickPlayTime = 0.0f;
        isQuickPlaying = false;
    }
    void updateSpriteOrigin(const sf::Texture& tex) {
        sprite.setOrigin(sf::Vector2f(            static_cast<float>(tex.getSize().x) / 2.0f,
            static_cast<float>(tex.getSize().y) / 2.0f
        ));
    }
    void updateSpriteOrigin(const sf::Texture& tex, sf::Sprite& targetsprite) {
        targetsprite.setOrigin(sf::Vector2f(static_cast<float>(tex.getSize().x) / 2.0f,
            static_cast<float>(tex.getSize().y) / 2.0f
        ));
    }

    b2AABB getFireAABB() const {
        sf::FloatRect rect = firesprite.getGlobalBounds();
        b2AABB aabb;
        // ����������ת��Ϊ������������ (���� / 32)
        aabb.lowerBound = { rect.position.x / 32.0f, rect.position.y / 32.0f };
        aabb.upperBound = { (rect.position.x + rect.size.x) / 32.0f, (rect.position.y + rect.size.y) / 32.0f };
        return aabb;
    }

    b2AABB getclothAABB() const {
        sf::FloatRect rect = clothsprit.getGlobalBounds();
        b2AABB aabb;
        // ����������ת��Ϊ������������ (���� / 32)
        aabb.lowerBound = { rect.position.x / 32.0f, rect.position.y / 32.0f };
        aabb.upperBound = { (rect.position.x + rect.size.x) / 32.0f, (rect.position.y + rect.size.y) / 32.0f };
        return aabb;
    }

   
    b2BodyId getBody() const { return body; }
    void update(float deltatime)
    {
        if (isQuickPlaying) {
            quickPlayTime += deltatime;
            if (quickPlayTime >= 0.5f)
            {
                isQuickPlaying = false;
                quickPlayTime = 0.0f;
                switchanimstate(Animstate::idle);
            }
        }
        b2Vec2 current = b2Body_GetLinearVelocity(body);
        switch (currentstate)
        {
        case Animstate::idle:
            idleanim.update(deltatime);
            sprite.setTexture(idleanim.gettexture());
            break;
        case Animstate::run:
            runanim.update(deltatime);
            sprite.setTexture(runanim.gettexture());
            break;
        case Animstate::jump:
            jumpanim.update(deltatime);
            sprite.setTexture(jumpanim.gettexture());
            break;
        case Animstate::quick:
            quickanim.update(deltatime);
            sprite.setTexture(quickanim.gettexture());
            break;
        case Animstate::attack2:
        {
            if (!havesound2)
            {
                sound2.play();
                havesound2 = true;
            }
            clothanim.update(deltatime);
            clothsprit.setTexture(clothanim.gettexture());
            int dir = getface();
            if(dir == -1)
            {
                clothsprit.setScale(sf::Vector2f(-2.0f, 2.0f));
            }
            else
            {
                clothsprit.setScale(sf::Vector2f(2.0f, 2.0f));
            }
            updateSpriteOrigin(clothanim.gettexture(), clothsprit);
            sf::Vector2u texSize = clothanim.gettexture().getSize();
            clothsprit.setOrigin(sf::Vector2f(0.0f, static_cast<float>(texSize.y) / 2.0f));
            float attack2time = clothanim.getplaytime();
            sf::Vector2f playerPos = sprite.getPosition();
            
            clothsprit.setPosition(sf::Vector2f(playerPos.x - dir * 60.0f,playerPos.y));
            idleanim.update(deltatime);
            sprite.setTexture(idleanim.gettexture());
            if (attack2time >= 1.2f)
            {
                iscloth = false;
                havesound2 = false;
                clothanim.reset();
                idleanim.reset();
                switchanimstate(Animstate::idle);
            }
            break;
        }
        case Animstate::attack1:
        {
            if (!havesound) {
                sound.play();
                havesound = true;
            }
            attack1.update(deltatime);
            sprite.setTexture(attack1.gettexture());
            float attacktime = attack1.getplaytime();
            showfire = (attacktime >= 1.0f && attacktime <= 1.6f);
            if (showfire)
            {
                fireanim.update(deltatime);
                firesprite.setTexture(fireanim.gettexture());
                int dir = getface();
                b2Transform localTransform = b2Transform_identity;
                if (dir == -1)
                {
                    firesprite.setScale(sf::Vector2f(-2.0f, 2.0f));
                }
                else
                {
                    firesprite.setScale(sf::Vector2f(2.0f, 2.0f));
                }
                
                firesprite.setRotation(sprite.getRotation());
                sf::Vector2f playerPos = sprite.getPosition();
                
                float offsetX = dir * 250.0f;
                firesprite.setPosition(sf::Vector2f(playerPos.x + offsetX, playerPos.y));
            }
            if (attacktime >= 1.6f) {
                isshowfire = false;
                havesound = false;
                showfire = false;
                attack1.reset();
                fireanim.reset();
                switchanimstate(Animstate::idle);
            }
            break;
        }
        
        }
        b2Vec2 pos = b2Body_GetPosition(body);
        b2Transform transform = b2Body_GetTransform(body);
        float angle = b2Rot_GetAngle(transform.q);
        sprite.setPosition(sf::Vector2f(pos.x * 32.0f, pos.y * 32.0f));
        sprite.setRotation(sf::degrees(angle));
    }

    int getface() const
    {
        return sprite.getScale().x > 0 ? 1 : -1;
    }

    bool isonground() const
    {
        b2Vec2 bodyPos = b2Body_GetPosition(body);
        b2AABB groundAabb;
        groundAabb.lowerBound.x = bodyPos.x - 1.8f;
        groundAabb.upperBound.x = bodyPos.x + 1.8f;
        groundAabb.lowerBound.y = bodyPos.y + 0.05f + 2.1875f;
        groundAabb.upperBound.y = bodyPos.y + 0.1f + 2.1875f;
        b2QueryFilter filter = b2DefaultQueryFilter();
        OverlapCallbackState callbackState;
        callbackState.player = body;
        callbackState.touching = false;

        b2World_OverlapAABB(
            world,                // ��������ID
            groundAabb,             // ����ײ��Ĳ�ѯAABB
            filter,                 // ��ѯ������
            &sasuke::OverlapCallbackStatic,        // �Զ���ص�����
            &callbackState                 // ������ָ�루�˴����贫�ݣ���Ϊnullptr��
        );
        return callbackState.touching;
    }

    b2ShapeId getActiveFireShape() const {
        int idx = (getface() == 1) ? 0 : 1; // 1���ң�-1����
        return fireShapes[idx];
    }

    b2ShapeId getActiveClothShape() const {
        int idx = (getface() == 1) ? 0 : 1;
        return clothShapes[idx];
    }


    void handinput(bool onground)
    {
        b2Vec2 current = b2Body_GetLinearVelocity(body);
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
            isQuickPressed = false;
        }
        if (isshowfire || iscloth || isQuickPlaying)
        {
            return;
        }
        if (onground) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                b2Body_SetLinearVelocity(body, b2Vec2{-5.0f, current.y});
                sprite.setScale(sf::Vector2f(-1.f, 1.f));
                updateSpriteOrigin(runanim.gettexture());
                switchanimstate(Animstate::run);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                b2Body_SetLinearVelocity(body, b2Vec2{5.0f, current.y});
                sprite.setScale(sf::Vector2f(1.f, 1.f));
                updateSpriteOrigin(runanim.gettexture());
                switchanimstate(Animstate::run);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) && !isshowfire)
            {
                updateSpriteOrigin(attack1.gettexture());
                switchanimstate(Animstate::attack1);
                isshowfire = true;
                havesound = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                b2Body_SetLinearVelocity(body, b2Vec2{current.x, -12.0f});
                updateSpriteOrigin(jumpanim.gettexture());
                switchanimstate(Animstate::jump);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
            {
                updateSpriteOrigin(idleanim.gettexture());
                switchanimstate(Animstate::attack2);
                iscloth = true;
                havesound2 = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) && !isQuickPressed)
            {
                isQuickPressed = true;
                isQuickPlaying = true; 
                quickPlayTime = 0.0f;
                updateSpriteOrigin(quickanim.gettexture());
                switchanimstate(Animstate::quick);
                b2Vec2 currentPos = b2Body_GetPosition(body);
                b2Rot currentrot = b2Body_GetRotation(body);
                int dir = getface();
                b2Body_SetTransform(body, b2Vec2{currentPos.x + dir * 5.0f, currentPos.y}, currentrot);
            }
            else {
                b2Body_SetLinearVelocity(body, b2Vec2{0.0f, current.y});
                switchanimstate(Animstate::idle);
                
                updateSpriteOrigin(idleanim.gettexture());
            }
        }
        if (!onground) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) && !isQuickPressed)
            {
                isQuickPressed = true;
                isQuickPlaying = true; 
                quickPlayTime = 0.0f;
                updateSpriteOrigin(quickanim.gettexture());
                switchanimstate(Animstate::quick);
                b2Vec2 currentPos = b2Body_GetPosition(body);
                b2Rot currentrot = b2Body_GetRotation(body);
                int dir = getface();
                b2Body_SetTransform(body, b2Vec2{currentPos.x + dir * 5.0f, currentPos.y}, currentrot);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                b2Body_SetLinearVelocity(body, b2Vec2{5.0f, current.y});
                sprite.setScale(sf::Vector2f(1.f, 1.f));
                updateSpriteOrigin(jumpanim.gettexture());
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                b2Body_SetLinearVelocity(body, b2Vec2{-5.0f, current.y});
                sprite.setScale(sf::Vector2f(-1.f, 1.f));
                updateSpriteOrigin(jumpanim.gettexture());
            }
            else {
                switchanimstate(Animstate::jump);
                sprite.setTexture(jumpanim.gettexture());
                updateSpriteOrigin(jumpanim.gettexture());
            }
        }
        
    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
        if (showfire) {
            window.draw(firesprite);
        }
        if (iscloth) {
            window.draw(clothsprit);
        }
    }

    void switchanimstate(Animstate newstate)
    {
        if (currentstate == newstate)
        {
            return;
        }
        if (newstate != Animstate::attack1) {
            showfire = false;
            fireanim.reset();
            attack1.reset();
        }
        if (newstate != Animstate::attack2) {
            iscloth = false;
            clothanim.reset();
        }
        
        currentstate = newstate;
    }

    float getHealthPercentage() const {
        return currentHP / maxHP;
    }

    void takeDamage(float damage) {
        currentHP -= damage;
        if (currentHP < 0) currentHP = 0;
    }

    bool isFiring() const { return showfire; }
    bool isUsingCloth() const { return iscloth; }
private:
    b2ShapeId fireShapes[2];  // 0: ��, 1: ��
    b2ShapeId clothShapes[2]; // 0: ��, 1: ��
    b2WorldId world;
    sf::Texture idletex[3];
    sf::Texture runtex[6];
    sf::Texture jumptex[2];
    sf::Texture attacktex[8];
    sf::Texture firetex[7];
    sf::Texture clothtex[8];
    sf::Texture quicktex[5];
    sf::Sprite sprite;
    sf::Sprite firesprite;
    sf::Sprite clothsprit;
    b2BodyId body;
    Animation idleanim;
    Animation runanim;
    Animation jumpanim;
    Animation attack1;
    Animation fireanim;
    Animation clothanim;
    Animation quickanim;
    Animstate currentstate = Animstate::idle;
    sf::Sound sound;
    sf::Sound sound2;
    float groundy;
    bool showfire = false;
    bool isshowfire = false;
    bool havesound = false;
    bool iscloth = false;
    bool havesound2 = false;
    bool isQuickPressed = false;
    bool isQuickPlaying = false;   
    float quickPlayTime = 0.0f;
    float currentHP = 100.0f;
    float maxHP = 100.0f;
    struct OverlapCallbackState
    {
        b2BodyId player;
        bool touching;
    };
        static bool OverlapCallbackStatic(b2ShapeId shapeId, void* context)
        {
            OverlapCallbackState* state = static_cast<OverlapCallbackState*>(context);
            if (state == nullptr) return true;
            b2BodyId hitBody = b2Shape_GetBody(shapeId);
            if (hitBody.index1 == state->player.index1 && hitBody.generation == state->player.generation)
            {
                return true;
            }
            b2BodyType bodyType = b2Body_GetType(hitBody);
            if (bodyType == b2_staticBody)
            {
                state->touching = true;
                return false;
            }
            return true;
        }
};

class menuui
{
private:
    sf::Font font;
    sf::Text enter;
    sf::Texture begintexture;
    sf::Sprite beginsprite;
public:
    menuui(const sf::Vector2u& windowsize, const std::string& imgpath, const std::string& fontpath)
        :beginsprite(begintexture),enter(font,"ENTER")
    {
        if (!font.openFromFile(fontpath))
        {
            std::cerr << "baocuo" << std::endl;
        }
        enter.setFont(font);
        enter.setString("ENTER");
        enter.setFillColor(sf::Color::Blue);
        enter.setOutlineColor(sf::Color::Black);
        enter.setOutlineThickness(2);
        enter.setPosition(sf::Vector2f((windowsize.x - enter.getLocalBounds().size.x) / 2.0f, windowsize.y / 2.0f));
        if (!begintexture.loadFromFile(imgpath))
        {
            std::cerr << "chucuo" << std::endl;
        }
        beginsprite = sf::Sprite(begintexture);
        beginsprite.setScale(sf::Vector2f((float)windowsize.x / begintexture.getSize().x, (float)windowsize.y / begintexture.getSize().y));
    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(beginsprite);
        window.draw(enter);
    }
    bool entergame(sf::RenderWindow& window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            return true;
        }
        sf::Vector2i mice = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mice);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && enter.getGlobalBounds().contains(mouseWorldPos))
        {
            return true;
        }
        return false;
    }
};

void updateCamera(sf::View& view, b2BodyId playerBody, const sf::Vector2u& windowSize, const sf::Vector2f& worldSize) {
    b2Vec2 pos = b2Body_GetPosition(playerBody);
    float screenX = pos.x * 32.0f;
    float screenY = pos.y * 32.0f;
    // ������ĵ����
    float centerX = screenX;
    float centerY = screenY;
    // ��ֹ�����������ͼ�߽�
    centerX = std::max(windowSize.x / 2.0f, std::min(centerX, worldSize.x - windowSize.x / 2.0f));
    centerY = std::max(windowSize.y / 2.0f, std::min(centerY, worldSize.y - windowSize.y / 2.0f));
    // ������ĵ�
    view.setCenter(sf::Vector2f(centerX, centerY));
}

void drawplayform(int size,float boxHalfWidth,float boxHalfHeight,sf::Texture& texture, b2BodyDef& ground, sf::RenderWindow& window)
{
    float groundPixelWidth = boxHalfWidth * 2 * 32.0f;  
    float groundPixelHeight = boxHalfHeight * 2 * 32.0f;
    int xCount = static_cast<int>(ceil(groundPixelWidth / size)); 
    int yCount = static_cast<int>(ceil(groundPixelHeight / size));
    float groundOriginX = (ground.position.x * 32.0f) - (groundPixelWidth / 2.0f);
    float groundOriginY = (ground.position.y * 32.0f) - (groundPixelHeight / 2.0f);
    for (int x = 0; x < xCount; x++)
    {
        for (int y = 0; y < yCount; y++)
        {
            sf::Sprite sprite(texture);
            sprite.setTextureRect(sf::IntRect({ 0,0 }, { size,size }));
            sprite.setPosition(sf::Vector2f(groundOriginX + x * size, groundOriginY + y * size));
            window.draw(sprite);
        }
    }
}

class healthy
{
private:
    sf::RectangleShape backbar;
    sf::RectangleShape frontbar;
    sf::Vector2f size;
public:
    healthy(float w, float h)
        :size(sf::Vector2f(w,h))
    {
        backbar.setSize(size);
        backbar.setFillColor(sf::Color(50,50,50,200));
        backbar.setOutlineColor(sf::Color::Black);
        backbar.setOutlineThickness(2);
        frontbar.setSize(size);
        frontbar.setFillColor(sf::Color::Red);
       
    }
    void update(float percentage) {
        frontbar.setSize(sf::Vector2f(size.x * percentage, size.y));
    }
    void draw(sf::RenderWindow& window,sf::Vector2f screenPos)
    {
        sf::View currentView = window.getView();
        window.setView(window.getDefaultView());

        backbar.setPosition(screenPos);
        frontbar.setPosition(screenPos);

        window.draw(backbar);
        window.draw(frontbar);

        window.setView(currentView);
    }
    void drawemeny(sf::RenderWindow& window, sf::Vector2f worldPos)
    {
        backbar.setPosition(worldPos);
        frontbar.setPosition(worldPos);
        window.draw(backbar);
        window.draw(frontbar);
    }

};

class ban
{
private:
    sf::Sprite sprite;
    sf::Texture texture[4];
    b2BodyId body;
    Animation idleanim;
    float currentHP = 100.0f;
    float maxHP = 100.0f;
public:
    ban(b2WorldId& world, const sf::Vector2f& positionx)
        :sprite(texture[0])
    {
        b2BodyDef b2bodydef = b2DefaultBodyDef();
        b2bodydef.type = b2_dynamicBody;
        b2bodydef.fixedRotation = true;
        b2bodydef.position.x = positionx.x / 32.0f;
        b2bodydef.position.y = positionx.y / 32.0f;
        body = b2CreateBody(world, &b2bodydef);
        b2Polygon dynamicBox = b2MakeBox(1.25f,2.5f);
        b2ShapeDef shapedef = b2DefaultShapeDef();
        shapedef.density = 1.0f;
        shapedef.material.friction = 0.5f;
        b2CreatePolygonShape(body, &shapedef, &dynamicBox);
        //���崴�����
        const std::string heropath[4] = {"resources/B1.png","resources/B2.png","resources/B3.png","resources/B4.png"};
        for (int i = 0; i < 4;i++)
        {
            if (!texture[i].loadFromFile(heropath[i]))
            {
                std::cerr << "����ʧ��" << std::endl;
            }
        }
        std::vector<Animframe>idleframes;
        for (auto& son : texture)
        {
            idleframes.emplace_back(0.1f, son);
        }
        idleanim = Animation(0.4f, idleframes);
        sprite = sf::Sprite(texture[0]);
        sf::Vector2u texSize = texture[0].getSize();
        sprite.setOrigin({ texSize.x / 2.0f, texSize.y / 2.0f });
        sprite.setPosition(positionx);
    }
    void draw(sf::RenderWindow& window)
    {
        if (currentHP <= 0) return;
        window.draw(sprite);
    }
    float getHealthPercentage() const {
        return currentHP / maxHP;
    }

    // �����Ҫֱ�ӻ�ȡ��ǰѪ��
    float getCurrentHP() const {
        return currentHP;
    }
    void update(float deltatime)
    {
        idleanim.update(deltatime);
        
        b2Vec2 pos = b2Body_GetPosition(body);
        b2Transform transform = b2Body_GetTransform(body);
        float angle = b2Rot_GetAngle(transform.q);
        sprite.setPosition(sf::Vector2f(pos.x * 32.0f, pos.y * 32.0f));
        sprite.setRotation(sf::degrees(angle));
        sprite.setScale({ 2.0f,2.0f });
        sprite.setTexture(idleanim.gettexture());
    }
    void takeDamage(float damage) {
        currentHP -= damage;
        if (currentHP < 0) currentHP = 0;
    }
        
    b2BodyId getBodyId() const { return body; }
    bool isAlive() const { return currentHP > 0; }
};

bool CheckAABBOverlap(const b2AABB& a, const b2AABB& b) {
    bool d1 = b.lowerBound.x > a.upperBound.x; // b �� a �ұ�
    bool d2 = b.upperBound.x < a.lowerBound.x; // b �� a ���
    bool d3 = b.lowerBound.y > a.upperBound.y; // b �� a �±�
    bool d4 = b.upperBound.y < a.lowerBound.y; // b �� a �ϱ�

    // ��������ĸ������������㣬˵�������ص���
    return !(d1 || d2 || d3 || d4);
}

// �������������Ӿ� AABB �� ���� Shape AABB �Ա�
bool PreciseCollisionTest(const b2AABB& attackAABB, b2ShapeId enemyShape) {
    b2AABB enemyAABB = b2Shape_GetAABB(enemyShape);
    return CheckAABBOverlap(attackAABB, enemyAABB);
}

int main() {
    sf::Vector2f worldSize(60.0f * 32.0f, 33.75f * 32.0f);
    const std::string bgpath = "resources/map.png";
    const std::string menupath = "resources/MENU2.png";
    const std::string fontpath = "resources/arial.ttf";
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("resources/meau.ogg"))
        std::cerr << '666' << std::endl;
    sf::Sound sound(buffer);
    sf::SoundBuffer buffer1;
    if (!buffer1.loadFromFile("resources/attack.ogg"))
        std::cerr << '666' << std::endl;
    sf::Sound sound1(buffer1);
    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile("resources/tianzhao.mp3"))
        std::cerr << '666' << std::endl;
    sf::Sound sound2(buffer2);
    //��Ƶ��Դ����
    const std::string birckpath = "resources/brick.png";
    const std::string grasspath = "resources/grass.png";
    sf::Texture grass;
    sf::Texture brick;
    if (!brick.loadFromFile(birckpath))
    {
        std::cerr << '777' << std::endl;
    }
    if (!grass.loadFromFile(grasspath))
    {
        std::cerr << '777' << std::endl;
    }
    gamestate state = gamestate::begin;
    sf::RenderWindow window(sf::VideoMode({ 1200, 1080 }), "folk");
    sf::Clock deltaClock;
    window.setFramerateLimit(60);
    sf::View crame = sf::View::View(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2),sf::Vector2f(window.getSize().x, window.getSize().y));
    background bg(window.getSize(),bgpath);
    bg.setPosition(0.0f, 0.0f);
    menuui menu(window.getSize(), menupath,fontpath);
    //��������
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = b2Vec2{0.0f, 9.8f};
    b2WorldId id = b2CreateWorld(&world);
    //���
    b2BodyDef ground = b2DefaultBodyDef();
    ground.type = b2_staticBody;
    ground.position.x = window.getSize().x / 2.0f / 32.0f;
    ground.position.y = window.getSize().y / 32.0f -2.0f;
    b2BodyId groundid = b2CreateBody(id,&ground);
    b2Polygon groundbox = b2MakeBox(50.0f, 2.0f);
    b2ShapeDef groundshape = b2DefaultShapeDef();
    groundshape.material.friction = 0.8f;
    b2CreatePolygonShape(groundid, &groundshape, &groundbox);
    //������һ������
    b2BodyDef playstatue1 = b2DefaultBodyDef();
    playstatue1.type = b2_staticBody;
    playstatue1.position.x = 20.0f;
    playstatue1.position.y = 24.0f;
    b2BodyId playstatuelid = b2CreateBody(id, &playstatue1);
    b2Polygon playstatuelbox = b2MakeBox(3.0f, 0.2f);
    b2ShapeDef playstatueldef = b2DefaultShapeDef();
    b2CreatePolygonShape(playstatuelid, &playstatueldef,&playstatuelbox);
    //ƽ̨1
    b2BodyDef playstatue2 = b2DefaultBodyDef();
    playstatue2.type = b2_staticBody;
    playstatue2.position.x = 25.0f;
    playstatue2.position.y = 18.0f;
    b2BodyId playstatuelid2 = b2CreateBody(id, &playstatue2);
    b2Polygon playstatuelbox2 = b2MakeBox(3.0f, 0.2f);
    b2ShapeDef playstatueldef2 = b2DefaultShapeDef();
    b2CreatePolygonShape(playstatuelid2, &playstatueldef2, &playstatuelbox2);
    //ƽ̨2
    sasuke player(id, sf::Vector2f(400, 600),sound1, sound2);
    ban emeny(id, sf::Vector2f(200, 600));
    healthy playerHP(400.0f, 25.0f);
    healthy enemyUIHP(400.0f, 25.0f);
    bool is_music = true;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        float deltatime = deltaClock.restart().asSeconds();
        if (state == gamestate::begin)
        {
            if (menu.entergame(window))
            {
                state = gamestate::game;
                sound.stop();
            }
        }
        window.clear(sf::Color::White);
        if (state == gamestate::begin) {
            menu.draw(window);
            if (is_music)
            {
                sound.play();
                is_music = false;
            }
        }
        else if (state == gamestate::game)
        {
            b2World_Step(id, 1.0f / 60.0f, 3);
            b2ShapeId banshape;
            int count = b2Body_GetShapes(emeny.getBodyId(), &banshape, 1);

            if (count > 0) {
                // ��ݼ��
                if (player.isFiring()) {
                    // ������Զ����� firesprite �� 140x140 ���غ� setScale �����ʵ������м���
                    b2AABB currentFireAABB = player.getFireAABB();
                    if (PreciseCollisionTest(currentFireAABB, banshape)) {
                        emeny.takeDamage(0.6f); // ÿһ֡���˺�
                    }
                }

                // �������
                if (player.isUsingCloth()) {
                    // ������Զ���ȡ��ǰ֡ͼƬ����ʵ����
                    b2AABB currentClothAABB = player.getclothAABB(); 
                    if (PreciseCollisionTest(currentClothAABB, banshape)) {
                        emeny.takeDamage(1.0f);
                    }
                }
     
            }
            updateCamera(crame, player.getBody(), window.getSize(), worldSize);
            playerHP.update(player.getHealthPercentage());
            enemyUIHP.update(emeny.getHealthPercentage());
            window.setView(crame);
            bg.draw(window);
 
            drawplayform(16, 50.0f,2.0f, brick, ground, window);
            drawplayform(16,3.0f, 0.2f, grass, playstatue1, window);
            drawplayform(16, 3.0f, 0.2f, grass, playstatue2, window);

            emeny.update(deltatime);
            emeny.draw(window);

            bool isPlayerOnGround = player.isonground();
            player.handinput(isPlayerOnGround);
            player.update(deltatime);
            player.draw(window);

            playerHP.draw(window, { 20, 20 });
            enemyUIHP.draw(window, { 780.0f, 20.0f });
        }
        window.display();
    }
    b2DestroyWorld(id);
    return 0;
}
