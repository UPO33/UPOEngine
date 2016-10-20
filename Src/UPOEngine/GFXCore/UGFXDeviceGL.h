#pragma once

#include "UGFXDevice.h"

#include <gl\glew.h>

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class GFXDeviceGL;


	//////////////////////////////////////////////////////////////////////////
	GLenum ToGLType(EMapFlag in)
	{
		switch (in)
		{
		case EMapFlag::EMF_READ: return GL_READ_ONLY;
			break;
		case EMapFlag::EMF_WRITE:
		case EMapFlag::EMF_WRITE_DISCARD:
		case EMapFlag::EMF_WRITE_NO_OVERWRITE:
			return GL_WRITE_ONLY;
			break;
		case EMapFlag::EMF_READ_WRITE: return GL_READ_WRITE;
			break;
		}

		UASSERT(true);
		return 0;
	}
	GLenum ToGLType(EPrimitiveTopology in)
	{
		//TODO
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	class GFXVertexBufferGL : public GFXVertexBuffer
	{
		friend GFXDeviceGL;
	public:
		GLuint mHandle;

		GFXVertexBufferGL(GLuint handle, const GFXVertexBuffer_Desc& desc) : mHandle(handle)
		{
			mDesc = desc;
		}
		~GFXVertexBufferGL()
		{
			if (mHandle) glDeleteBuffers(1, &mHandle);
			mHandle = 0;
		}
		
		void* Map(EMapFlag flag) override
		{
			glMapBuffer(GL_ARRAY_BUFFER, ToGLType(flag));
			return nullptr;
		}
		void Unmap() override
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXIndexBufferGL : public GFXIndexBuffer
	{
		friend GFXDeviceGL;
	public:
		GLuint mHandle;

		GFXIndexBufferGL(GLuint handle, const GFXIndexBuffer_Desc& desc) : mHandle(handle)
		{
			mDesc = desc;
		}
		~GFXIndexBufferGL()
		{
			if (mHandle) glDeleteBuffers(1, &mHandle);
			mHandle = 0;
		}

		void* Map(EMapFlag flag) override
		{
			//@access : GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE.

			glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, ToGLType(flag));
			return nullptr;
		}
		void Unmap() override
		{
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
	};


	class GFXDeviceGL : public GFXDevice
	{
		GLuint mPrimitiveTopology = 0;
		GFXIndexBuffer* mBindedIndexBuffer = nullptr;

		GLuint CreateBuffer(GLuint target, unsigned size, void* initialData, GLuint usage)
		{
			GLuint handle = 0;
			glGenBuffers(1, &handle);
			glBindBuffer(target, handle);
			glBufferData(target, size, initialData, usage);
			glBindBuffer(target, 0);
			return handle;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXVertexBuffer* CreateVertexBuffer(const GFXVertexBuffer_Desc& param) override
		{
			GLenum usage = param.mUsage == EResourceUsage::EBU_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			GLuint handle = CreateBuffer(GL_ARRAY_BUFFER, param.mSize, param.mInitialData, usage);
			if (handle)
				return new GFXVertexBufferGL(handle, param);

			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXIndexBuffer* CreateIndexBuffer(const GFXIndexBuffer_Desc& param) override
		{
			GLenum usage = param.mUsage == EResourceUsage::EBU_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			GLuint handle = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, param.mSize, param.mInitialData, usage);
			if (handle)
				return new GFXIndexBufferGL(handle, param);

			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		void BinVertexBuffer(const GFXVertexBuffer* buffer, unsigned stride, unsigned offset) override
		{
			if (buffer)
			{
				glBindBuffer(GL_ARRAY_BUFFER, buffer->As<GFXVertexBufferGL>()->mHandle);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		void Draw(unsigned vertexCount, unsigned startVertexLocation /* = 0 */) override
		{
			glDrawArrays(mPrimitiveTopology, startVertexLocation, vertexCount);
		}
		//////////////////////////////////////////////////////////////////////////
		void DrawIndexed(unsigned indexCount, unsigned startIndexLocation /* = 0 */, unsigned baseVertexLocation /* = 0 */) override
		{
			glDrawElements(mPrimitiveTopology, indexCount, GL_UNSIGNED_SHORT, nullptr);
		}
		//////////////////////////////////////////////////////////////////////////
		void SetPrimitiveTopology(EPrimitiveTopology topology) override
		{
			mPrimitiveTopology = ToGLType(topology);
		}
		//////////////////////////////////////////////////////////////////////////

	};
};