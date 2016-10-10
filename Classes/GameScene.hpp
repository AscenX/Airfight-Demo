//
//  GameScene.hpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "FKSprite.hpp"

USING_NS_CC;

class Game : public Layer {
private:
    Sprite* _plane;
    int _screenWidth, _screenHeight;
    
    Sprite* _bg1;
    Sprite* _bg2;
    
    void update(float delta);
    void updateBackground();
    
    int _count;
    Vector<FKSprite*> _enemyVector;
    
    void enemyPlaneDive(float delta);
    void removeEnemy(float delta);
    
public:
    virtual bool init();
    static Scene* createScene();
    virtual void onEnter();
    Animation* getAnimationByName(std::string animName, float delay, int animNum);
    
    
    
    CREATE_FUNC(Game);
};

#endif /* GameScene_hpp */
