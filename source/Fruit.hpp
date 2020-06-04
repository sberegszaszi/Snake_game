#ifndef FRUIT_HPP
#define FRUIT_HPP

#include <chrono>
#include <random>
#include "GUInterface.hpp"
#include "Position.hpp"

class Fruit
{
private:
    Position fruitPos{0,0};
    std::random_device rand{};
    std::default_random_engine generator{rand()};
    std::uniform_int_distribution<int> x;
    std::uniform_int_distribution<int> y;
public:
    Fruit() = delete;
    explicit Fruit(Position max);
    Fruit(const Fruit &) = default;
    Fruit & operator=(const Fruit &)= default;
    Fruit(Fruit &&)= default;
    Fruit & operator=(Fruit &&)= default;
    ~Fruit() = default;

    void Regenerate();
    Position GetPosition() const { return fruitPos; }
    void SendPositionDataTo(GUInterface & output) const;
};

#endif // FRUIT_HPP