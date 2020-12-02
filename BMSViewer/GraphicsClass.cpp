#include "stdafx.h"
#include "d3dclass.h"
#include "graphicsclass.h"

#include "CameraClass.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"
#include "TextClass.h"


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
	if (!m_Camera) {
		return false;
	}

	// 카메라 객체 초기화
	XMMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	/* Bitmap 객체 생성*/
	m_Bitmap_UIpng = new BitmapClass;
	if (!m_Bitmap_UIpng) {
		return false;
	}

	if (!m_Bitmap_UIpng->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight,
		L"resource/MainFrameUI.png", 640, 360, true))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	/* Text 객체 생성*/
	m_Text = new TextClass;
	if (!m_Text) {
		return false;
	}

	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd,
		screenWidth, screenHeight, baseViewMatrix))	{

		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
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
	// TextureShader 객체 반환
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Bitmap 객체 반환
	if (m_Bitmap_UIdds)
	{
		m_Bitmap_UIdds->Shutdown();
		delete m_Bitmap_UIdds;
		m_Bitmap_UIdds = 0;
	}

	if (m_Bitmap_UIpng)
	{
		m_Bitmap_UIpng->Shutdown();
		delete m_Bitmap_UIpng;
		m_Bitmap_UIpng = 0;
	}

	// Text 객체 반환
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Camera 객체 반환
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


bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	/* 동적 변경 사항 구현 */

	if (!m_Text->SetMousePosition(m_Direct3D->GetDeviceContext(), mouseX, mouseY)) {
		return false;
	}

	/* 동적 변경 사항 구현 끝 */

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	return true;
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
	m_Direct3D->TurnOnAlphaBlending();

	/* 실제 UI 구현 부분 */

	if (!m_Bitmap_UIpng->Render(m_Direct3D->GetDeviceContext(), 0, 0)) {
		return false;
	}

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap_UIpng->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Bitmap_UIpng->GetImage())) {

		return false;
	}

	if (!m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix)) {
		return false; // Text의 처리
	}

	/* 실제 UI 구현 부분 끝*/

	m_Direct3D->TurnOffAlphaBlending();
	m_Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}