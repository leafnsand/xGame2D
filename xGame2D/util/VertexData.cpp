#include "util/VertexData.h"
#include "util/Console.h"
#include <string.h>

#define MIN_ALPHA (5.0f / 255.0f)

namespace xGame2D
{
	static Position VertexPointMake(float x, float y)
	{
		Position point = {x, y};
		return point;
	}

	static Color VertexColorMake(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		Color vertexColor = {r, g, b, a};
		return vertexColor;
	}

	static Color VertexColorMakeWithColorAndAlpha(int32_t rgb, float alpha)
	{
		Color vertexColor = {
			ColorGetRed(rgb),
			ColorGetGreen(rgb),
			ColorGetBlue(rgb),
			static_cast<uint8_t>(alpha * 255.0f)
		};
		return vertexColor;
	}

	static Color premultiplyAlpha(Color color)
	{
		auto alpha = color.a / 255.0f;
		if (alpha == 1.0f) return color;
		else
			return VertexColorMake(
				static_cast<uint8_t>(color.r * alpha),
				static_cast<uint8_t>(color.g * alpha),
				static_cast<uint8_t>(color.b * alpha),
				color.a);
	}

	static Color unmultiplyAlpha(Color color)
	{
		auto alpha = color.a / 255.0f;
		if (alpha == 0.0f || alpha == 1.0f) return color;
		else
			return VertexColorMake(
				static_cast<uint8_t>(color.r / alpha),
				static_cast<uint8_t>(color.g / alpha),
				static_cast<uint8_t>(color.b / alpha),
				color.a);
	}

	static bool isOpaqueWhite(Color color)
	{
		return color.a == 255 && color.r == 255 && color.g == 255 && color.b == 255;
	}

	VertexData::VertexData():
		vertices(nullptr),
		numVertices(0),
		premultipliedAlpha(false)
	{
	}

	VertexData::~VertexData()
	{
		if (vertices) free(vertices);
	}

	bool VertexData::init(int32_t numVertices)
	{
		return init(numVertices, false);
	}

	bool VertexData::init(int32_t numVertices, bool pma)
	{
		this->premultipliedAlpha = pma;
		setNumVertices(numVertices);
		return true;
	}

	void VertexData::copyTo(VertexData *target)
	{
		copyTo(target, 0, numVertices);
	}

	void VertexData::copyTo(VertexData *target, int32_t targetIndex)
	{
		copyTo(target, targetIndex, numVertices);
	}

	void VertexData::copyTo(VertexData *target, int32_t targetIndex, int32_t count)
	{
		X_ASSERT(count >= 0 && count <= numVertices, "Invalid vertex count");
		X_ASSERT(targetIndex + count <= target->numVertices, "Target too small");
		memcpy(&target->vertices[targetIndex], vertices, sizeof(Vertex) * count);
	}

	Vertex VertexData::vertexAtIndex(int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		return vertices[index];
	}

	void VertexData::setVertexAtIndex(Vertex vertex, int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		vertices[index] = vertex;
		if (premultipliedAlpha)
			vertices[index].color = premultiplyAlpha(vertex.color);
	}

	void VertexData::appendVertex(Vertex vertex)
	{
		setNumVertices(numVertices + 1);
		if (vertices)
		{
			if (premultipliedAlpha)
				vertex.color = premultiplyAlpha(vertex.color);
			vertices[numVertices - 1] = vertex;
		}
	}

	Point *VertexData::positionAtIndex(int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		auto position = vertices[index].position;
		return Object::create<Point>(position.x, position.y);
	}

	void VertexData::setPositionAtIndex(Point *position, int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		vertices[index].position = VertexPointMake(position->x, position->y);
	}

	void VertexData::setPositionAtIndex(float x, float y, int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		vertices[index].position = VertexPointMake(x, y);
	}

	Point *VertexData::texcoordsAtIndex(int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		auto texcoords = vertices[index].texcoords;
		return Object::create<Point>(texcoords.x, texcoords.y);
	}

	void VertexData::setTexcoordsAtIndex(Point *texcoords, int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		vertices[index].texcoords = VertexPointMake(texcoords->x, texcoords->y);
	}

	void VertexData::setTexcoordsAtIndex(float x, float y, int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		vertices[index].texcoords = VertexPointMake(x, y);
	}

	void VertexData::setColorAtIndex(uint32_t color, float alpha, int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		alpha = CLAMP(alpha, premultipliedAlpha ? MIN_ALPHA : 0.0f, 1.0f);
		auto vertexColor = VertexColorMakeWithColorAndAlpha(color, alpha);
		vertices[index].color = premultipliedAlpha ? premultiplyAlpha(vertexColor) : vertexColor;
	}

	void VertexData::setColorAtIndex(uint32_t color, int32_t index)
	{
		auto alpha = alphaAtIndex(index);
		setColorAtIndex(color, alpha, index);
	}

	void VertexData::setColorOfAll(uint32_t color, float alpha)
	{
		for (auto i = 0; i < numVertices; ++i)
			setColorAtIndex(color, alpha, i);
	}

	void VertexData::setColorOfAll(uint32_t color)
	{
		for (auto i = 0; i < numVertices; ++i)
			setColorAtIndex(color, i);
	}

	uint32_t VertexData::colorAtIndex(int32_t index)
	{
		X_ASSERT(index > 0 && index < numVertices, "Invalid vertex index");
		auto vertexColor = vertices[index].color;
		if (premultipliedAlpha) vertexColor = unmultiplyAlpha(vertexColor);
		return ColorMake(vertexColor.r, vertexColor.g, vertexColor.b);
	}

