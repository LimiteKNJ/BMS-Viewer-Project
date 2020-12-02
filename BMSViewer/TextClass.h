#pragma once

class FontClass;
class FontShaderClass;

class TextClass : public AlignedAllocationPolicy<16>
{

private:

	struct SentenceType	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};


public:

	TextClass();
	TextClass(const TextClass& other);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*,
		HWND, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX worldMatrix, XMMATRIX orthoMatrix);

	bool SetMousePosition(ID3D11DeviceContext*, int mouseX, int mouseY);


private:

	bool InitializeSentence(SentenceType**, int maxlength, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char* text, int positionX, int positionY,
		float red, float green, float blue, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*,
		XMMATRIX worldMatrix, XMMATRIX orthoMatrix);


private:

	FontClass* m_Font = nullptr;
	FontShaderClass* m_FontShader = nullptr;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	XMMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};