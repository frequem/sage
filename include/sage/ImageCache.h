#ifndef _SAGE_IMAGECACHE_H
#define _SAGE_IMAGECACHE_H

#include <sage/FileCache.h>

#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "ogl.h"
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

namespace sage{
	class ImageCache{
		
		struct Texture{
			Texture(){}
			GLuint tid;
			glm::vec2 size;
			int index;
		};
		
	public:
		ImageCache(FileCache*);
		void load(const std::string&);
		void createTexture(const std::string&);
		
		GLuint getTexture(const std::string&);
		glm::vec2 getSize(const std::string&);
		float getWidth(const std::string&);
		float getHeight(const std::string&);
		~ImageCache();
	private:
		void waitfor(const std::string&);
		void load_func(const std::string&);
		
		FileCache* fileCache;
		std::map<const std::string, std::thread> threads;
		
		std::mutex imageMutex;
		std::vector<unsigned char*> image_data;
		
		std::mutex textureMutex;
		std::map<const std::string, Texture> textures;
	};
}

#endif // _SAGE_IMAGECACHE_H
