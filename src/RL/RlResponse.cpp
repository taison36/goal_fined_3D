//
// Created by Tykhon Korol on 13.01.26.
//

#include "RlResponse.h"

#include <nlohmann/json.hpp>

constexpr float ROTATION_PER_FRAME = 15.0f; // Grad pro Frame

RlResponse::RlResponse(std::string const &json_str) {
    auto j = nlohmann::json::parse(json_str);

    std::string player_move_str = j.at("player_move").get<std::string>();
    if (player_move_str == "LEFT") player_move = PlayerMovement::LEFT;
    else if (player_move_str == "FORWARD") player_move = PlayerMovement::FORWARD;
    else if (player_move_str == "BACKWARD") player_move = PlayerMovement::BACKWARD;
    else if (player_move_str == "RIGHT") player_move = PlayerMovement::RIGHT;
    else throw std::runtime_error("Unknown player_move value: " + player_move_str);

    std::string mouse_move_str = j.at("mouse_move").get<std::string>();
    if (mouse_move_str == "LEFT") {
        mouse_xoffset = -ROTATION_PER_FRAME;
    } else if (mouse_move_str == "STAY") {
        mouse_xoffset = 0;
    } else if (mouse_move_str == "RIGHT") {
        mouse_xoffset = +ROTATION_PER_FRAME;
    } else {
        throw std::runtime_error("Unknown mouse_mov value: " + mouse_move_str);
    }
}
