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
	// �ؽ�ó�� ���Ϸκ��� �о�´�
	if (FAILED(CreateWICTextureFromFile(device, filename, nullptr, &m_texture)))
	{
		return false;
	}

	return true;
}



void ImageClass::Shutdown()
{
	//�ؽ�ó �� ���ҽ��� �����Ѵ�.
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
