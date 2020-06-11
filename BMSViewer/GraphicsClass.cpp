#include "stdafx.h"
#include "d3dclass.h"
#include "graphicsclass.h"

#include "CameraClass.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	/* Direct3D 객체 생성 */
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if(!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	/* Camera 객체 생성 */
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 객체 초기화
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);

	/* Bitmap 객체 생성*/
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Bitmap 객체 초기화
	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight,
		L"resource/seafloor.dds", 256, 256))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	
	/* TextureShader 객체 생성*/
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// TextureShader 객체 초기화
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// ColorShader 객체 반환
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Direct3D 객체 반환
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Direct3D 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}


bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	// 그래픽 랜더링 처리
	return Render();
}


bool GraphicsClass::Render()
{
	// 버퍼의 색깔 지정
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Direct3D->TurnZBufferOff();

	int x = 250;
	int y = 250;
	m_Bitmap->Render(m_Direct3D->GetDeviceContext(), x, y);
	if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), x, y)) {

		return false;
	}
	
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture())) {

		return false;
	}

	m_Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}