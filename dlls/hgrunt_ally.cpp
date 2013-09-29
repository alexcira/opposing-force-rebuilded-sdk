/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// Human Grunt Ally 
//=========================================================

#include	"extdll.h"
#include	"plane.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"animation.h"
#include	"squadmonster.h"
#include	"weapons.h"
#include	"talkmonster.h"
#include	"soundent.h"
#include	"effects.h"
#include	"customentity.h"

//=========================================================
// monster-specific DEFINE's
//=========================================================
#define HGRUNT_ALLY_M4A1				( 1 << 0)
#define HGRUNT_ALLY_HANDGRENADE			( 1 << 1)
#define HGRUNT_ALLY_GRENADELAUNCHER		( 1 << 2)
#define HGRUNT_ALLY_SHOTGUN				( 1 << 3)
#define HGRUNT_ALLY_M249				( 1 << 4)

#define HGRUNT_NUM_HEADS				8

// Head group
#define ALLY_HEAD_GROUP					1
#define ALLY_HEAD_MASK					0
#define ALLY_HEAD_BERET					1
#define ALLY_HEAD_SHOTGUN				2
#define ALLY_HEAD_SAW					3
#define ALLY_HEAD_SAW_BLACK				4
#define ALLY_HEAD_MP					5
#define ALLY_HEAD_MAJOR					6
#define ALLY_HEAD_BERET_BLACK			7

// Torso group for weapons
#define	ALLY_TORSO_GROUP				2
#define ALLY_TORSO_DEFAULT				0
#define ALLY_TORSO_M249					1
#define ALLY_TORSO_FLAT					2
#define ALLY_TORSO_SHOTGUN				3

// Weapon group
#define ALLY_GUN_GROUP					3
#define ALLY_GUN_MP5					0
#define ALLY_GUN_SHOTGUN				1
#define ALLY_GUN_SAW					2
#define ALLY_GUN_NONE					3

//=========================================================
// Dead Human Grunt Ally PROP
//=========================================================
class CDeadHGruntAlly : public CBaseMonster
{
public:
	void Spawn( void );
	void Precache( void );

	int	Classify ( void ) { return CLASS_PLAYER_ALLY; }
	void KeyValue( KeyValueData *pkvd );

	int	m_iHead;
	int	m_iPose;
	static char *m_szPoses[8];
};

char *CDeadHGruntAlly::m_szPoses[] = { "dead_on_stomach", "deadstomach", "deadside", "deadsitting", "dead_on_back", "dead_headcrabed", "hgrunt_dead_stomach", "dead_canyon" };
void CDeadHGruntAlly::KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "pose"))
	{
		m_iPose = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "head"))
	{
		m_iHead = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else 
		CBaseMonster::KeyValue( pkvd );
}

LINK_ENTITY_TO_CLASS( monster_human_grunt_ally_dead, CDeadHGruntAlly );

// ********** Dead Human Grunt Ally SPAWN **********
void CDeadHGruntAlly :: Spawn( void )
{
	Precache();
	SET_MODEL(ENT(pev), "models/hgrunt_opfor.mdl");

	pev->effects		= 0;
	pev->yaw_speed		= 8;
	pev->sequence		= 0;
	pev->health			= 8; // Corpses have less health
	m_bloodColor		= BLOOD_COLOR_RED;

	pev->sequence = LookupSequence( m_szPoses[m_iPose] );
	if (pev->sequence == -1)
		ALERT ( at_console, "Dead Human Grunt Ally with bad pose\n" );

	// Weapons & Torso
	switch( pev->weapons )
	{
		case HGRUNT_ALLY_SHOTGUN: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_MASK ); break;
		case HGRUNT_ALLY_M4A1: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_BERET ); break;
		case HGRUNT_ALLY_M249: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_SHOTGUN ); break;
		default: SetBodygroup( ALLY_GUN_GROUP, ALLY_GUN_NONE ); break;
	}

	// Heads
	switch( m_iHead )
	{
		default:
		case 0: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_MASK ); break;
		case 1: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_BERET ); break;
		case 2: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_SHOTGUN ); break;
		case 3: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_SAW ); break;
		case 4: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_SAW_BLACK ); break;
		case 5: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_MP ); break;
		case 6: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_MAJOR ); break;
		case 7: SetBodygroup( ALLY_HEAD_GROUP, ALLY_HEAD_BERET_BLACK ); break;
	}

	MonsterInitDead();
}

void CDeadHGruntAlly :: Precache( void )
{
	PRECACHE_MODEL("models/hgrunt_opfor.mdl");
}