#include"../include/Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;
	unit = slot;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true); // flip vertically
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	if (!bytes) {
		std::cerr << "Failed to load texture: " << image << std::endl;
		return;
	}

	GLenum internalFormat, dataFormat;
	if (numColCh == 1) { internalFormat = dataFormat = GL_RED; }
	else if (numColCh == 3) { internalFormat = dataFormat = GL_RGB; }
	else if (numColCh == 4) { internalFormat = dataFormat = GL_RGBA; }
	else {
		std::cerr << "Unsupported texture format: " << numColCh << " channels\n";
		stbi_image_free(bytes);
		return;
	}

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);

	// Stretch textures across the surface
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // minification with mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload the texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, widthImg, heightImg, 0, dataFormat, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}