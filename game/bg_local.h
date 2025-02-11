// Copyright (C) 1999-2000 Id Software, Inc.
//
// bg_local.h -- local definitions for the bg (both games) files

#define MIN_WALK_NORMAL 0.7f // can't walk on very steep slopes

#define TIMER_LAND 130
#define TIMER_GESTURE (34 * 66 + 50)

#define OVERCLIP 1.001f

// all of the locals will be zeroed before each
// pmove, just to make damn sure we don't have
// any differences when running on client or server
typedef struct {
    vector3 forward, right, up;
    float frametime;

    int msec;

    qboolean walking;
    qboolean groundPlane;
    trace_t groundTrace;

    float impactSpeed;

    vector3 previous_origin;
    vector3 previous_velocity;
    int previous_waterlevel;
} pml_t;

extern pml_t pml;

// movement parameters
extern float pm_stopspeed;
extern float pm_duckScale;
extern float pm_swimScale;
extern float pm_wadeScale;

extern float pm_accelerate;
extern float pm_airaccelerate;
extern float pm_wateraccelerate;
extern float pm_flyaccelerate;

extern float pm_friction;
extern float pm_waterfriction;
extern float pm_flightfriction;

#ifdef _DEBUG
extern int c_pmove;
#endif // _DEBUG

extern int forcePowerNeeded[NUM_FORCE_POWER_LEVELS][NUM_FORCE_POWERS];

// PM anim utility functions:
qboolean PM_SaberInParry(int move);
qboolean PM_SaberInKnockaway(int move);
qboolean PM_SaberInReflect(int move);
qboolean PM_SaberInStart(int move);
qboolean PM_InSaberAnim(int anim);
qboolean PM_InKnockDown(playerState_t *ps);
qboolean PM_PainAnim(int anim);
qboolean PM_JumpingAnim(int anim);
qboolean PM_LandingAnim(int anim);
qboolean PM_SpinningAnim(int anim);
qboolean PM_InOnGroundAnim(int anim);
qboolean PM_InRollComplete(playerState_t *ps, int anim);

int PM_AnimLength(int index, animNumber_e anim);

int PM_GetSaberStance(void);
float PM_GroundDistance(void);
qboolean PM_SomeoneInFront(trace_t *tr);
saberMoveName_e PM_SaberFlipOverAttackMove(void);
saberMoveName_e PM_SaberJumpAttackMove(void);

void PM_ClipVelocity(vector3 *in, vector3 *normal, vector3 *out, float overbounce);
void PM_AddTouchEnt(int entityNum);
void PM_AddEvent(int newEvent);

qboolean PM_SlideMove(qboolean gravity);
void PM_StepSlideMove(qboolean gravity);

void PM_StartTorsoAnim(int anim);
void PM_ContinueLegsAnim(int anim);
void PM_ForceLegsAnim(int anim);

void PM_BeginWeaponChange(int weapon);
void PM_FinishWeaponChange(void);

void PM_SetAnim(int setAnimParts, int anim, uint32_t setAnimFlags, int blendTime);

void PM_WeaponLightsaber(void);
void PM_SetSaberMove(short newMove);

void PM_SetForceJumpZStart(float value);

void BG_CycleForce(playerState_t *ps, int direction);
void BG_CycleInven(playerState_t *ps, int direction);

qboolean GetCInfo(uint32_t bit);
qboolean GetCPD(bgEntity_t *self, uint32_t bit);
