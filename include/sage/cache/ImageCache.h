#ifndef _SAGE_IMAGECACHE_H
#define _SAGE_IMAGECACHE_H

#include <sage/util/ThreadManager.h>
#include <sage/cache/FileCache.h>

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <utility>
#include <variant>

#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

namespace sage{
	class ImageCache{
	public:
		ImageCache(FileCache*, ThreadManager*);
		void load(const std::string&);
		void unload(const std::string&);
		
		GLuint getTexture(const std::string&);
		glm::vec2 getSize(const std::string&);
		float getWidth(const std::string&);
		float getHeight(const std::string&);
		
		~ImageCache();
	private:
		void load_func(const std::string&);
		
		ThreadManager* threadManager;
		FileCache* fileCache;
				
		std::mutex mtx;
		std::condition_variable cv;
		std::map<const std::string, std::optional<std::pair<glm::vec2, std::variant<GLuint, unsigned char*>>>> textures;
	};
}

#endif // _SAGE_IMAGECACHE_H
