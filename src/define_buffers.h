/*  Copyright 2013 Alexis Herault, Giuseppe Bilotta, Robert A. Dalrymple, Eugenio Rustico, Ciro Del Negro

    Istituto Nazionale di Geofisica e Vulcanologia
        Sezione di Catania, Catania, Italy

    Università di Catania, Catania, Italy

    Johns Hopkins University, Baltimore, MD

    This file is part of GPUSPH.

    GPUSPH is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GPUSPH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GPUSPH.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Define one flag for each buffer which is used in a worker */

#ifndef DEFINED_BUFFERS
#define DEFINED_BUFFERS

// sanity check
#ifndef FIRST_DEFINED_BUFFER
#error "define_buffers.h was included without specifying starting point"
#endif

#include "common_types.h"

#ifndef SET_BUFFER_TRAITS
#include "buffer_traits.h"
#endif


// start from FIRST_DEFINED_BUFFER
// double-precision position buffer (used on host only)
#define BUFFER_POS_GLOBAL	FIRST_DEFINED_BUFFER
SET_BUFFER_TRAITS(BUFFER_POS_GLOBAL, double4, 1, "Position (double precision)");

#define BUFFER_POS			(BUFFER_POS_GLOBAL << 1)
SET_BUFFER_TRAITS(BUFFER_POS, float4, 2, "Position");
#define BUFFER_VEL			(BUFFER_POS << 1)
SET_BUFFER_TRAITS(BUFFER_VEL, float4, 2, "Velocity");
#define BUFFER_INFO			(BUFFER_VEL << 1)
SET_BUFFER_TRAITS(BUFFER_INFO, particleinfo, 2, "Info");
#define BUFFER_HASH			(BUFFER_INFO << 1)
SET_BUFFER_TRAITS(BUFFER_HASH, hashKey, 1, "Hash");

#define BUFFER_PARTINDEX	(BUFFER_HASH << 1)
SET_BUFFER_TRAITS(BUFFER_PARTINDEX, uint, 1, "Particle Index");
#define BUFFER_INVINDEX		(BUFFER_PARTINDEX << 1)
SET_BUFFER_TRAITS(BUFFER_INVINDEX, uint, 1, "Inverse Particle Index");

// not used for the time being. evaluate if they should be migrated to the buffer mechanism
// too or not
#define BUFFER_CELLSTART	(BUFFER_INVINDEX << 1)
SET_BUFFER_TRAITS(BUFFER_CELLSTART, uint, 1, "Cell Start");
#define BUFFER_CELLEND		(BUFFER_CELLSTART << 1)
SET_BUFFER_TRAITS(BUFFER_CELLEND, uint, 1, "Cell End");

#define BUFFER_NEIBSLIST	(BUFFER_CELLEND << 1)
SET_BUFFER_TRAITS(BUFFER_NEIBSLIST, neibdata, 1, "Neighbor List");

#define BUFFER_FORCES		(BUFFER_NEIBSLIST << 1)
SET_BUFFER_TRAITS(BUFFER_FORCES, float4, 1, "Force");

#define BUFFER_XSPH			(BUFFER_FORCES << 1)
SET_BUFFER_TRAITS(BUFFER_XSPH, float4, 1, "XSPH");

#define BUFFER_TAU			(BUFFER_XSPH << 1)
SET_BUFFER_TRAITS(BUFFER_TAU, float2, 3, "Tau");

#define BUFFER_VORTICITY	(BUFFER_TAU << 1)
SET_BUFFER_TRAITS(BUFFER_VORTICITY, float3, 1, "Vorticity");
#define BUFFER_NORMALS		(BUFFER_VORTICITY << 1)
SET_BUFFER_TRAITS(BUFFER_NORMALS, float4, 1, "Normals");

