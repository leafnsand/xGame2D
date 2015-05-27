#include "display/QuadBatch.h"
#include "display/Quad.h"
#include "display/BlendMode.h"
#include "display/DisplayObjectContainer.h"
#include "platform/OpenGL.h"
#include "render/BaseEffect.h"
#include "render/RenderSupport.h"
#include "textures/Texture.h"
#include "util/VertexData.h"
#include "util/Console.h"

namespace xGame2D
{
	QuadBatch::QuadBatch(): numQuads(0), 
		syncRequired(false), 
		premultipliedAlpha(false), 
		tinted(false), 
		texture(nullptr), 
		baseEffect(nullptr), 
		vertexData(nullptr), 
		vertexBufferName(0), 
		indexBufferName(0), 
		indexData(nullptr)
	{
	}

	QuadBatch::~QuadBatch()
	{
		if (indexData)
		{
			free(indexData);
		}
		glDeleteBuffers(1, &vertexBufferName);
		glDeleteBuffers(1, &indexBufferName);
		if (texture) texture->release();
		if (vertexData) vertexData->release();
		if (baseEffect) baseEffect->release();
	}

	bool QuadBatch::init()
	{
		return init(0);
	}

	bool QuadBatch::init(int32_t capacity)
	{
		vertexData = Object::generate<VertexData>();
		baseEffect = Object::generate<BaseEffect>();
		if (capacity > 0)
		{
			setCapacity(capacity);
		}
		return true;
	}

	void QuadBatch::reset()
	{
		numQuads = 0;
		syncRequired = true;
		baseEffect->setTexture(nullptr);
		if (texture) texture->release();
		texture = nullptr;
	}

	void QuadBatch::addQuad(Quad *quad)
	{
		addQuad(quad, quad->getAlpha(), quad->getBlendMode(), nullptr);
	}

	void QuadBatch::addQuad(Quad *quad, float alpha)
	{
		addQuad(quad, alpha, quad->getBlendMode(), nullptr);
	}

	void QuadBatch::addQuad(Quad *quad, float alpha, uint32_t blendMode)
	{
		addQuad(quad, alpha, blendMode, nullptr);
	}

	void QuadBatch::addQuad(Quad *quad, float alpha, uint32_t blendMode, Matrix *matrix)
	{
		if (!matrix) matrix = quad->getTransformationMatrix();
		if (numQuads + 1 > getCapacity()) expand();
		if (numQuads == 0)
		{
			texture = quad->getTexture();
			if (texture) texture->retain();
			premultipliedAlpha = quad->getPremultipliedAlpha();
			setBlendMode(blendMode);
			vertexData->setPremultipliedAlpha(premultipliedAlpha, false);
		}
		auto vertexID = numQuads * 4;
		quad->copyVertexDataTo(vertexData, vertexID);
		vertexData->transformVerticesWithMatrix(matrix, vertexID, 4);
		if (alpha != 1.0f)
		{
			vertexData->scaleAlphaOfVertices(alpha, vertexID, 4);
		}
		if (!tinted)
		{
			tinted = alpha != 1.0f || quad->getTinted();
		}
		syncRequired = true;
		++numQuads;
	}

	void QuadBatch::addQuadBatch(QuadBatch *quadBatch)
	{
		addQuadBatch(quadBatch, quadBatch->getAlpha(), quadBatch->getBlendMode(), nullptr);
	}

	void QuadBatch::addQuadBatch(QuadBatch *quadBatch, float alpha)
	{
		addQuadBatch(quadBatch, alpha, quadBatch->getBlendMode(), nullptr);
	}

	void QuadBatch::addQuadBatch(QuadBatch *quadBatch, float alpha, uint32_t blendMode)
	{
		addQuadBatch(quadBatch, alpha, blendMode, nullptr);
	}

	void QuadBatch::addQuadBatch(QuadBatch *quadBatch, float alpha, uint32_t blendMode, Matrix *matrix)
	{
		auto vertexID = numQuads * 4;
		auto numQuadsOfBatch = quadBatch->getNumQuads();
		auto numVertices = numQuads * 4;
		if (!matrix) matrix = quadBatch->getTransformationMatrix();
		if (numQuads + numQuadsOfBatch > getCapacity()) setCapacity(numQuads + numQuadsOfBatch);
		if (numQuads == 0)
		{
			texture = quadBatch->getTexture();
			texture->retain();
			premultipliedAlpha = quadBatch->getPremultipliedAlpha();
			setBlendMode(blendMode);
			vertexData->setPremultipliedAlpha(premultipliedAlpha, false);
		}
		quadBatch->vertexData->copyTo(vertexData, vertexID, numVertices);
		vertexData->transformVerticesWithMatrix(matrix, vertexID, numVertices);
		if (alpha != 1.0f)
		{
			vertexData->scaleAlphaOfVertices(alpha, vertexID, numVertices);
		}
		if (!tinted)
		{
			tinted = alpha != 1.0f || quadBatch->getTinted();
		}
		syncRequired = true;
		numQuads += numQuadsOfBatch;
	}

