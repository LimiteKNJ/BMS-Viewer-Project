#pragma once

class ImageClass
{
public:
	ImageClass();
	ImageClass(const ImageClass&);
	~ImageClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();


private:
	ID3D11Resource* m_resourse = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;
};