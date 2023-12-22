#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct ShaderUtils {
	unsigned int (*compileFromChars)(const char* vertShaderSource,
			const char* fragShaderSource);
};

extern const struct ShaderUtils ShaderUtils;