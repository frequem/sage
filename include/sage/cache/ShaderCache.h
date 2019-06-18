#ifndef _SAGE_SHADERCACHE_H
#define _SAGE_SHADERCACHE_H

#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <glm/glm.hpp>

namespace sage{
	class Application;
	class ShaderCache{
	
	public:
		ShaderCache(Application&);
		void load(const std::string&);
		void unload(const std::string&);
		GLuint get(const std::string&);
		~ShaderCache();
	private:
		Application* application;
		std::map<const std::string, GLuint> shaders;
		
		GLuint createShader(const std::string&, GLenum shaderType);
	};
}

#endif // _SAGE_SHADERCACHE_H
