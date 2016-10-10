//
//  AudioControlScene.hpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#ifndef AudioControlScene_hpp
#define AudioControlScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class AudioControl: public Layer {
public:
    virtual bool init();
    static Scene* createScene();
    
    CREATE_FUNC(AudioControl);
};

#endif /* AudioControlScene_hpp */
