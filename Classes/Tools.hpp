//
//  Tools.hpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#ifndef Tools_hpp
#define Tools_hpp

#include "cocos2d.h"

USING_NS_CC;

class Tools {
public:
    //拉伸图片适应size
    static void adapt(Node* node, Size size);
};

#endif /* Tools_hpp */
