#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector>
#include "GUInterface.hpp"
#include "Position.hpp"

enum class Direction { up, right, down, left };

class Snake
{
private:
	std::vector<Position> body;
	Direction currentDirection;
	Position nextHeadPosition;
	void UpdateNextHeadPosition();
public:
	Snake() = delete;
	explicit Snake(Position head): body{head, Position{head.GetX()-1, head.GetY()}, Position{head.GetX()-2, head.GetY()}}, currentDirection{Direction::right}, nextHeadPosition{head + Position{1,0}} {}
	Snake(const Snake &) = delete;
	Snake & operator=(const Snake &) = delete;
	Snake(Snake &&) = delete;
	Snake & operator=(Snake &&) = delete;
	~Snake() = default;

	void Move();
	void IncreaseLength();
	bool NotInBody(const Position & nextHeadPos) const;
	Position GetCurrentHeadPosition() const { return body.at(0); }
	int GetLength() const { return static_cast<int>(body.size()); }
	Position GetNextHeadPosition() const { return nextHeadPosition; }
	void UpdateDirection(const Input & direction);
	void SendPositionDataTo(GUInterface & output) const;
};

#endif // SNAKE_HPP