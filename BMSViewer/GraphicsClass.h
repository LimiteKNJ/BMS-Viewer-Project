#pragma once

/////////////
// GLOBALS //
/////////////

// Windows â �⺻ ũ�� ����
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Ǯ��ũ�� �� â��� ����, ��������ȭ �ʱ� ����
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class BitmapClass;
class TextureShaderClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	BitmapClass* m_Bitmap = nullptr;

	TextureShaderClass* m_TextureShader = nullptr;
};