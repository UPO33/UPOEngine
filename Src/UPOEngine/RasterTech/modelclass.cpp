////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

#include "../Core/UCore.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
#if 0
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
#endif // 0
	

	Assimp::Importer importer;
	int flag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	const aiScene* scene = importer.ReadFile("building1.obj", flag);
	if (scene == nullptr)
	{
		ULOG_ERROR("loading model failed");
		return false;
	}
	ULOG_MESSAGE("NumMeshes %d", scene->mNumMeshes);
	ULOG_MESSAGE("NumVerts %d", scene->mMeshes[0]->mNumVertices);
	ULOG_MESSAGE("NumTexCoord %d", scene->mMeshes[0]->GetNumUVChannels());
	ULOG_MESSAGE("numFace %d", scene->mMeshes[0]->mNumFaces);

	aiMesh* mesh = scene->mMeshes[0];

	VertexType* vertices = new VertexType[mesh->mNumVertices];
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i].position = D3DXVECTOR3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices[i].normal = D3DXVECTOR3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->GetNumUVChannels())
			vertices[i].texture = D3DXVECTOR2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	unsigned* indices = new unsigned [mesh->mNumFaces * 3];
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	m_vertexCount = mesh->mNumVertices;
	m_indexCount = mesh->mNumFaces * 3;

	GFXVertexBuffer_Desc vbDesc;
	vbDesc.mInitialData = vertices;
	vbDesc.mSize = sizeof(VertexType) * m_vertexCount;
	vbDesc.mUsage = EBU_IMMUTABLE;

	mVertexBuffer = gDevice->CreateVertexBuffer(vbDesc);
	if (mVertexBuffer == nullptr) return false;
	GFXIndexBuffer_Desc ibDesc;
	ibDesc.mInitialData = indices;
	ibDesc.mSize = sizeof(unsigned) * m_indexCount;
	ibDesc.mUsage = EBU_IMMUTABLE;
	ibDesc.mType = EBT_UINT;
	mIndexBuffer = gDevice->CreateIndexBuffer(ibDesc);
	if (mIndexBuffer == nullptr) return false;


	D3D11_BUFFER_DESC vBufferDesc;
	vBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;
	vBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = vertices;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;


	D3D11_BUFFER_DESC iBufferDesc;
	iBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	iBufferDesc.ByteWidth = m_indexCount * sizeof(unsigned);
	iBufferDesc.CPUAccessFlags = 0;
	iBufferDesc.MiscFlags = 0;
	iBufferDesc.StructureByteStride = 0;
	iBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vBufferDesc, &vData, &m_vertexBuffer)))
	{
		ULOG_ERROR("creating vertex buffer failed");
	}
	if (FAILED(device->CreateBuffer(&iBufferDesc, &iData, &m_indexBuffer)))
	{
		ULOG_ERROR("creating index buffer failed");
	}
	return true;
}


void ModelClass::ShutdownBuffers()
{
	if (mIndexBuffer)mIndexBuffer->Release();
	if (mVertexBuffer) mVertexBuffer->Release();
	// Release the index buffer.
// 	if(m_indexBuffer)
// 	{
// 		m_indexBuffer->Release();
// 		m_indexBuffer = 0;
// 	}
// 
// 	// Release the vertex buffer.
// 	if(m_vertexBuffer)
// 	{
// 		m_vertexBuffer->Release();
// 		m_vertexBuffer = 0;
// 	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
// 	unsigned int stride;
// 	unsigned int offset;
// 	// Set vertex buffer stride and offset.
// 	stride = sizeof(VertexType); 
// 	offset = 0;
// 	// Set the vertex buffer to active in the input assembler so it can be rendered.
// 	//deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
// 	deviceContext->IASetVertexBuffers(0, 1, &(mVertexBuffer->As<GFXVertexBufferDX>()->mHandle), &stride, &offset);
// 
//     // Set the index buffer to active in the input assembler so it can be rendered.
// // 	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
// // 
// //     // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
// // 	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// // 
	gDevice->BinVertexBuffer(mVertexBuffer, sizeof(VertexType), 0);
	gDevice->BinIndexBuffer(mIndexBuffer, 0);
	gDevice->SetPrimitiveTopology(EPrimitiveTopology::EPT_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	return true;
	
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}