	float VertexData::alphaAtIndex(int32_t index)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		return vertices[index].color.a / 255.0f;
	}

	void VertexData::setAlphaAtIndex(float alpha, int32_t index)
	{
		auto color = colorAtIndex(index);
		setColorAtIndex(color, alpha, index);
	}

	void VertexData::setAlphaOfAll(float alpha)
	{
		for (auto i = 0; i < numVertices; ++i)
			setAlphaAtIndex(alpha, i);
	}

	void VertexData::scaleAlphaOfAll(float scale)
	{
		scaleAlphaOfVertices(scale, 0, numVertices);
	}

	void VertexData::scaleAlphaOfVertices(float scale, int32_t index, int32_t count)
	{
		X_ASSERT(index >= 0 && index < numVertices, "Invalid vertex index");
		if (scale == 1.0f) return;
		uint8_t minAlpha = premultipliedAlpha ? static_cast<uint8_t>(MIN_ALPHA * 255.0f) : 0;
		for (auto i = index; i < index + count; ++i)
		{
			auto vertex = &vertices[i];
			auto vertexColor = vertex->color;
			auto newAlpha = CLAMP(static_cast<uint8_t>(vertexColor.a * scale), minAlpha, static_cast<uint8_t>(255));
			if (premultipliedAlpha)
			{
				vertexColor = unmultiplyAlpha(vertexColor);
				vertexColor.a = newAlpha;
				vertex->color = premultiplyAlpha(vertexColor);
			}
			else
			{
				vertex->color = VertexColorMake(vertexColor.r, vertexColor.g, vertexColor.b, newAlpha);
			}
		}
	}

	void VertexData::setPremultipliedAlpha(bool value, bool update)
	{
		if (value == premultipliedAlpha) return;
		if (update)
		{
			if (value)
			{
				for (auto i = 0; i < numVertices; ++i)
					vertices[i].color = premultiplyAlpha(vertices[i].color);
			}
			else
			{
				for (auto i = 0; i < numVertices; ++i)
					vertices[i].color = unmultiplyAlpha(vertices[i].color);
			}
		}
		premultipliedAlpha = value;
	}

	void VertexData::transformVerticesWithMatrix(Matrix *matrix, int32_t index, int32_t count)
	{
		X_ASSERT(index >= 0 && index + count < numVertices, "Invalid vertex range");
		if (!matrix) return;
		for (auto i = index, end = index + count; i < end; ++i)
		{
			auto pos = vertices[i].position;
			vertices[i].position.x = matrix->a * pos.x + matrix->c * pos.y + matrix->tx;
			vertices[i].position.y = matrix->b * pos.x + matrix->d * pos.y + matrix->ty;
		}
	}

	Rectangle *VertexData::bounds()
	{
		return boundsAfterTransformationOfVertices(nullptr, 0, numVertices);
	}

	Rectangle *VertexData::boundsAfterTransformation(Matrix *matrix)
	{
		return boundsAfterTransformationOfVertices(matrix, 0, numVertices);
	}

	Rectangle *VertexData::boundsAfterTransformationOfVertices(Matrix *matrix, int32_t index, int32_t count)
	{
		X_ASSERT(index >= 0 && index + count <= numVertices, "Invalid index range");
		if (!count) return nullptr;
		auto minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
		auto endIndex = index + count;
		if (matrix)
		{
			for (auto i = index; i < endIndex; ++i)
			{
				auto position = vertices[i].position;
				auto transformedPoint = matrix->transform(position.x, position.y);
				auto tfX = transformedPoint->x;
				auto tfY = transformedPoint->y;
				minX = MIN(minX, tfX);
				maxX = MAX(maxX, tfX);
				minY = MIN(minY, tfY);
				maxY = MAX(maxY, tfY);
			}
		}
		else
		{
			for (auto i = index; i < endIndex; ++i)
			{
				auto position = vertices[i].position;
				minX = MIN(minX, position.x);
				maxX = MAX(maxX, position.x);
				minY = MIN(minY, position.y);
				maxY = MAX(maxY, position.y);
			}
		}
		return Object::create<Rectangle>(minX, minY, maxX - minX, maxY - minY);
	}

	void VertexData::setNumVertices(int32_t numVertices)
	{
		if (numVertices != this->numVertices)
		{
			if (numVertices > 0)
			{
				if (vertices)
					vertices = static_cast<Vertex *>(realloc(vertices, sizeof(Vertex) * numVertices));
				else
					vertices = static_cast<Vertex *>(malloc(sizeof(Vertex) * numVertices));
				if (numVertices > this->numVertices)
				{
					memset(&vertices[this->numVertices], 0, sizeof(Vertex) * (numVertices - this->numVertices));
					for (auto i = this->numVertices; i < numVertices; ++i)
						vertices[i].color = VertexColorMakeWithColorAndAlpha(0, 1.0f);
				}
			}
			else
			{
				if (vertices)
				{
					free(vertices);
					vertices = nullptr;
				}
			}
			this->numVertices = numVertices;
		}
	}

	void VertexData::setPremultipliedAlpha(bool pma)
	{
		setPremultipliedAlpha(pma, true);
	}

	bool VertexData::getTinted()
	{
		for (auto i = 0; i < numVertices; ++i)
		{
			if (!isOpaqueWhite(vertices[i].color))
				return true;
		}
		return false;
	}

	VertexData *VertexData::copy()
	{
		auto value = Object::generate<VertexData>(numVertices, premultipliedAlpha);
		memcpy(value->vertices, vertices, sizeof(Vertex) * numVertices);
		return value;
	}
}
