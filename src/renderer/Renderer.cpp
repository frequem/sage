#include <sage/renderer/Renderer.h>
#include <sage/view/Application.h>
#include <sage/view/Node.h>
#include <sage/view/Scene.h>
#include <sage/view/TexturedNode.h>
#include <algorithm>

using namespace sage;

Renderer::Renderer(Application& application) : application(&application){}

void Renderer::render(){ this->render(*(this->application->getScene())); }

void Renderer::render(std::vector<std::shared_ptr<Node>> ns){
	for(auto n : ns){
		this->render(*n);
	}
}

void Renderer::render(Node& n){
	this->tryRenderSingles(n);
	this->render(n.getChildren());
}

void Renderer::tryRenderSingles(Node& n){
	if(tryRenderSingle<TexturedNode>(n)) return;
}

Renderer::~Renderer(){}
