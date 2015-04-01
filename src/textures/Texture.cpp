#include "base/Data.h"
#include "textures/Texture.h"
#include "textures/SubTexture.h"
#include "textures/GLTexture.h"
#include "geom/Rectangle.h"
#include "util/Utils.h"
#include "util/Console.h"
#include <typeinfo>
#include <png.h>

namespace xGame2D
{
	typedef struct
	{
		uint8_t *data;
		size_t size;
		size_t offset;
	} internal_image_source;

	static void image_png_read_callback(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		internal_image_source *isource = (internal_image_source *)png_get_io_ptr(png_ptr);
		if ((size_t)(isource->offset + length) <= isource->size)
		{
			memcpy(data, isource->data + isource->offset, length);
			isource->offset += length;
		}
		else
		{
			png_error(png_ptr, "pngReaderCallback failed");
		}
	}
	
	Texture *Texture::create(float width, float height)
	{
		size_t size = (int32_t)width * (int32_t)height * 4;
		void *data = malloc(size);
		memset(data, 0, size);
		return create(width, height, Object::create<Data>(data, size), TextureFormatRGBA);
	}

	Texture *Texture::create(float width, float height, Data *data, TextureFormat format)
	{
		return create(width, height, data, false, format);
	}

	Texture *Texture::create(float width, float height, Data *data, bool mipmaps, TextureFormat format)
	{
		// TODO Sparrow.contentScaleFactor
		return create(width, height, data, mipmaps, 1.0f, format);
	}

	Texture *Texture::create(float width, float height, Data *data, bool mipmaps, float scale, TextureFormat format)
	{
		/*release();*/
		auto legalWidth = (int32_t)width;// Utils::nextPowerOfTwo(static_cast<int32_t>(width * scale));
		auto legalHeight = (int32_t)height;// Utils::nextPowerOfTwo(static_cast<int32_t>(height * scale));
		TextureProperties properties = {
			format,
			scale,
			legalWidth,
			legalHeight,
			0,
			mipmaps,
			false
		};
		GLTexture *glTexture = Object::create<GLTexture>(data->getBuffer(), properties);
		Rectangle *region = Object::create<Rectangle>(0.0f, 0.0f, width, height);
		return create(region, glTexture);
	}

	Texture *Texture::create(std::string &path)
	{
		return create(path, false);
	}

	Texture *Texture::create(std::string &path, bool mipmaps)
	{
		float width = 0.0f, height = 0.0f;
		TextureFormat format = TextureFormatRGBA;
		size_t size = 0;
		void *buffer = nullptr;

		Data *data = Utils::readFile(path);
		if (data == nullptr)
		{
			Console::Error("file not found.\n", path.c_str());
		}
		internal_image_source source;
		source.data = (uint8_t *)data->getBuffer();
		source.size = data->getSize();
		source.offset = 0;
		if (png_sig_cmp(source.data, 0, 8))
		{
			Console::Log("file(%s) is not a png image.\n", path.c_str());
			return nullptr;
		}
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			Console::Log("create png struct error.\n");
			return nullptr;
		}
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			Console::Log("create png info error.\n");
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return nullptr;
		}
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			Console::Log("set png jump buffer error.\n");
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return nullptr;
		}
		png_set_read_fn(png_ptr, &source, image_png_read_callback);
		png_read_info(png_ptr, info_ptr);
		width = (float)png_get_image_width(png_ptr, info_ptr);
		height = (float)png_get_image_height(png_ptr, info_ptr);
		png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		{
			bit_depth = 8;
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
		}
		if (bit_depth < 8)
		{
			png_set_packing(png_ptr);
		}
		png_read_update_info(png_ptr, info_ptr);
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
		switch (color_type)
		{
			case PNG_COLOR_TYPE_GRAY:
				format = TextureFormatI8;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				format = TextureFormatAI88;
				break;
			case PNG_COLOR_TYPE_RGB:
				format = TextureFormat888;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				format = TextureFormatRGBA;
				break;
			default:
				Console::Log("invalid png color type.\n");
				break;
		}
		png_size_t rowbytes;
		png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (int32_t)height);
		rowbytes = png_get_rowbytes(png_ptr, info_ptr);
		size = rowbytes * (int32_t)height;
		buffer = malloc(size);
		uint8_t *image_data = (uint8_t *)buffer;
		for (uint32_t i = 0; i < height; i++)
		{
			row_pointers[i] = image_data + i * rowbytes;
		}
		png_read_image(png_ptr, row_pointers);
		png_read_end(png_ptr, NULL);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(row_pointers);
		return create(width, height, Object::create<Data>(buffer, size), format);
	}

	Texture *Texture::create(Rectangle *region, Texture *texture)
	{
		return create(region, nullptr, texture);
	}

	Texture *Texture::create(Rectangle *region, Rectangle *frame, Texture *texture)
	{
		if (frame || region->x != 0.0f || region->width != texture->getWidth()
			|| region->y != 0.0f || region->height != texture->getHeight())
		{
			return Object::create<SubTexture>(region, frame, texture);
		}
		else
		{
			texture->retain();
			return texture;
		}
	}
}