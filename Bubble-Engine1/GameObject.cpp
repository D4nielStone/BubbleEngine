#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"

GameObject::GameObject(ObjectFile object_file) {
	loadModel(object_file);
}

void GameObject::loadModel(ObjectFile object_file) {
	Components.push_back(std::make_unique<Transform>());
	int m = 0;
	for (auto vertex : object_file.vertices) {
		Components.push_back(std::make_unique<Renderer>(vertex, object_file.materials[m]));
		m++;
	}
}