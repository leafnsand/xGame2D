#include "render/OpenGL.h"
#include "textures/GLTexture.h"
#include "util/Console.h"
#include "util/PVRData.h"

namespace xGame2D
{
	GLTexture::GLTexture()
		: format(TextureFormatRGBA)
		, smoothing(TextureSmoothingNone)
		, name(0)
		, width(0.0f)
		, height(0.0f)
		, scale(1.0f)
		, repeat(true)
		, premultipliedAlpha(false)
		, mipmaps(false)
	{
	}

	GLTexture::~GLTexture()
	{
		// destroyFramebufferForTexture
		glDeleteTextures(1, &name);
	}

	bool GLTexture::init()
	{
		return init(0, TextureFormatRGBA, 64, 64, false, 1.0f, false);
	}

	bool GLTexture::init(uint32_t name, TextureFormat format, float width, float height, bool mipmaps, float scale, bool pma)
	{
		if (width <= 0.0f)  Console::Error("invalid width");
		if (height <= 0.0f) Console::Error("invalid height");
		if (scale <= 0.0f)  Console::Error("invalid scale");
		this->name = name;
		this->width = width;
		this->height = height;
		this->mipmaps = mipmaps;
		this->scale = scale;
		this->premultipliedAlpha = pma;
		setRepeat(false);
		setSmoothing(TextureSmoothingBilinear);
		return true;
	}

	bool GLTexture::init(const void *imageData, TextureProperties properties)
	{
		GLenum glTexType = GL_UNSIGNED_BYTE;
		GLenum glTexFormat;
		GLuint glTexName;
		int32_t bitsPerPixel;
		auto compressed = false;
		switch (properties.format)
		{
			default:
			case TextureFormatRGBA:
				bitsPerPixel = 32;
				glTexFormat = GL_RGBA;
				break;
			case TextureFormatAlpha:
				bitsPerPixel = 8;
				glTexFormat = GL_ALPHA;
				break;
#if COMPRESSED_TEXTURE
			case TextureFormatPvrtcRGBA2:
				compressed = true;
				bitsPerPixel = 2;
				glTexFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
				break;
			case TextureFormatPvrtcRGB2:
				compressed = true;
				bitsPerPixel = 2;
				glTexFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
				break;
			case TextureFormatPvrtcRGBA4:
				compressed = true;
				bitsPerPixel = 4;
				glTexFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
				break;
			case TextureFormatPvrtcRGB4:
				compressed = true;
				bitsPerPixel = 4;
				glTexFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				break;
#endif
			case TextureFormat565:
				bitsPerPixel = 16;
				glTexFormat = GL_RGB;
				glTexType = GL_UNSIGNED_SHORT_5_6_5;
				break;
			case TextureFormat888:
				bitsPerPixel = 24;
				glTexFormat = GL_RGB;
				break;
			case TextureFormat5551:
				bitsPerPixel = 16;
				glTexFormat = GL_RGBA;
				glTexType = GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			case TextureFormat4444:
				bitsPerPixel = 16;
				glTexFormat = GL_RGBA;
				glTexType = GL_UNSIGNED_SHORT_4_4_4_4;
				break;
			case TextureFormatAI88:
				bitsPerPixel = 16;
				glTexFormat = GL_LUMINANCE_ALPHA;
				break;
			case TextureFormatI8:
				bitsPerPixel = 8;
				glTexFormat = GL_LUMINANCE;
				break;
		}
		glGenTextures(1, &glTexName);
		glBindTexture(GL_TEXTURE_2D, glTexName);
		if (!compressed)
		{
			auto levelWidth = properties.width;
			auto levelHeight = properties.height;
			auto levelData = static_cast<const uint8_t *>(imageData);
			for (auto level = 0; level <= properties.numMipmaps; level++)
			{
				auto size = levelWidth * levelHeight * bitsPerPixel / 8;
				glTexImage2D(GL_TEXTURE_2D, level, glTexFormat, levelWidth, levelHeight, 0, glTexFormat, glTexType, levelData);
				levelData += size;
				levelWidth /= 2;
				levelHeight /= 2;
			}
			if (properties.numMipmaps == 0 && properties.generateMipmaps)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else
		{
			auto levelWidth = properties.width;
			auto levelHeight = properties.height;
			auto levelData = static_cast<const unsigned char *>(imageData);
			for (auto level = 0; level <= properties.numMipmaps; level++)
			{
				auto size = levelWidth * levelHeight * bitsPerPixel / 8;
				if (size < 32)
				{
					size = 32;
				}
				glTexImage2D(GL_TEXTURE_2D, level, glTexFormat, levelWidth, levelHeight, 0, glTexFormat, glTexType, levelData);
				levelData += size;
				levelWidth /= 2;
				levelHeight /= 2;
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		auto containsMipmaps = properties.numMipmaps > 0 || (properties.generateMipmaps && !compressed);
		return init(glTexName, properties.format, static_cast<float>(properties.width), static_cast<float>(properties.height)
			, containsMipmaps, properties.scale, properties.premultipliedAlpha);
	}

	bool GLTexture::init(PVRData *pvrData, float scale)
	{
		TextureProperties properties = {
			pvrData->format,
			scale,
			pvrData->width,
			pvrData->height,
			pvrData->numMipmaps,
			false,
			false
		};
		return init(pvrData->getImageData(), properties);
	}

	float GLTexture::getWidth()
	{
		return width / scale;
	}

	float GLTexture::getHeight()
	{
		return height / scale;
	}

	float GLTexture::getNativeWidth()
	{
		return width;
	}

	float GLTexture::getNativeHeight()
	{
		return height;
	}

	GLTexture *GLTexture::getRoot()
	{
		return this;
	}

	uint32_t GLTexture::getName()
	{
		return name;
	}

	bool GLTexture::getPremultipliedAlpha()
	{
		return premultipliedAlpha;
	}

	float GLTexture::getScale()
	{
		return scale;
	}

	TextureFormat GLTexture::getFormat()
	{
		return format;
	}

	bool GLTexture::getMipmaps()
	{
		return mipmaps;
	}

	bool GLTexture::getRepeat()
	{
		return repeat;
	}

	void GLTexture::setRepeat(bool value)
	{
		if (value != repeat)
		{
			repeat = value;
			glBindTexture(GL_TEXTURE_2D, name);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		}
	}

	TextureSmoothing GLTexture::getSmoothing()
	{
		return smoothing;
	}

	void GLTexture::setSmoothing(TextureSmoothing value)
	{
		if (value != smoothing)
		{
			smoothing = value;
			glBindTexture(GL_TEXTURE_2D, name);
			GLint magFilter, minFilter;
			if (value == TextureSmoothingNone)
			{
				magFilter = GL_NEAREST;
				minFilter = mipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			}
			else if (value == TextureSmoothingBilinear)
			{
				magFilter = GL_LINEAR;
				minFilter = mipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
			}
			else
			{
				magFilter = GL_LINEAR;
				minFilter = mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		}
	}
}