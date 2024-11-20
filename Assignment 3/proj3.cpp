////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////
#define NOMINMAX
#include "d3dUtility.h"
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

// score and lives left
int g_score = 0;
int g_lives = 5;
bool game_started = false;

// font
ID3DXFont* g_font = NULL;
RECT rectStart, rectLives, rectScore;

// counts of balls and walls
const int ball_cnt = 56;
const int wall_cnt = 3;
const float d = 0.4;
float top_x = -3.8f;
float top_y = -1.55f;
float bot_y = -1.55f;
float left_x = top_x + 2 * d;
float right_x = top_x + 2 * d;
float left_y = top_y - 2 * d;
float right_y = top_y + 9 * d;
float bot_x = top_x + 13 * d;

// Initialize ball positions
const float spherePos[ball_cnt][2] = { {3.8f,-1},{4.3f,-1},{top_x + d,top_y + d * 8},{top_x + d,top_y - d},{left_x + 10 * d,left_y + d},{right_x + 10 * d,right_y - d},
									   {top_x,top_y},{top_x,top_y + d},{top_x,top_y + 2 * d},{top_x,top_y + 3 * d},{top_x,top_y + 4 * d},{top_x,top_y + 5 * d},{top_x,top_y + 6 * d},{top_x,top_y + 7 * d},
									   {bot_x,bot_y},{bot_x,bot_y + d},{bot_x,bot_y + 2 * d},{bot_x,bot_y + 3 * d},{bot_x,bot_y + 4 * d},{bot_x,bot_y + 5 * d},{bot_x,bot_y + 6 * d},{bot_x,bot_y + 7 * d},
									   {left_x,left_y},{left_x + d,left_y},{left_x + 2 * d,left_y},{left_x + 3 * d,left_y},{left_x + 4 * d,left_y},{left_x + 5 * d,left_y},{left_x + 6 * d,left_y},{left_x + 7 * d,left_y},{left_x + 8 * d,left_y},{left_x + 9 * d,left_y},
									   {right_x,right_y},{right_x + d,right_y},{right_x + 2 * d,right_y},{right_x + 3 * d,right_y},{right_x + 4 * d,right_y},{right_x + 5 * d,right_y},{right_x + 6 * d,right_y},{right_x + 7 * d,right_y},{right_x + 8 * d,right_y},{right_x + 9 * d,right_y},
									   {left_x + 6 * d,left_y + 2 * d},{left_x + 7 * d,left_y + 3 * d},{left_x + 8 * d,left_y + 4 * d},{left_x + 8 * d,left_y + 5 * d},{left_x + 8 * d,left_y + 6 * d},{left_x + 8 * d,left_y + 7 * d},{left_x + 7 * d,left_y + 8 * d},{left_x + 6 * d,left_y + 9 * d},
									   {left_x + d,left_y + 3 * d},{left_x + 2 * d,left_y + 3 * d},{left_x + 4 * d,left_y + 5 * d},{left_x + 5 * d,left_y + 5 * d},{left_x + d,left_y + 8 * d},{left_x + 2 * d,left_y + 8 * d}
};
// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[ball_cnt] = { d3d::RED,d3d::WHITE,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,
										 d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,
										 d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,
										 d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,
										 d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,
										 d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,
										 d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW,d3d::YELLOW
};

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private:
	float					center_x, center_y, center_z;
	float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;

public:
	CSphere(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
		m_pSphereMesh = NULL;
	}
	~CSphere(void) {}

	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
			return false;
		return true;
	}

	void destroy(void)
	{
		if (m_pSphereMesh != NULL) {
			m_pSphereMesh->Release();
			m_pSphereMesh = NULL;
		}
	}

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& red) {
		D3DXVECTOR3 this_center = getCenter();
		D3DXVECTOR3 red_center = red.getCenter();
		float dist = D3DXVec3Length(&(this_center - red_center));
		return (dist <= (getRadius() + red.getRadius()));
	}

	void hitBy(CSphere& other, CSphere* g_sphere)
	{
		if (&other == &g_sphere[0]) {
			if (hasIntersected(other)) {
				D3DXVECTOR3 vec = other.getCenter() - this->getCenter();
				D3DXVec3Normalize(&vec, &vec);

				D3DXVECTOR3 v_red(other.getVelocity_X(), 0, other.getVelocity_Z());

				float dot_product = D3DXVec3Dot(&v_red, &vec);
				D3DXVECTOR3 new_v = v_red - 2 * dot_product * vec;

				if (this != &g_sphere[0] && this != &g_sphere[1]) {
					this->setCenter(100.0f, 0.0f, 0.0f);
					g_score += 100;
				}
				other.setPower(new_v.x, new_v.z);
			}
		}
	}

	void ballUpdate(float timeDiff, CSphere* g_sphere)
	{
		if (this != &g_sphere[0]) return;
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if (vx > 0.01 || vz > 0.01) {
			float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
			float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

			if (g_sphere->getCenter().x > spherePos[0][0] + 0.5) {
				g_lives--;
				if (!g_lives || g_score == (ball_cnt - 2) * 100) {
					std::string end_msg = "Game Over!! (Final Score: " + std::to_string(g_score) + ")";
					const char* n_endmsg = end_msg.c_str();
					int result = MessageBox(
						NULL,
						n_endmsg,
						"Game Manager",
						MB_OK | MB_ICONEXCLAMATION
					);
					if (result == IDOK) exit(0);
				}
				tX = g_sphere[1].getCenter().x - 0.6;
				tZ = g_sphere[1].getCenter().z;
				g_sphere[0].setPower(0.0, 0.0);
				game_started = false;
			}
			this->setCenter(tX, cord.y, tZ);
		}
	}

	double getVelocity_X() { return this->m_velocity_x; }
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x = x;	center_y = y;	center_z = z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getRadius(void)  const { return (float)(M_RADIUS); }
	const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(center_x, center_y, center_z);
		return org;
	}

