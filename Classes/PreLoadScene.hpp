//
//  PreLoadScene.hpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

//预加载界面，显示一个进度条，进度条显示完毕则代表加载完毕，加载完毕后显示主场景

#ifndef PreLoadScene_hpp
#define PreLoadScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class PreLoad : public Layer {
public:
    virtual bool init();
    static Scene* createScene();
    
    virtual void onEnterTransitionDidFinish();
    void loadMusic(ValueVector musicFiles);
    void loadEffect(ValueVector effectFiles);
    void loadSpriteSheets(ValueVector spriteSheets);
    //更新进度条
    void progressUpdate();
    

    
    CREATE_FUNC(PreLoad);
    
protected:
    //资源总数
    int _sourceCount;
    //进度条
    ProgressTimer* _progress;
    //进度条更新的次数
    float _progressInterval;
};

#endif /* PreLoadScene_hpp */
