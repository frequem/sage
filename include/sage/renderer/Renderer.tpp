#ifndef _SAGE_RENDERER_TPP
#define _SAGE_RENDERER_TPP


template <typename T>
bool Renderer::tryRenderSingle(Node& n){
	T* o = dynamic_cast<T*>(&n);
	if(o != nullptr){
		this->renderSingle(*o);
		return true;
	}
	return false;
}

#endif // _SAGE_RENDERER_TPP
