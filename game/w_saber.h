#define ARMOR_EFFECT_TIME 500

// saberEventFlags
#define SEF_HITENEMY (0x0001u)  // Hit the enemy
#define SEF_HITOBJECT (0x0002u) // Hit some other object
#define SEF_HITWALL (0x0004u)   // Hit a wall
#define SEF_PARRIED (0x0008u)   // Parried a saber swipe
#define SEF_DEFLECTED (0x0010u) // Deflected a missile or saberInFlight
#define SEF_BLOCKED (0x0020u)   // Was blocked by a parry
#define SEF_EVENTS (SEF_HITENEMY | SEF_HITOBJECT | SEF_HITWALL | SEF_PARRIED | SEF_DEFLECTED | SEF_BLOCKED)
#define SEF_LOCKED (0x0040u)   // Sabers locked with someone else
#define SEF_INWATER (0x0080u)  // Saber is in water
#define SEF_LOCK_WON (0x0100u) // Won a saberLock

// saberEntityState
#define SES_LEAVING 1
#define SES_HOVERING 1  // 2
#define SES_RETURNING 1 // 3
// This is a hack because ATM the saberEntityState is only non-0 if out or 0 if in, and we
// at least want NPCs knowing when their saber is out regardless.

#define JSF_AMBUSH 16 // ambusher Jedi

#define SABER_RADIUS_STANDARD 3.0f
#define SABER_REFLECT_MISSILE_CONE 0.2f

#define MAX_GRIP_DISTANCE 256
#define MAX_TRICK_DISTANCE 512
#define FORCE_JUMP_CHARGE_TIME 6400 // 3000.0f
#define GRIP_DRAIN_AMOUNT 30
#define FORCE_LIGHTNING_RADIUS 300
#define MAX_DRAIN_DISTANCE 512

enum forceJump_e { FJ_FORWARD, FJ_BACKWARD, FJ_RIGHT, FJ_LEFT, FJ_UP };

enum evasionType_e {
    EVASION_NONE = 0,
    EVASION_PARRY,
    EVASION_DUCK_PARRY,
    EVASION_JUMP_PARRY,
    EVASION_DODGE,
    EVASION_JUMP,
    EVASION_DUCK,
    EVASION_FJUMP,
    EVASION_CARTWHEEL,
    EVASION_OTHER,
    NUM_EVASION_TYPES
};

#define SABERMINS_X -3.0f //-24.0f
#define SABERMINS_Y -3.0f //-24.0f
#define SABERMINS_Z -3.0f //-8.0f
#define SABERMAXS_X 3.0f  // 24.0f
#define SABERMAXS_Y 3.0f  // 24.0f
#define SABERMAXS_Z 3.0f  // 8.0f
#define SABER_MIN_THROW_DIST 80.0f

extern float forceJumpHeight[NUM_FORCE_POWER_LEVELS];
extern float forceJumpStrength[NUM_FORCE_POWER_LEVELS];
