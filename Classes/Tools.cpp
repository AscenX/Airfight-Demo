//
//  Tools.cpp
//  Airfight
//
//  Created by Ascen on 16/10/9.
//
//

#include "Tools.hpp"

void Tools::adapt(Node* node, Size size){
    
    float spx = node->getContentSize().width;
    float spy = node->getContentSize().height;
    node->setScaleX(size.width/spx); //设置精灵宽度缩放比例
    node->setScaleY(size.height/spy);
}
