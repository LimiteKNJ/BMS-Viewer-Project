#pragma once

class TextureClass;
class ImageClass;

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass& other);
	~FontClass();

	bool Initialize(ID3D11Device*, char* fontFileName, WCHAR* textureFileName);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	bool LoadFontData(char* fontFileName);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR* textureFileName);
	void ReleaseTexture();

private:
	FontType* m_Font = nullptr;
	TextureClass* m_Texture = nullptr;
	ImageClass* m_Image = nullptr;
};