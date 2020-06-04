#ifndef POSITION_HPP
#define POSITION_HPP

class Position
{
private:
	int x{0};
	int y{0};
public:
	explicit Position() = default;
	explicit Position(int posX, int posY): x{posX}, y{posY} {}
	Position(const Position & other) = default;
	Position & operator=(const Position &) = default;
	Position(Position &&) = default;
	Position & operator=(Position &&) = default;
	~Position() = default;

	int GetX() const { return x; }
	int GetY() const { return y; }
	void SetX(int value){ x = value; }
	void SetY(int value){ y = value; }

	bool operator==(const Position & lhs) const
	{
		return (x == lhs.x && y == lhs.y);
	}

	bool operator!=(const Position & lhs) const
	{
		return (x != lhs.x || y != lhs.y);
	}

	Position operator+(const Position & other) const
	{
		Position rhs(0,0);
		rhs.x = this->x + other.x;
		rhs.y = this->y + other.y;
		return rhs;
	}

	Position& operator+=(const Position & lhs)
	{
		x += lhs.x;
		y += lhs.y;
		return *this;
	}
};

#endif // POSITION_HPP