	bool QuadBatch::isStateChangeWithTinted(bool tinted, Texture *texture, float alpha, bool pma, uint32_t blendMode, int32_t numQuads)
	{
		if (this->numQuads == 0) return false;
		else if (this->numQuads + numQuads > 8192) return true; // maximum buffer size
		else if (!this->texture && !texture)
			return this->premultipliedAlpha != pma || getBlendMode() != blendMode;
		else if (this->texture && texture)
			return this->tinted != (tinted || alpha != 1.0f) ||
				this->texture->getName() != texture->getName() ||
				getBlendMode() != blendMode;
		else return true;
	}

	void QuadBatch::renderWithMvpMatrix(Matrix *matrix)
	{
		renderWithMvpMatrix(matrix, 1.0f, getBlendMode());
	}

	void QuadBatch::renderWithMvpMatrix(Matrix *matrix, float alpha, uint32_t blendMode)
	{
		if (!numQuads) return;
		if (syncRequired) syncBuffers();
		X_ASSERT(blendMode != BlendModeAuto, "cannot render object with blend mode AUTO");
		baseEffect->setTexture(texture);
		baseEffect->setPremultipliedAlpha(premultipliedAlpha);
		baseEffect->setMvpMatrix(matrix);
		baseEffect->setUseTinting(tinted || alpha != 1.0f);
		baseEffect->setAlpha(alpha);
		baseEffect->prepareToDraw();
		BlendMode::applyBlendFactorsForBlendMode(blendMode, premultipliedAlpha);
		auto attribPosition = baseEffect->getAttribPosition();
		auto attribColor = baseEffect->getAttribColor();
		auto attribTexcoord = baseEffect->getAttribTexcoord();
		glEnableVertexAttribArray(attribPosition);
		if (attribColor != -1)
		{
			glEnableVertexAttribArray(attribColor);
		}
		if (texture)
		{
			glEnableVertexAttribArray(attribTexcoord);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);
		glVertexAttribPointer(attribPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, position)));
		if (attribColor != -1)
		{
			glVertexAttribPointer(attribColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)(offsetof(Vertex, color)));
		}
		if (texture)
		{
			glVertexAttribPointer(attribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, texcoords)));
		}
		auto numIndices = numQuads * 6;
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);
	}

	void QuadBatch::render(RenderSupport *support)
	{
		if (numQuads)
		{
			support->finishQuadBatch();
			support->addDrawCalls(1);
			renderWithMvpMatrix(support->getMvpMatrix(), support->getAlpha(), support->getBlendMode());
		}
	}

	Rectangle *QuadBatch::boundsInSpace(DisplayObject *targetSpace)
	{
		auto matrix = targetSpace == this ? nullptr : transformationMatrixToSpace(targetSpace);
		return vertexData->boundsAfterTransformationOfVertices(matrix, 0, numQuads * 4);
	}

	std::vector<QuadBatch *> *QuadBatch::compileObject(DisplayObject *object)
	{
		return compileObject(object, nullptr);
	}

	std::vector<QuadBatch *> *QuadBatch::compileObject(DisplayObject *object, std::vector<QuadBatch *> *quadBatches)
	{
		if (!quadBatches) quadBatches = new std::vector<QuadBatch *>;
		compile(object, quadBatches, -1, Object::create<Matrix>(), 1.0f, BlendModeAuto);
		return quadBatches;
	}

	int32_t QuadBatch::compile(DisplayObject *object, std::vector<QuadBatch *> *quadBatches, int32_t quadBatchID, Matrix *transformationMatrix, float alpha, uint32_t blendMode)
	{
		auto isRootObject = false;
		auto objectAlpha = object->getAlpha();
		auto quad = dynamic_cast<Quad *>(object);
		auto batch = dynamic_cast<QuadBatch *>(object);
		auto container = dynamic_cast<DisplayObjectContainer *>(object);
		if (quadBatchID == -1)
		{
			isRootObject = true;
			quadBatchID = 0;
			objectAlpha = 1.0f;
			blendMode = object->getBlendMode();
			if (quadBatches->size() == 0)
			{
				quadBatches->push_back(Object::generate<QuadBatch>());
			}
			else
			{
				quadBatches->at(0)->reset();
			}
		}
		if (container)
		{
			auto childMatrix = Object::create<Matrix>();
			auto children = container->getChildren();
			for (auto child : children)
			{
				if (child->hasVisibleArea())
				{
					auto childBlendMode = child->getBlendMode();
					if (childBlendMode == BlendModeAuto) childBlendMode = blendMode;
					childMatrix->clone(transformationMatrix);
					childMatrix->prependMatrix(child->getTransformationMatrix());
					quadBatchID = compile(child, quadBatches, quadBatchID, childMatrix, alpha * objectAlpha, childBlendMode);
				}
			}
		}
		else if (quad || batch)
		{
			auto texture = quad ? quad->getTexture() : batch->getTexture();
			auto tinted = quad ? quad->getTinted() : batch->getTinted();
			auto pma = quad ? quad->getPremultipliedAlpha() : batch->getPremultipliedAlpha();
			auto numQuad = batch ? batch->getNumQuads() : 1;
			auto currentBatch = quadBatches->at(quadBatchID);
			if (currentBatch->isStateChangeWithTinted(tinted, texture, alpha * objectAlpha, pma, blendMode, numQuad))
			{
				++quadBatchID;
				if (static_cast<int32_t>(quadBatches->size()) <= quadBatchID)
				{
					quadBatches->push_back(Object::generate<QuadBatch>());
				}
				currentBatch = quadBatches->at(quadBatchID);
				currentBatch->reset();
			}
			if (quad)
			{
				currentBatch->addQuad(quad, alpha * objectAlpha, blendMode, transformationMatrix);
			}
			else
			{
				currentBatch->addQuadBatch(batch, alpha * objectAlpha, blendMode, transformationMatrix);
			}
		}
		else
		{
			Console::error << "Unsupported display object" << Console::endl;
		}
		if (isRootObject)
		{
			for (auto i = static_cast<int32_t>(quadBatches->size()) - 1; i > quadBatchID; --i)
			{
				auto pop = quadBatches->at(i);
				quadBatches->pop_back();
				pop->release();
			}
		}
		return quadBatchID;
	}

	int32_t QuadBatch::getNumQuads()
	{
		return numQuads;
	}

	bool QuadBatch::getTinted()
	{
		return tinted;
	}

	Texture *QuadBatch::getTexture()
	{
		return texture;
	}

	bool QuadBatch::getPremultipliedAlpha()
	{
		return premultipliedAlpha;
	}

	int32_t QuadBatch::getCapacity()
	{
		return vertexData->getNumVertices() / 4;
	}

	void QuadBatch::setCapacity(int32_t value)
	{
		X_ASSERT(value > 0, "capacity must not be zero");
		auto oldCapacity = getCapacity();
		auto numVertices = value * 4;
		auto numIndices = value * 6;
		vertexData->setNumVertices(numVertices);
		if (!indexData)
		{
			indexData = static_cast<unsigned short *>(malloc(sizeof(unsigned short) * numIndices));
		}
		else
		{
			indexData = static_cast<unsigned short *>(realloc(indexData, sizeof(unsigned short) * numIndices));
		}
		for (auto i = oldCapacity; i < value; ++i)
		{
			indexData[i * 6] = i * 4;
			indexData[i * 6 + 1] = i * 4 + 1;
			indexData[i * 6 + 2] = i * 4 + 2;
			indexData[i * 6 + 3] = i * 4 + 1;
			indexData[i * 6 + 4] = i * 4 + 3;
			indexData[i * 6 + 5] = i * 4 + 2;
		}
		destroyBuffers();
		syncRequired = true;
	}

	void QuadBatch::expand()
	{
		auto oldCapacity = getCapacity();
		setCapacity(oldCapacity < 8 ? 16 : oldCapacity * 2);
	}

	void QuadBatch::createBuffers()
	{
		destroyBuffers();
		auto numVertices = vertexData->getNumVertices();
		auto numIndices = numVertices / 4 * 6;
		if (numVertices == 0) return;
		glGenBuffers(1, &vertexBufferName);
		glGenBuffers(1, &indexBufferName);
		X_ASSERT(vertexBufferName && indexBufferName, "could not create vertex buffers");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indexData, GL_STATIC_DRAW);
		syncRequired = true;
	}

	void QuadBatch::destroyBuffers()
	{
		if (vertexBufferName)
		{
			glDeleteBuffers(1, &vertexBufferName);
			vertexBufferName = 0;
		}

		if (indexBufferName)
		{
			glDeleteBuffers(1, &indexBufferName);
			indexBufferName = 0;
		}
	}

	void QuadBatch::syncBuffers()
	{
		if (!vertexBufferName)
		{
			createBuffers();
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexData->getNumVertices(), vertexData->getVertices(), GL_STATIC_DRAW);
		syncRequired = false;
	}
}
