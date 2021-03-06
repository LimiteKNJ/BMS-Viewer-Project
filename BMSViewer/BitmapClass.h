#pragma once

class TextureClass;
class ImageClass;
class BitmapClass {

private:
	struct VertexType{

		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass& other);
	~BitmapClass();

	bool Initialize(ID3D11Device*, int screenWidth, int screenHeight, WCHAR* ddsFilename,
		int bitmapWidth, int bitmapHeight, bool imgFlag);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int positionX, int positionY);
	
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetImage();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR* ddsFilename);
	bool LoadImage(ID3D11Device*, WCHAR* imgFilename);
	void ReleaseTexture();

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	TextureClass* m_Texture = nullptr;
	ImageClass* m_Image = nullptr;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_bitmapWidth = 0;
	int m_bitmapHeight = 0;
	int m_previousPosX = 0;
	int m_previousPosY = 0;
};