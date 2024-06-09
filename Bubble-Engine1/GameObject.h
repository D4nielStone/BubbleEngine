#pragma once
#include "Component.h"
#include "ObjectFile.h"
#include "Transform.h"
#include "Renderer.h"
#include <memory>

class GameObject
{
public:
	GameObject(ObjectFile object_file);
	void loadModel(ObjectFile object_file);
	Component& getComponent(const char* name) {
		for (auto& c : Components) {
			if (c->Name == name) {
				return *c;
			}
		}
	};
	std::vector<Component*> getComponents(const char* name) {
		std::vector<Component*> comps;
		for (auto& c : Components) {
			if (c->Name == name) {
				comps.push_back(c.get());
			}
		}
		return comps;
	};
	Transform* getTransform() {
		for (auto& c : Components) {
			if (c->Name == "Transform") {
				return &dynamic_cast<Transform&>(*c);
			}
		}
		return nullptr;
	}
	Renderer* getRenderer() {
		for (auto& c : Components) {
			if (c->Name == "Renderer") {
				return &dynamic_cast<Renderer&>(*c);
			}
		}
		return nullptr;
	}
	std::vector<std::unique_ptr<Component>> Components;
private:
};

