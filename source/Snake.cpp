#include <iostream>
#include "Snake.hpp"

void Snake::UpdateNextHeadPosition(){
    Position delta;
    switch(currentDirection){
        case Direction::up:
			delta = Position{0,-1};
			break;
        case Direction::right:
			delta = Position{1,0};
			break;
        case Direction::down:
			delta = Position{0,1};
			break;
        case Direction::left:
			delta = Position{-1,0};
			break;
    }
	nextHeadPosition = body.at(0) + delta;
	return;
}

void Snake::Move()
{
	for(auto ri = body.rbegin(); ri != body.rend()-1; ++ri){
		*ri = *(ri+1);
	}
	body.at(0) = nextHeadPosition;
	return;
}

void Snake::IncreaseLength()
{
	body.emplace_back(body.back());
}

bool Snake::NotInBody(const Position & nextHeadPos) const
{
	for(auto i = body.begin()+1; i != body.end(); ++i){
		if(*i == nextHeadPos){
			return false;
		}
	}
	return true;
}

void Snake::UpdateDirection(const Input & direction)
{
	switch(direction){
		case Input::none:
		case Input::quit:
		case Input::enter:
		case Input::pause:
			break;
		case Input::up:
			if(currentDirection != Direction::down) currentDirection = Direction::up;
			break;
		case Input::right:
			if(currentDirection != Direction::left) currentDirection = Direction::right;
			break;
		case Input::down:
			if(currentDirection != Direction::up) currentDirection = Direction::down;
			break;
		case Input::left:
			if(currentDirection != Direction::right) currentDirection = Direction::left;
			break;
	}
	UpdateNextHeadPosition();
	return;
}

void Snake::SendPositionDataTo(GUInterface & output) const
{
	output.UpdatePixel(body.at(0), PixelType::snake_head);
	for(auto i = body.begin()+1; i != body.end(); ++i){
		output.UpdatePixel(*i, PixelType::snake_body);
	}
	return;
}