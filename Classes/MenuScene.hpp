//
//  MenuScene.hpp
//  Airfight
//
//  Created by Ascen on 16/10/8.
//
//

#ifndef MenuScene_hpp
#define MenuScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class GameMenu : public Layer {
public:
    virtual bool init();
    static Scene* createScene();
    
    CREATE_FUNC(GameMenu);
};

#endif /* MenuScene_hpp */
