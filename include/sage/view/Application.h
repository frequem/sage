#ifndef _SAGE_APPLICATION_H
#define _SAGE_APPLICATION_H

#include <sage/util/ThreadManager.h>
#include <sage/cache/FileCache.h>
#include <sage/cache/ImageCache.h>
#include <sage/cache/ShaderCache.h>
#include <sage/cache/FontCache.h>
#include <sage/cache/AudioCache.h>
#include <sage/util/AudioManager.h>
#include <sage/util/EventDispatcher.h>
#include <sage/renderer/Renderer.h>

#include <stack>
#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <SDL2/SDL_mixer.h> 
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace sage{
	class Scene;
	
	/**
	 * @brief Class that handles the window.
	 * Also manages Scenes, Caches and runs the main game loop.
	 */
	class Application{
	public:
		/**
		 * @brief Application constructor.
		 * Initializes the Application with title "sage Application".
		 * @see Application(const std::string& title)
		 */
		Application();
		
		/**
		 * @brief Application constructor.
		 * Initializes the Application with size 960x540.
		 * @param title the window title
		 * @see Application(const std::string& title, int with, int height)
		 */
		Application(const std::string& title);
		
		/**
		 * \brief Application constructor.
		 * Initializes the Application.
		 * @param title the window title
		 * @param width the window width
		 * @param height the window height
		 */
		Application(const std::string& title, int width, int height);
		
		/**
		 * @brief Pushes a Scene onto the stack.
		 * It will be displayed in the next rendering call and be active until it is popped.
		 * Calls the init() function of the Scene.
		 * @param scene the Scene
		 * @see replaceScene()
		 * @see popScene()
		 * @see getScene()
		 */
		void pushScene(std::shared_ptr<Scene> scene);
		
		/**
		 * @brief Replaces the current Scene on the stack.
		 * @param scene the new Scene
		 * @see pushScene
		 * @see popScene
		 * @see getScene
		 */
		void replaceScene(std::shared_ptr<Scene> scene);
		
		/**
		 * @brief Fetches the current Scene from the stack.
		 * @return the current Scene
		 * @see pushScene
		 * @see replaceScene
		 * @see popScene
		 */
		std::shared_ptr<Scene> getScene();
		
		/**
		 * @brief Pops the current Scene from the stack.
		 * Also deletes the Scene, do not delete manually!
		 * @see pushScene
		 * @see replaceScene
		 * @see getScene
		 */
		void popScene();
		
		/**
		 * @brief Sets the size of the window.
		 * @param size the new window size
		 * @see getWindowSize()
		 */
		void setWindowSize(glm::vec2 size);
		
		/**
		 * @brief Fetches the current size of the window.
		 * @return the window size
		 * @see setWindowSize()
		 * @see getWindowWidth()
		 * @see getWindowHeight()
		 */
		glm::vec2 getWindowSize();
		
		/**
		 * @brief Fetches the current width of the window.
		 * @return the window width
		 * @see getWindowSize()
		 * @see getWindowHeight()
		 */
		float getWindowWidth();
		
		/**
		 * @brief Fetches the current height of the window.
		 * @return the window height
		 * @see getWindowSize()
		 * @see getWindowWidth()
		 */
		float getWindowHeight();
		
		/**
		 * @brief Fetches the DPI of the display, the window is currently on.
		 * x contains the horizontal, y the vertical DPI value.
		 * Currently always returns 400 on Android (SDL_GetDisplayDPI does not work on Android)
		 * @return the displays horizontal and vertical DPI
		 */
		glm::vec2 getDPI();
		
		/**
		 * @brief Fetches the FileCache.
		 * @return the FileCache
		 * @see fileCache
		 * @see getImageCache()
		 * @see getFontCache()
		 * @see getShaderCache()
		 * @see getAudioCache()
		 */
		FileCache& getFileCache();
		
		/**
		 * @brief Fetches the ImageCache.
		 * @return the ImageCache
		 * @see imageCache
		 * @see getFileCache()
		 */
		ImageCache& getImageCache();
		
		/**
		 * @brief Fetches the FontCache.
		 * @return the FontCache
		 * @see fontCache
		 * @see getFileCache()
		 */
		FontCache& getFontCache();
		
		/**
		 * @brief Fetches the ShaderCache.
		 * @return the ShaderCache
		 * @see shaderCache
		 * @see getFileCache()
		 */
		ShaderCache& getShaderCache();
		
		/**
		 * @brief Fetches the AudioCache.
		 * @return the AudioCache
		 * @see audioCache
		 * @see getAudioManager()
		 * @see getFileCache()
		 */
		AudioCache& getAudioCache();
		
		/**
		 * @brief Fetches the EventDispatcher.
		 * @return the EventDispatcher
		 * @see eventDispatcher
		 */
		EventDispatcher& getEventDispatcher();
		
		/**
		 * @brief Fetches the AudioManager.
		 * @return the AudioManager
		 * @see audioManager
		 * @see getAudioCache()
		 */
		AudioManager& getAudioManager();
		
		/**
		 * @brief Fetches the ThreadManager.
		 * @return the ThreadManager
		 * @see threadManager
		 */
		ThreadManager& getThreadManager();
		
		/**
		 * @brief Fetches the Renderer.
		 * @return the Renderer
		 * @see setRenderer(std::unique_ptr<Renderer> renderer)
		 * @see renderer
		 */
		Renderer& getRenderer();
		
		/**
		 * @brief Sets the Renderer
		 * @param renderer the Renderer
		 * @see getRenderer()
		 * @see renderer
		 */
		 void setRenderer(std::unique_ptr<Renderer> renderer);
		 
		/**
		 * @brief Starts the main game loop.
		 * Manages rendering and timing, starts the EventDispatcher.
		 */
		void run();
		
		/**
		 * @brief Application Destructor.
		 * Deletes all Scenes, Caches, etc., destroys the window.
		 */
		~Application();
	private:
		std::stack<std::shared_ptr<Scene>> scenes; /**< The Scene stack */
		uint32_t lastUpdate = 0; /**< The time, that the most recent update call has been made to a Scene */
		
		std::unique_ptr<ThreadManager> threadManager; /**< Pointer to the ThreadManager */
		std::unique_ptr<FileCache> fileCache; /**< Pointer to the FileCache */
		std::unique_ptr<ImageCache> imageCache; /**< Pointer to the ImageCache */
		std::unique_ptr<ShaderCache> shaderCache; /**< Pointer to the ShaderCache */
		std::unique_ptr<FontCache> fontCache; /**< Pointer to the FontCache */
		std::unique_ptr<AudioCache> audioCache; /**< Pointer to the AudioCache */
		std::unique_ptr<EventDispatcher> eventDispatcher; /**< Pointer to the EventDispatcher */
		std::unique_ptr<AudioManager> audioManager; /**< Pointer to the AudioManager */
		std::unique_ptr<Renderer> renderer; /**< Pointer to the Renderer which decides how to render the Nodes */
		
		bool isRunning = false; /**< Whether the run function has already been called or not*/
		bool isPaused = false; /**< Whether the Application is currently paused or not*/
		
		SDL_Window* sdlWindow; /**< The SDL window that the Application manages */
		SDL_GLContext glContext; /**< The OpenGL context for the window */
		
	};
}
#endif // _SAGE_APPLICATION_H
