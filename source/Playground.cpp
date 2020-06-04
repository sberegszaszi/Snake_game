#include "Playground.hpp"

bool Playground::IsInside(const Position & nextHeadPos) const
{
    return (nextHeadPos.GetX() >= 0 && nextHeadPos.GetX() < width &&
            nextHeadPos.GetY() >= 0 && nextHeadPos.GetY() < height);
}

void Playground::SendPositionDataTo(GUInterface & output) const
{
    for(auto i = 0; i < height; ++i){
        for(auto j = 0; j < width; ++j){
            Position emptyPos{i,j};
            output.UpdatePixel(emptyPos, PixelType::empty);
        }
    }
    return;
}