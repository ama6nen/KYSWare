#pragma once

#include "../Math/VMatrix.hpp"
class IPanel;
class C_BaseEntity;

enum class ClearFlags_t
{
    VIEW_CLEAR_COLOR = 0x1,
    VIEW_CLEAR_DEPTH = 0x2,
    VIEW_CLEAR_FULL_TARGET = 0x4,
    VIEW_NO_DRAW = 0x8,
    VIEW_CLEAR_OBEY_STENCIL = 0x10,
    VIEW_CLEAR_STENCIL = 0x20,
};


enum class MotionBlurMode_t
{
    MOTION_BLUR_DISABLE = 1,
    MOTION_BLUR_GAME = 2,
    MOTION_BLUR_SFM = 3
};

struct dworldlight_t
{
	Vector origin; //0x0000 
	Vector intensity; //0x000C 
	Vector normal; //0x0018 
	int cluster; //0x0024 
	int type; //0x0028 
	int style; //0x002C 
	float stopdot; //0x0030 
	float stopdot2; //0x0034 
	float exponent; //0x0038 
	float radius; //0x003C 
	float constant_attn; //0x0040 
	float linear_attn; //0x0044 
	float quadratic_attn; //0x0048 
	int flags; //0x004C 
	int texinfo; //0x0050 
	int owner; //0x0054 
	char pad_0x0058[0xC]; //0x0058
};
class CHostState
{
public:
	char pad_0x0000[0xA8]; //0x0000
	int numworldlights;
	dworldlight_t *worldlights;
};
class CViewSetup
{
public:
	int			x, x_old;
	int			y, y_old;
	int			width, width_old;
	int			height, height_old;
	bool		m_bOrtho;
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;
	bool		m_bCustomViewMatrix;
	matrix3x4_t	m_matCustomViewMatrix;
	char		pad_0x68[0x48];
	float		fov;
	float		fovViewmodel;
	Vector		origin;
	Vector		angles;
	float		zNear;
	float		zFar;
	float		zNearViewmodel;
	float		zFarViewmodel;
	float		m_flAspectRatio;
	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	int			m_nDoFQuality;
	MotionBlurMode_t			m_nMotionBlurMode;
	float		m_flShutterTime;
	Vector		m_vShutterOpenPosition;
	Vector		m_shutterOpenAngles;
	Vector		m_vShutterClosePosition;
	Vector		m_shutterCloseAngles;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	int			m_iEdgeBlur;
};//Size=0x014C//Size=0x014C


class CBaseViewport;
class CBaseHudWeaponSelection;
typedef unsigned long HCursor;


//https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Printing_text_to_chat

enum ChatFilters
{
	CHAT_FILTER_NONE = 0,
	CHAT_FILTER_JOINLEAVE = 0x000001,
	CHAT_FILTER_NAMECHANGE = 0x000002,
	CHAT_FILTER_PUBLICCHAT = 0x000004,
	CHAT_FILTER_SERVERMSG = 0x000008,
	CHAT_FILTER_TEAMCHANGE = 0x000010,
	CHAT_FILTER_ACHIEVEMENT = 0x000020,
};


class CHudChat
{
public:

	
	void toggle_feature(bool on, const char* fmt)
	{
		on ? this->print("%s \x04on", fmt) : this->print("%s \x02off", fmt);
	}

	//auto adds [KYSWare] cuz LAZY
	void print(const char* fmt, ...)
	{
		if (!this) return;


		std::string kysware(" ""\x07""[KYSWare] ""\x01""");
		kysware += fmt;

		const char* fmt2 = kysware.c_str();
		
		char buf[1024];
		va_list va;

		va_start(va, fmt);
		_vsnprintf_s(buf, 1024, fmt2, va);
		va_end(va);

	
		CallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 26)(this, 0, 0, buf);

	}

	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		if (!this) return;

		CallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 26)(this, iPlayerIndex, iFilter, fmt);
	}

	struct CHudCol
	{
	public:
		const char* white = """\x01""";
		const char* red = """\x01""";
		const char* purple = """\x02""";
		const char* green = """\x04""";
		const char* light_green = """\x05""";
		const char* lime = """\x06""";
		const char* red_flatui = """\x07""";
		const char* grey = """\x08""";
		const char* yellowish = """\x09""";
		const char* light_grey = """\x0A""";
		const char* light_blue = """\x0B""";
		const char* blue = """\x0C""";
		const char* white_grey = """\x0D""";
		const char* magenta = """\x0E""";
		const char* red_flatui2 = """\x0F""";
		const char* golden = """\x10""";

	} CHudColors;
};
class IClientMode
{
public:
    virtual             ~IClientMode() {}
    virtual int         ClientModeCSNormal(void *) = 0;
    virtual void        Init() = 0;
    virtual void        InitViewport() = 0;
    virtual void        Shutdown() = 0;
    virtual void        Enable() = 0;
    virtual void        Disable() = 0;
    virtual void        Layout() = 0;
    virtual IPanel*     GetViewport() = 0;
    virtual void*       GetViewportAnimationController() = 0;
    virtual void        ProcessInput(bool bActive) = 0;
    virtual bool        ShouldDrawDetailObjects() = 0;
    virtual bool        ShouldDrawEntity(C_BaseEntity *pEnt) = 0;
    virtual bool        ShouldDrawLocalPlayer(C_BaseEntity *pPlayer) = 0;
    virtual bool        ShouldDrawParticles() = 0;
    virtual bool        ShouldDrawFog(void) = 0;
    virtual void        OverrideView(CViewSetup *pSetup) = 0;
    virtual int         KeyInput(int down, int keynum, const char *pszCurrentBinding) = 0;
    virtual void        StartMessageMode(int iMessageModeType) = 0;
    virtual IPanel*     GetMessagePanel() = 0;
    virtual void        OverrideMouseInput(float *x, float *y) = 0;
    virtual bool        CreateMove(float flInputSampleTime, void* usercmd) = 0;
    virtual void        LevelInit(const char *newmap) = 0;
    virtual void        LevelShutdown(void) = 0;

public:
	void* ptr1;
	void* ptr2;
	CBaseViewport			*m_pViewport;
	// Message mode handling
	// All modes share a common chat interface
	CHudChat			*m_pChatElement;
	HCursor			m_CursorNone;
	CBaseHudWeaponSelection *m_pWeaponSelection;
	int						m_nRootSize[2];
};