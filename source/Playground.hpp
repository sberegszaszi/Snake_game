#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

#include "GUInterface.hpp"
#include "Position.hpp"

class Playground{
private:
    const int width;
    const int height;
public:
    Playground() = delete;
	explicit Playground(int w, int h): width{w}, height{h} {}
	Playground(const Playground &) = delete;
	Playground & operator=(const Playground &) = delete;
	Playground(Playground &&) = delete;
	Playground & operator=(Playground &&) = delete;
	~Playground() = default;

	auto GetWidth() const { return width; }
	auto GetHeight() const { return height; }
    bool IsInside(const Position & nextHeadPos) const;
	void SendPositionDataTo(GUInterface & output) const;
};

#endif // PLAYGROUND_HPP