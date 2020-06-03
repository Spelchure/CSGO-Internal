/*****************************************************************//**
 * \file   TraceRay.hpp
 * \brief  
 * 
 * 
 * \author ALPEREN
 * \date   June 2020
 * 
 *  <br>
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/
#pragma once
#include "Vectors.hpp"
#include <cstdint>
#include <cfloat>
typedef
struct 
{
    Vector normal;
    float dist;
    uint8_t type;   // for fast side tests
    uint8_t signbits;  // signx + (signy<<1) + (signz<<1)
    uint8_t pad[2];
}cplane_t;

typedef
struct 
{
    char* name; //0x0000
    int16_t surface_prop; //0x0004
    uint16_t flags; //0x0006
}surface_t;

struct Ray_t
{
    VectorAligned  m_Start;  // starting point, centered within the extents
    VectorAligned  m_Delta;  // direction + length of the ray
    VectorAligned  m_StartOffset; // Add this to m_Start to Get the actual ray start
    VectorAligned  m_Extents;     // Describes an axis aligned box extruded along a ray
    const void* m_pWorldAxisTransform;
    bool m_IsRay;  // are the extents zero?
    bool m_IsSwept;     // is delta != 0?

    Ray_t() : m_pWorldAxisTransform(NULL) {}

    void Init(Vector const& start, Vector const& end)
    {
        m_Delta = end - start;

        m_IsSwept = (m_Delta.LengthSqr() != 0);

        m_Extents.Init();   

        m_pWorldAxisTransform = NULL;
        m_IsRay = true;

        // Offset m_Start to be in the center of the box...
        m_StartOffset.Init();
        m_Start = start;
    }

    void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
    {
        m_Delta = end - start;

        m_pWorldAxisTransform = NULL;
        m_IsSwept = (m_Delta.LengthSqr() != 0);

        m_Extents = maxs - mins;
        m_Extents *= 0.5f;
        m_IsRay = (m_Extents.LengthSqr() < 1e-6);

        // Offset m_Start to be in the center of the box...
        m_StartOffset = maxs + mins;
        m_StartOffset *= 0.5f;
        m_Start = start + m_StartOffset;
        m_StartOffset *= -1.0f;
    }

    Vector InvDelta() const
    {
        Vector vecInvDelta;
        for (int iAxis = 0; iAxis < 3; ++iAxis) {
            if (m_Delta[iAxis] != 0.0f) {
                vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
            }
            else {
                vecInvDelta[iAxis] = FLT_MAX;
            }
        }
        return vecInvDelta;
    }
};

enum class TraceType
{
    TRACE_EVERYTHING = 0,
    TRACE_WORLD_ONLY,
    TRACE_ENTITIES_ONLY,
    TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
    virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
    virtual TraceType GetTraceType() const = 0;
};


// This is the one most normal traces will inherit from
class CTraceFilter : public ITraceFilter
{
public:
    bool ShouldHitEntity(void* pEntityHandle, int /*contentsMask*/)
    {
        return !(pEntityHandle == pSkip);
    }
    virtual TraceType GetTraceType() const
    {
        return TraceType::TRACE_EVERYTHING;
    }
    void* pSkip;
};

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define CONTENTS_MOVEABLE		0x4000
#define	CONTENTS_MONSTER		0x2000000
#define	CONTENTS_MONSTERCLIP	0x20000
#define	CONTENTS_DEBRIS			0x4000000
#define CONTENTS_HITBOX			0x40000000

#define MASK_NPCWORLDSTATIC	    ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE )
#define STANDARD_TRACE_MASK     ( MASK_NPCWORLDSTATIC | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_HITBOX )
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

class CBaseTrace
{
public:
    bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
    bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
    bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
    bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
    bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

    // these members are aligned!!
    Vector          startpos;            // start position
    Vector         endpos;              // final position
    cplane_t       plane;               // surface normal at impact

    float          fraction;            // time completed, 1.0 = didn't hit anything

    int            contents;            // contents on other side of surface hit
    unsigned short dispFlags;           // displacement flags for marking surfaces with data

    bool           allsolid;            // if true, plane is not valid
    bool           startsolid;          // if true, the initial point was in a solid area

    CBaseTrace() {}

};

class CGameTrace : public CBaseTrace
{
public:
    bool DidHitWorld() const;
    bool DidHitNonWorldEntity() const;
    int GetEntityIndex() const;
    bool DidHit() const;
    bool IsVisible() const;

public:

    float               fractionleftsolid;  // time we left a solid, only valid if we started in solid
    surface_t          surface;            // surface hit (impact surface)
    int                 hitgroup;           // 0 == generic, non-zero is specific body part
    short               physicsbone;        // physics bone hit by trace in studio
    unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable
    //IClientEntity* hit_entity;
    void* hit_entity;
    int                 hitbox;                       // box hit by trace in studio

    CGameTrace() {}
private:
    // No copy constructors allowed
    CGameTrace(const CGameTrace& other) :
        fractionleftsolid(other.fractionleftsolid),
        surface(other.surface),
        hitgroup(other.hitgroup),
        physicsbone(other.physicsbone),
        worldSurfaceIndex(other.worldSurfaceIndex),
        hit_entity(other.hit_entity),
        hitbox(other.hitbox)
    {
        startpos = other.startpos;
        endpos = other.endpos;
        plane = other.plane;
        fraction = other.fraction;
        contents = other.contents;
        dispFlags = other.dispFlags;
        allsolid = other.allsolid;
        startsolid = other.startsolid;
    }

    CGameTrace& operator=(const CGameTrace& other)
    {
        startpos = other.startpos;
        endpos = other.endpos;
        plane = other.plane;
        fraction = other.fraction;
        contents = other.contents;
        dispFlags = other.dispFlags;
        allsolid = other.allsolid;
        startsolid = other.startsolid;
        fractionleftsolid = other.fractionleftsolid;
        surface = other.surface;
        hitgroup = other.hitgroup;
        physicsbone = other.physicsbone;
        worldSurfaceIndex = other.worldSurfaceIndex;
        hit_entity = other.hit_entity;
        hitbox = other.hitbox;
        return *this;
    }
};

inline bool CGameTrace::DidHit() const
{
    return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
    return fraction > 0.97f;
}

class IEngineTrace
{
public:
    virtual void  fn0() = 0;
    virtual void  fn1() = 0;
    virtual void  fn2() = 0;
    virtual void  fn3() = 0;
    virtual void  fn4() = 0;
    virtual void  TraceRay(Ray_t& ray, unsigned int fMask, CTraceFilter* pTraceFilter, CGameTrace* pTrace) = 0;
};

typedef void(__fastcall* tTraceRay)(void* thisptr, void* not_edx, Ray_t& ray, unsigned int fMask, CTraceFilter* pTraceFilter, CGameTrace* pTrace);
