//
// Created by Tykhon Korol on 13.01.26.
//

#ifndef RLRESPONSE_H
#define RLRESPONSE_H
#include <string>

#include "../render/PlayerMovement.h"



struct RlResponse {

    PlayerMovement player_move;
    float mouse_xoffset;

    explicit RlResponse(std::string const &json_str);
};



#endif //RLRESPONSE_H
