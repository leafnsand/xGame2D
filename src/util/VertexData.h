#ifndef __X_VERTEX_H__
#define __X_VERTEX_H__

#include "base/Object.h"
#include "base/Copying.h"
#include "base/Math.h"
#include "geom/Point.h"
#include "geom/Matrix.h"
#include "geom/Rectangle.h"

#include <cstdlib>

namespace xGame2D
{
	typedef struct
	{
		float x;
		float y;
	} Position;

	typedef struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	} Color;

	typedef struct
	{
		Position position;
		Position texcoords;
		Color color;
	} Vertex;

	class VertexData : public Object, Copying
	{
	constructor:
		VertexData();
		~VertexData();

	public:
		OVERRIDE_DEFAULT_INIT
		bool init(int32_t numVertices);
		bool init(int32_t numVertices, bool pma);

		void copyTo(VertexData *target);
		void copyTo(VertexData *target, int32_t targetIndex);
		void copyTo(VertexData *target, int32_t targetIndex, int32_t count);
		Vertex vertexAtIndex(int32_t index);
		void setVertexAtIndex(Vertex vertex, int32_t index);
		void appendVertex(Vertex vertex);
		Point *positionAtIndex(int32_t index);
		void setPositionAtIndex(Point *position, int32_t index);
		void setPositionAtIndex(float x, float y, int32_t index);
		Point *texcoordsAtIndex(int32_t index);
		void setTexcoordsAtIndex(Point *texcoords, int32_t index);
		void setTexcoordsAtIndex(float x, float y, int32_t index);
		void setColorAtIndex(uint32_t color, float alpha, int32_t index);
		void setColorAtIndex(uint32_t color, int32_t index);
		void setColorOfAll(uint32_t color, float alpha);
		void setColorOfAll(uint32_t color);
		uint32_t colorAtIndex(int32_t index);
		float alphaAtIndex(int32_t index);
		void setAlphaAtIndex(float alpha, int32_t index);
		void setAlphaOfAll(float alpha);
		void scaleAlphaOfAll(float scale);
		void scaleAlphaOfVertices(float scale, int32_t index, int32_t count);
		void setPremultipliedAlpha(bool value, bool update);
		void transformVerticesWithMatrix(Matrix *matrix, int32_t index, int32_t count);
		Rectangle *bounds();
		Rectangle *boundsAfterTransformation(Matrix *matrix);
		Rectangle *boundsAfterTransformationOfVertices(Matrix *matrix, int32_t index, int32_t count);
		void setNumVertices(int32_t numVertices);
		void setPremultipliedAlpha(bool pma);
		bool getTinted();

		virtual VertexData *copy() override;

		inline Vertex *getVertices()
		{
			return this->vertices;
		}

		inline int32_t getNumVertices()
		{
			return this->numVertices;
		}

		inline bool getPremultipliedAlpha()
		{
			return this->premultipliedAlpha;
		}

	protected:
		Vertex *vertices;
		int32_t numVertices;
		bool premultipliedAlpha;
	};
}

#endif