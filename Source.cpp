/* MADE BY NBQ 9/24/2019 */


#include "structs.h"
#include "ct_models.h"
#include "t_models.h"
#include <iostream>

bool TargetCT = false; // for toggling between targeting CT and T players
HANDLE hEntry = NULL;

// function that converts hex color to its rgb equivalent
stRGB HexToRgb(int hex) 
{
	stRGB rgb;
	rgb.r = (hex >> 16);
	rgb.g = (hex & 0x00ff00) >> 8;
	rgb.b = (hex & 0x0000ff);
	return rgb;
}

// function that converts rgb color to hsl color
// (important because rgb color might vary due to brightness)
// (thats why we use hsl (hue, saturation, luminescence) color with certain tolerance for more accurate bot)
stHSL RgbToHsl(stRGB rgb)
{
	stHSL hsl;

	float r = rgb.r / 255.0f;
	float g = rgb.g / 255.0f;
	float b = rgb.b / 255.0f;

	float h, s, v;

	float max = tmax(r, g, b);
	float min = tmin(r, g, b);

	v = max;

	if (max == 0.0f)
	{
		s = 0;
		h = 0;
	}
	else if (max - min == 0.0f)
	{
		s = 0;
		h = 0;
	}
	else
	{
		s = (max - min) / max;

		if (max == r)
		{
			h = 60 * ((g - b) / (max - min)) + 0;
		}
		else if (max == g)
		{
			h = 60 * ((b - r) / (max - min)) + 120;
		}
		else
		{
			h = 60 * ((r - g) / (max - min)) + 240;
		}
	}

	if (h < 0)
	{
		h += 360.0f;
	}
	hsl.h = h;
	hsl.s = s * 100;
	hsl.l = v * 100;
	return hsl;
}

// function for converting all the hex colors from t_models.h and ct_models.h to hsl values
bool LoadHsl(pplayer_model ct_model, pplayer_model t_model)
{
	for (int i = 0; i < NUM_MODEL_COLORS; i++) // filing ct_model
	{
		stRGB rgb = HexToRgb(ct_model_colors[i]);
		ct_model->hslModel[i] = RgbToHsl(rgb);
	}
	for (int i = 0; i < NUM_MODEL_COLORS; i++) // filing t_model
	{
		stRGB rgb = HexToRgb(t_model_colors[i]);
		t_model->hslModel[i] = RgbToHsl(rgb);
	}
	return true;
}

// function for loading our cfg
bool LoadConfig(pconfig cfg)
{
	stHSL tol;
	tol.h = 2;
	tol.s = 3;
	tol.l = 10;
	cfg->key = VK_XBUTTON1;
	cfg->switchtargetkey = VK_F6;
	cfg->closekey = VK_F9;
	cfg->tolerance = tol;
	cfg->fov = 3;
	cfg->delay = 0;
	cfg->burst = 100;
	cfg->pause = 1;
	return true;
}

// comparison / shoot function
void Chromebot(stHSL pxColor, stHSL modelColor, config cfg)
{
	if (GetAsyncKeyState(cfg.key))
	{
		if ((abs(pxColor.h - modelColor.h) < cfg.tolerance.h) &&
			(abs(pxColor.s - modelColor.s) < cfg.tolerance.s) &&
			(abs(pxColor.l - modelColor.l) < cfg.tolerance.l))
		{
			Sleep(cfg.delay);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(cfg.burst);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(cfg.pause);
		}
	}
}

bool ThreadFn()
{
	player_model ct_model;
	player_model t_model;
	config cfg;
	if (!LoadHsl(&ct_model, &t_model))
	{
		return false;
	}

	if (!LoadConfig(&cfg))
	{
		return false;
	}
	HDC hdc = GetDC(HWND_DESKTOP);
	POINT p;

	while (!GetAsyncKeyState(cfg.closekey))
	{
		if (GetAsyncKeyState(cfg.switchtargetkey))
		{
			TargetCT = !TargetCT;
			Sleep(100);
			Beep(400, 100);
		}
		stRGB pxRgb;
		GetCursorPos(&p);
		COLORREF pxCref = GetPixel(hdc, p.x + cfg.fov, p.y + cfg.fov);
		pxRgb.r = GetRValue(pxCref);
		pxRgb.g = GetGValue(pxCref);
		pxRgb.b = GetBValue(pxCref);
		stHSL pxHsl = RgbToHsl(pxRgb);
		for (int i = 0; i < NUM_MODEL_COLORS; i++)
		{
			if (TargetCT)
			{
				Chromebot(pxHsl, ct_model.hslModel[i], cfg);
			}
			else
			{
				Chromebot(pxHsl, t_model.hslModel[i], cfg);
			}
		}
		Sleep(1);
	}
	return true;
}

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		Beep(400, 400);
		hEntry = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadFn, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		WaitForSingleObject(hEntry, INFINITE);
		CloseHandle(hEntry);
		break;
	}
	return TRUE;
}