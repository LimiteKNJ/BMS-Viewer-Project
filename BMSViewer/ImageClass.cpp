#include "stdafx.h"
#include "ImageClass.h"


ImageClass::ImageClass()
{
}


ImageClass::ImageClass(const ImageClass& other)
{
}


ImageClass::~ImageClass()
{
}



bool ImageClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	// 텍스처를 파일로부터 읽어온다
	if (FAILED(CreateWICTextureFromFile(device, filename, nullptr, &m_texture)))
	{
		return false;
	}

	return true;
}



void ImageClass::Shutdown()
{
	//텍스처 뷰 리소스를 해제한다.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}


ID3D11ShaderResourceView* ImageClass::GetTexture()
{
	return m_texture;
}
