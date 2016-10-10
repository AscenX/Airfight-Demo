//
//  FKSprite.hpp
//  Airfight
//
//  Created by Ascen on 16/10/10.
//
//

#ifndef FKSprite_hpp
#define FKSprite_hpp

#include "cocos2d.h"
#include "ui/cocosGUI.h"

USING_NS_CC;
using namespace ui;

class FKSprite : public Sprite {
private:
    int _lifeValue;
    LoadingBar* _HP;
    float _HPInterval;
    
public:
    static FKSprite* createWithSpriteFrameName(const std::string& filename){
        FKSprite* sprite = new FKSprite();
        if(sprite && sprite->initWithSpriteFrameName(filename)){
            sprite->autorelease();
            return sprite;
        }
        
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    void setLifeValue(int lifeValue){
        _lifeValue = lifeValue;
    }
    
    int getLifeValue(){
        return _lifeValue;
    }
    
    void setHP(LoadingBar* HP){
        _HP = HP;
    }
    
    LoadingBar* getHP(){
        return _HP;
    }
    
    void setHPInterval(float HPInterval){
        _HPInterval = HPInterval;
    }
    
    float getHpInterval(){
        return _HPInterval;
    }
};

#endif /* FKSprite_hpp */
