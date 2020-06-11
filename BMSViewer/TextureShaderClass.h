#pragma once

class TextureShaderClass : public AlignedAllocationPolicy<16> {

private:
	struct MatrixBufferType {

		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass& other);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int index,
		XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView*);

private:
	bool InitalizeShader(ID3D11Device*, HWND, WCHAR* vsFileName, WCHAR* psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR* shaderFileName);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int index);

private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;
};