#define BUFFER_BOUNDELEMENTS	(BUFFER_NORMALS << 1)
SET_BUFFER_TRAITS(BUFFER_BOUNDELEMENTS, float4, 2, "Boundary Elements");
#define BUFFER_GRADGAMMA		(BUFFER_BOUNDELEMENTS << 1)
SET_BUFFER_TRAITS(BUFFER_GRADGAMMA, float4, 2, "Gamma Gradient");
#define BUFFER_VERTICES			(BUFFER_GRADGAMMA << 1)
SET_BUFFER_TRAITS(BUFFER_VERTICES, vertexinfo, 2, "Vertices");
#define BUFFER_VERTPOS			(BUFFER_VERTICES << 1)
SET_BUFFER_TRAITS(BUFFER_VERTPOS, float2, 3, "Vertex positions relative to s");

#define BUFFER_TKE			(BUFFER_VERTPOS << 1)
SET_BUFFER_TRAITS(BUFFER_TKE, float, 2, "Turbulent Kinetic Energy [k]");
#define BUFFER_EPSILON		(BUFFER_TKE << 1)
SET_BUFFER_TRAITS(BUFFER_EPSILON, float, 2, "Turbulent Dissipation Rate [e]");
#define BUFFER_TURBVISC		(BUFFER_EPSILON << 1)
SET_BUFFER_TRAITS(BUFFER_TURBVISC, float, 2, "Eddy Viscosity");
#define BUFFER_DKDE			(BUFFER_TURBVISC << 1)
SET_BUFFER_TRAITS(BUFFER_DKDE, float2, 1, "[k]-[e] derivatives");

#define BUFFER_CFL			(BUFFER_DKDE << 1)
SET_BUFFER_TRAITS(BUFFER_CFL, float, 1, "CFL array");
#define BUFFER_CFL_TEMP		(BUFFER_CFL << 1)
SET_BUFFER_TRAITS(BUFFER_CFL_TEMP, float, 1, "CFL aux array");
#define BUFFER_CFL_KEPS		(BUFFER_CFL_TEMP << 1)
SET_BUFFER_TRAITS(BUFFER_CFL_KEPS, float, 1, "Turbulent Viscosity CFL array");

#define BUFFER_PRIVATE		(BUFFER_CFL_KEPS << 1)
SET_BUFFER_TRAITS(BUFFER_PRIVATE, float, 1, "Private scalar");

// last defined buffer. if new buffers are defined, remember to update this
#define LAST_DEFINED_BUFFER	BUFFER_PRIVATE

// common shortcut
#define BUFFERS_POS_VEL_INFO	(BUFFER_POS | BUFFER_VEL | BUFFER_INFO)

// all CFL buffers
#define BUFFERS_CFL			( BUFFER_CFL | BUFFER_CFL_TEMP | BUFFER_CFL_KEPS )

// all CELL buffers
#define BUFFERS_CELL		( BUFFER_CELLSTART | BUFFER_CELLEND )

// elegant way to set to 1 all bits in between the first and the last buffers
// NOTE: READ or WRITE specification must be added for double buffers
#define ALL_DEFINED_BUFFERS		((FIRST_DEFINED_BUFFER-1) ^ (LAST_DEFINED_BUFFER-1) | LAST_DEFINED_BUFFER )

// all particle-based buffers
#define ALL_PARTICLE_BUFFERS	(ALL_DEFINED_BUFFERS & ~(BUFFERS_CFL | BUFFERS_CELL | BUFFER_NEIBSLIST))

// particle-based buffers to be imported during the APPEND_EXTERNAL command
#define IMPORT_BUFFERS			(BUFFER_POS | BUFFER_HASH | BUFFER_VEL | BUFFER_INFO | BUFFER_VERTPOS | DBLBUFFER_READ)

// all double buffers TODO some template metaprogramming would help here
#define BUFFERS_ALL_DBL		(BUFFER_POS | BUFFER_VEL | BUFFER_INFO | \
	BUFFER_BOUNDELEMENTS | BUFFER_GRADGAMMA | BUFFER_VERTICES | \
	BUFFER_TKE | BUFFER_EPSILON | \
	BUFFER_TURBVISC)

// all buffers which need to transfer more than one array
#define BUFFER_BIG		(BUFFER_TAU | BUFFER_VERTPOS)

#endif

