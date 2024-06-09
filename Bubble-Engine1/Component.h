#pragma once
class Component {
public:
	const char* Name = "NoName_Component";
	Component(){};
	virtual void load() = 0; virtual void update() = 0;
};