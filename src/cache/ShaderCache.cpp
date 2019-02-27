#include <sage/cache/ShaderCache.h>
#include <sage/util/config.h>
#include <sage/util/macros.h>

using namespace sage;

ShaderCache::ShaderCache(FileCache* fileCache){
	this->fileCache = fileCache;
}

void ShaderCache::load(const std::string& shaderName){
	this->fileCache->load(shaderName + SHADER_VERTEX_POSTFIX);
	this->fileCache->load(shaderName + SHADER_FRAGMENT_POSTFIX);
}

void ShaderCache::unload(const std::string& shaderName){
	if(this->shaders.find(shaderName) != this->shaders.end()){
		std::map<const std::string, GLuint>::iterator i = this->shaders.find(shaderName);
		GLuint pid = i->second;
		glDeleteProgram(pid);
		this->shaders.erase(i);
	}
	this->fileCache->unload(shaderName + SHADER_VERTEX_POSTFIX);
	this->fileCache->unload(shaderName + SHADER_FRAGMENT_POSTFIX);
}

GLuint ShaderCache::createShader(const std::string& fn, GLenum shaderType){
	GLuint sid = glCreateShader(shaderType);
	
	const char* data = this->fileCache->get(fn);
	int size = this->fileCache->size(fn);
	
	LOG("sage::ShaderCache: Compiling '%s'", fn.c_str());
	glShaderSource(sid, 1, &data, &size);
	glCompileShader(sid);
	
	int infoLogLength;
	glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0){
		char log[infoLogLength+1];
		glGetShaderInfoLog(sid, infoLogLength, NULL, &log[0]);
		LOG("Shader '%s' Info Log: %s", fn.c_str(), log);
	}
	
	GLint res;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &res);
	ASSERT(res == GL_TRUE, "Could not compile shader '%s'", fn.c_str());
	
	return sid;
}

GLuint ShaderCache::get(const std::string& shaderName){
	if(this->shaders.find(shaderName) != this->shaders.end()){
		return this->shaders[shaderName];
	}
	this->load(shaderName);
	
	GLuint vert = createShader(shaderName + SHADER_VERTEX_POSTFIX, GL_VERTEX_SHADER);
	GLuint frag = createShader(shaderName + SHADER_FRAGMENT_POSTFIX, GL_FRAGMENT_SHADER);
	
	GLuint program = glCreateProgram();
	this->shaders[shaderName] = program;
	
	
	LOG("sage::ShaderCache: Linking '%s'", shaderName.c_str());
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	
	int infoLogLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength > 0){
		char log[infoLogLength+1];
		glGetProgramInfoLog(program, infoLogLength, NULL, &log[0]);
		LOG("Shader Program '%s' Info Log: %s", shaderName.c_str(), log);
	}
	
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	ASSERT(res == GL_TRUE, "Could not link shader program '%s'", shaderName.c_str());
	
	glDetachShader(program, vert);
	glDetachShader(program, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);
	
	return program;
}

ShaderCache::~ShaderCache(){
	for(std::map<const std::string, GLuint>::iterator i=this->shaders.begin(); i!=this->shaders.end(); i++){
		GLuint pid = i->second;//std::get<GLuint>(*i);
		glDeleteProgram(pid);
	}
}
