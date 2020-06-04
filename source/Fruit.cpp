#include "Fruit.hpp"

Fruit::Fruit(Position max):
    x{0, max.GetX()},
    y{0, max.GetY()}
{
    while( (fruitPos.GetX() == max.GetX()/2 && fruitPos.GetY() == max.GetY()/2)
            || (fruitPos.GetX() == (max.GetX()/2 - 1) && fruitPos.GetY() == max.GetY()/2)
            || (fruitPos.GetX() == (max.GetX()/2 - 2) && fruitPos.GetY() == max.GetY()/2) ){

        fruitPos = Position{x(generator), y(generator)};
    }
}

void Fruit::Regenerate()
{
    fruitPos = Position{x(generator), y(generator)};
    return;
}

void Fruit::SendPositionDataTo(GUInterface & output) const
{
    output.UpdatePixel(fruitPos, PixelType::fruit);
    return;
}