private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pSphereMesh;

};



// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:

	float					m_x;
	float					m_z;
	float                   m_width;
	float                   m_depth;
	float					m_height;

public:
	CWall(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_width = 0;
		m_depth = 0;
		m_pBoundMesh = NULL;
	}
	~CWall(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
	{
		if (NULL == pDevice)
			return false;

		m_mtrl.Ambient = color;
		m_mtrl.Diffuse = color;
		m_mtrl.Specular = color;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 5.0f;

		m_width = iwidth;
		m_depth = idepth;

		if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
			return false;
		return true;
	}
	void destroy(void)
	{
		if (m_pBoundMesh != NULL) {
			m_pBoundMesh->Release();
			m_pBoundMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return;
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
	}

	bool hasIntersected(CSphere& ball) {
		D3DXVECTOR3 ballCenter = ball.getCenter();
		float ballRadius = ball.getRadius();

		if (this->m_x == 0) {
			if (this->m_z < 0) // left wall
				return (ballCenter.z - ballRadius <= m_z + m_depth / 2);
			if (this->m_z > 0) // right wall
				return (ballCenter.z + ballRadius >= m_z - m_depth / 2);
		}
		else { // top wall
			return (ballCenter.x - ballRadius <= m_x + m_width / 2);
		}
	}

	void hitBy(CSphere& ball) {
		if (hasIntersected(ball)) {
			if (this->m_x == 0)
				ball.setPower(ball.getVelocity_X(), -ball.getVelocity_Z());
			else
				ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());
		}
	}

	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}

	float getHeight(void) const { return M_HEIGHT; }

private:
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh* m_pBoundMesh;
};

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
public:
	CLight(void)
	{
		static DWORD i = 0;
		m_index = i++;
		D3DXMatrixIdentity(&m_mLocal);
		::ZeroMemory(&m_lit, sizeof(m_lit));
		m_pMesh = NULL;
		m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bound._radius = 0.0f;
	}
	~CLight(void) {}
public:
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
	{
		if (NULL == pDevice)
			return false;
		if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
			return false;

		m_bound._center = lit.Position;
		m_bound._radius = radius;

		m_lit.Type = lit.Type;
		m_lit.Diffuse = lit.Diffuse;
		m_lit.Specular = lit.Specular;
		m_lit.Ambient = lit.Ambient;
		m_lit.Position = lit.Position;
		m_lit.Direction = lit.Direction;
		m_lit.Range = lit.Range;
		m_lit.Falloff = lit.Falloff;
		m_lit.Attenuation0 = lit.Attenuation0;
		m_lit.Attenuation1 = lit.Attenuation1;
		m_lit.Attenuation2 = lit.Attenuation2;
		m_lit.Theta = lit.Theta;
		m_lit.Phi = lit.Phi;
		return true;
	}
	void destroy(void)
	{
		if (m_pMesh != NULL) {
			m_pMesh->Release();
			m_pMesh = NULL;
		}
	}
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
	{
		if (NULL == pDevice)
			return false;

		D3DXVECTOR3 pos(m_bound._center);
		D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
		D3DXVec3TransformCoord(&pos, &pos, &mWorld);
		m_lit.Position = pos;

		pDevice->SetLight(m_index, &m_lit);
		pDevice->LightEnable(m_index, TRUE);
		return true;
	}

	void draw(IDirect3DDevice9* pDevice)
	{
		if (NULL == pDevice)
			return;
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
		pDevice->SetTransform(D3DTS_WORLD, &m);
		pDevice->SetMaterial(&d3d::WHITE_MTRL);
	}

	D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh* m_pMesh;
	d3d::BoundingSphere m_bound;
};


// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[wall_cnt];
CSphere	g_sphere[ball_cnt];
CLight	g_light;

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
	int i;

	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position
	if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, d3d::GREEN)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::BLACK)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::BLACK)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::BLACK)) return false;
	g_legowall[2].setPosition(-4.56f, 0.12f, 0.0f);

	// create balls and set the position
	for (i = 0; i < ball_cnt; i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
		g_sphere[i].setPower(0, 0);
	}

	// font setting
	if (FAILED(D3DXCreateFont(
		Device,                // D3D device
		25,                    // Height of the font
		0,                     // Width of the font
		FW_NORMAL,               // Font weight (FW_NORMAL for regular)
		1,                     // MipLevels
		FALSE,                 // Italic
		DEFAULT_CHARSET,       // CharSet
		OUT_DEFAULT_PRECIS,    // OutputPrecision
		DEFAULT_QUALITY,       // Quality
		DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
		"Arial",               // Font name
		&g_font)))             // Font pointer
	{
		MessageBox(0, "Font creation failed", 0, 0);
		return false;
	}


	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera.
	D3DXVECTOR3 pos(15.0f, 20.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 8,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < wall_cnt; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
	if (g_font != NULL) {
		g_font->Release();
		g_font = NULL;
	}
}

// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;

	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		// update the position of each ball
		for (int i = 0; i < ball_cnt; i++) {
			g_sphere[i].ballUpdate(timeDelta, g_sphere);
			// check collisions with walls
			for (int j = 0; j < wall_cnt; j++) {
				g_legowall[j].hitBy(g_sphere[i]);
			}
		}

		for (int i = 0; i < ball_cnt; i++) {
			g_sphere[i].hitBy(g_sphere[0], g_sphere);
		}

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i < wall_cnt; i++) {
			g_legowall[i].draw(Device, g_mWorld);
		}
		for (i = 0; i < ball_cnt; i++) {
			g_sphere[i].draw(Device, g_mWorld);
		}
		g_light.draw(Device);

		const int margin = 60;
		rectLives = { margin - 20, margin - 20, margin + 200, margin + 40 };
		rectScore = { Width - margin - 200, margin - 20, Width - margin, margin + 40 };
		rectStart = { (Width / 2) - 150, Height - margin - 100, (Width / 2) + 150, Height - margin };

		// draw "Lives Left" label
		std::string lives_txt = "Lives Left: " + std::to_string(g_lives);
		const char* lives = lives_txt.c_str();
		g_font->DrawText(NULL, lives, -1, &rectLives, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 0, 0));

		// draw "Score" label
		std::string score_txt = "Score: " + std::to_string(g_score);
		const char* score = score_txt.c_str();
		g_font->DrawText(NULL, score, -1, &rectScore, DT_RIGHT | DT_TOP, D3DCOLOR_XRGB(0, 0, 0));

		// draw "Press Space to START" label
		if (!game_started) {
			g_font->DrawText(NULL, "Press Space to START", -1, &rectStart, DT_CENTER | DT_BOTTOM, D3DCOLOR_XRGB(0, 0, 0));
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static int old_x = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;

	switch (msg) {
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_ESCAPE:
			::DestroyWindow(hwnd);
			break;
		case VK_SPACE:
			if (!game_started) {
				g_sphere[0].setPower(-2.0f, 0.0f);
				game_started = true;
			} 
			break;
		case VK_LEFT:
			g_sphere[1].setCenter(
				g_sphere[1].getCenter().x,
				g_sphere[1].getCenter().y,
				g_sphere[1].getCenter().z - 0.1);
			if (!game_started) g_sphere[0].setCenter(g_sphere[1].getCenter().x - 0.5, g_sphere[1].getCenter().y, g_sphere[1].getCenter().z);
			break;
		case VK_RIGHT:
			g_sphere[1].setCenter(
				g_sphere[1].getCenter().x,
				g_sphere[1].getCenter().y,
				g_sphere[1].getCenter().z + 0.1);
			if (!game_started) g_sphere[0].setCenter(g_sphere[1].getCenter().x - 0.5, g_sphere[1].getCenter().y, g_sphere[1].getCenter().z);
			break;
		}
	}

	case WM_MOUSEMOVE:
	{
		int new_z = LOWORD(lParam);
		float normalized_z = (float(new_z) / Height) * 6.0f - 3.0f;

		float wall_min_z = -3.0f + g_sphere[0].getRadius();
		float wall_max_z = 3.0f - g_sphere[0].getRadius();

		normalized_z = std::max(wall_min_z, std::min(normalized_z, wall_max_z));

		if (normalized_z > wall_min_z && normalized_z < wall_max_z) {
			if (!game_started) {
				g_sphere[0].setCenter(g_sphere[0].getCenter().x, g_sphere[0].getCenter().y, normalized_z);
			}
			g_sphere[1].setCenter(g_sphere[1].getCenter().x, g_sphere[1].getCenter().y, normalized_z);
		}
	}
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}
