#pragma once
#include "options.hpp"
#include <math.h>
#include "helpers\math.hpp"

static void DrawString(int x, int y, Color clr, vgui::HFont font, const char* text, bool padX = false)
{
	if (text == NULL)
		return;

	wchar_t buffer[1024] = { '\0' };
	wsprintfW(buffer, L"%S", text);

	if (padX)
	{
		int tw, th;
		vguisurface->GetTextSize(font, buffer, tw, th);
		vguisurface->DrawSetTextPos(x - (tw / 2), y);
	}
	else
		vguisurface->DrawSetTextPos(x, y);
	

	vguisurface->DrawSetTextFont(font);
	vguisurface->DrawSetTextColor(clr);
	vguisurface->DrawPrintText(buffer, wcslen(buffer));

}

static void DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	vguisurface->DrawSetColor(col);
	vguisurface->DrawLine(x0, y0, x1, y1);
}

static void DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawOutlinedRect(x, y, w, h);
}

static void DrawFilledRect(int x, int y, int w, int h, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
}

static void DrawPixel(int x, int y, Color col)
{
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawFilledRect(x, y, x + 1, y + 1);
}

static void FilledCircle(Vector2D position, float points, float radius, Color color)
{

	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

	vguisurface->DrawSetColor(color);
	vguisurface->DrawTexturedPolygon((int)points, vertices.data(), true);
}

static void DrawCircle(float x, float y, float r, float s, Color color)
{
	float Step = M_PI * 2.0 / s;
	for (float a = 0; a < (M_PI*2.0); a += Step)
	{
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;
		DrawLine(x1, y1, x2, y2, color);
	}
}

static Color HealthBased(C_BasePlayer* entity)
{
	if (!entity) return Color(0.f, 0.f, 0.f);

	float g = ((float)entity->m_iHealth() / 100.f);
	return Color(1.f - g, g, 0.f);
}

static Color ColorFromHue(double hue, int intensity) {
	int hi = (int)(floor(hue / 60)) % 6;
	double f = hue / 60 - floor(hue / 60);
	int v = (int)(intensity);
	int p = (int)(intensity * (1 - 1));
	int q = (int)(intensity * (1 - f * 1));
	int t = (int)(intensity * (1 - (1 - f) * 1));
	if (hi == 0) return Color( v, t, p, 255);
	else if (hi == 1) return Color( q, v, p, 255);
	else if (hi == 2) return Color( p, v, t, 255);
	else if (hi == 3) return Color( p, q, v, 255);
	else if (hi == 4) return Color( t, p, v, 255);
	else return Color( v, p, q, 255);
}

static Color FromHSB(float hue, float saturation, float brightness)
{
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1)
	{
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 2)
	{
		return Color(
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 3)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255)
		);
	}
	else if (h < 4)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else if (h < 5)
	{
		return Color(
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else
	{
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255)
		);
	}
}