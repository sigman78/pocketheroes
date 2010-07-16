/*
 * This file is a part of Pocket Heroes Game project
 * 	http://www.pocketheroes.net
 *	https://code.google.com/p/pocketheroes/
 *
 * Copyright 2004-2010 by Robert Tarasov and Anton Stuk (iO UPG)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 


#include "stdafx.h"

// Structures:::
// world	- former passMap&marksMap, caches cell move cost, stores visiting direction and cell status
// nodes	- former (visited&open) storage for 'open' cell positions and their respective h & g values
//		the difference what instead of storing h we store h(heuristic) + g since this value is used for sorting
// heap		- indices of nodes in sort order. indirection is used to reduce data transfers upon sorting
//		in principle we can immideately sort the nodes array, thus getting rid of this structure altogether
//
// algorithm follows:
//		-- initialization
//		PUSH START POINT AS THE FIRST CELL INTO THE HEAP
//
//		-- searching cycle
//		PICK CURRENT BEST CELL FROM HEAP
//		closed_node = parentIdx = getHeapRoot
//		parentNode  = &nodes[bestFnode]
//		if (parentNode.yx == endyx ) WE ARE DONE;
//		CLOSE CELL
//		world[ parentNode.yx ].state = CLOSED
//		RemoveHeapRoot
//		PROCESS OPEN NODES
//		foreach( dir in EightDirections )
//			pos		= parentNode.yx + offset[ dir ]
//			mapCell	= world[ pos ]
//			NEVER VISITED SO FAR
//			if ( mapCell.state == UNKNOWN ) 
//				mapCell.state = OPEN
//				mapCell.dir = dir
//				STORE NEW NODE 
//				newNode = &nodes[ ++lastNode ]
//				newNode.yx = yx
//				CALCULATE METRICS AND HEURISTCS
//				SUMM OF ALL g FACTORS SO FAR
//				newNode.g = parentNode.g + ( mapCell.cost * dirCostMultiplier[dir] )
//				SUMM OF g AND HEURISTIC 
//				newNode.f = newNode.g + Distance( yx )
//				PUSH IT INTO THE HEAP
//				heapInsert( newNode ) (actuall lastNode since we operate on indices )
//			else mapCell == Open && newG < mapCell.g
//				UPDATE WITH THE NEW BETTER PATH
//				...
//		if ( HeapIsEmpty() ) THERE IS NO PATH EXISTS
//
//
// Further Notes:
//
// 1) Requires extensive preallocation, essentially Nodes[MAP_SIZE], Heap[Max_HEAP], and WORLD[MAP_SIZE]
// 2) Requires replication of the move cost values from PassMap to the WORLD (for better cache coherence)
//	  could be avoided by the price of double lookup of moving cost value
// 3) G & F values are subject to overflow (since the using uint16 for storage) - need to check
// 4) Nodes array grows up to the size of map (in special cases) since we do not remove closed nodes
//	  could be avoided by the cost of additional memmove per checked point
//	  OR (check that assumption) erase is possible by exchanging last node with the removed one, 
//	  that require exchanging indexes in the heap though --
//    (In other words we need to track index in heap of last added opened element)
// 5) In this algorithm every map point is considired only once without checking whether we could have better proposal
// Additional improvements:
// 1) Hash or similar structure to speedup membership test on open queue (it happens only once in a while)
// 2) Store 'f' values in the heap inplace - it might speedup search by getting rid of indirection
// 3) Replace binary min-heap with the HOT heap (helps in extremily large paths)
// ___Newly discovered____
// 4) Use 'open parent directions lists' to reject unreachable destinations asap
//    i.e. setup 8 types of edge 'direction lists' (constant) and 4 for entry&finish points
//	  (if they has custom dir masks). Select correct 'directions list' by parent cell coordinate
//	  To get rid of selection logic we also can precalculate 'direcionts list' index into 
//    world ( mark ) map.
//    -------------------------------------
//	  for_all mark in world
//		mark.passable = edge_mask( mark.xy )
//	  world[startxy].passable &= startmask
//	  for_all neighbor of endp
//		neighbor.passable &= is_passable_to( endp )
//////////////////////////////////////////////////////////////////////////////////////////////////
// A* Algorithm improvements and replacement
// 1) Consider MA-IDA or FringeSort algorithms http://www.cs.ualberta.ca/~games/pathfind/
//    Those could forefit the cost of maintaining sorted open list with more advanced 
//	  and memory-hungry techniques. 
// 2) Introduce lower-active-boundary threshould value, which limits the number of 'active' 
//    (sorted) open nodes by its 'g' value. Initially threshold value set on 'H' value
//    and then updated to the total cost of the lowest-cost node that was pruned during 
//	  the previous iteration.
// 3) Do something else, I had enough of pathfinding for today!

//////////////////////////////////////////////////////////////////
//	Configurables
//////////////////////////////////////////////////////////////////

#define MAX_PATH_COST 2048			// maximum path cost until it gets (early) rejected
#define MAX_WORLD_SIZE 256
#define TUNE_MAXHEAPNODES	1024	// could be large as the mapsize however we keep this value capped
#define TUNE_NODESINITIAL	1024	// initial number of nodes, could grow as large as mapsize but unlikely

#define inline 

//////////////////////////////////////////////////////////////////
//	Small Point class (one byte per coordinate)
//////////////////////////////////////////////////////////////////
struct SmallPnt
{
	union {
		struct  {
			uint8 x,y;
		};
		uint16 yx;
	};

	inline SmallPnt( sint16 xx, sint16 yy )
	: x( (uint8)xx )
	, y( (uint8)yy )
	{}
	inline SmallPnt( uint16 c )
	: yx( c )
	{}
	inline bool operator==( const uint16 oyx ) const
	{ return yx == oyx;	}
	inline bool operator==( const SmallPnt other ) const
	{ return yx == other.yx;}
	inline size_t Idx( size_t stride ) const
	{ return stride * y + x; }
	inline iPoint ToPoint() const
	{ return iPoint( (uint32)x, (uint32)y ); }
};


//////////////////////////////////////////////////////////////////
//	Internal structures
//////////////////////////////////////////////////////////////////
const sint16 nOffs[8][2] = {
	{-1,-1}, {0,-1}, {+1,-1}, {+1,0},
	{+1,+1}, {0,+1}, {-1,+1}, {-1,0}
};

struct some_table_supposed_to_be_anonymous {
	uint32	dyx;	// direction offset in packed coordinates
	sint8	coef;	// direction multiplier coefficient
	uint8	dir;	// opposite direction code
} const dtab[9] = 
{
	{ 0xffffffff, 3, 4 },
	{ 0xffff0000, 2, 5 },
	{ 0xffff0001, 3, 6 },
	{ 0x00000001, 2, 7 },
	{ 0x00010001, 3, 0 },
	{ 0x00010000, 2, 1 },
	{ 0x0001ffff, 3, 2 },
	{ 0x0000ffff, 2, 3 },
	{ 0,0,0 }
};

//////////////////////////////////////////////////////////////////
//	Heap management
//////////////////////////////////////////////////////////////////
//#define HeapLess( n1, n2 ) ((0xffff0000 & heap[(n1)]) < (0xffff0000 & heap[(n2)]))
#define HeapLess( n1, n2 ) (heap[(n1)] < heap[(n2)])
#define HeapGetIdx( n ) ((n)&0xffff)
#define HeapGetVal( n ) ((n)>>16)
#define HeapCombine( n1, n2 ) ((HeapVal)((n1<<16)|n2))

//#define HeapSwap( n1, n2 )	{ heap[n1] ^= heap[n2]; heap[n2] ^= heap[n1]; heap[n1] ^= heap[n2];  }
#define HeapSwap( n1, n2 )	{ HeapVal t = heap[n1]; heap[n1] = heap[n2]; heap[n2] = t; }
#define HeapRoot()			(heap[1])

void
iPathFinder::HeapRemoveRoot()
{
	// move last node to the root and delete last leaf
	heap[1] = heap[lastHeapNode--];
	// adjust the tree
	const size_t last	= lastHeapNode;
	size_t k 			= 1;
	while( k <= last ) {
		const size_t kL = k*2;
		const size_t kR = kL + 1;
		size_t best;

		if ( kL <= last ) {
			best = kL;
			if ( kR <= last && !HeapLess(kL,kR)) {
				// Was: best = HeapLess(kL,kR) ? kL : kR;
				best = kR;
			}
		} else break;

		if ( HeapLess( best, k ) ) {
			HeapSwap( k, best );
			k = best;
		} else break;
	}
}

size_t
iPathFinder::HeapInsert( HeapVal n )
{
	// add new node unless the heap is full
	// if full - get rid of the worst node
	if ( lastHeapNode < TUNE_MAXHEAPNODES ) 
		lastHeapNode++;

	heap[lastHeapNode] = n;

	// move k through the nodes upward
	size_t k	= lastHeapNode;

	while( k != 0 ) {
		const size_t parent = k >> 1;
		if ( parent != 0 && HeapLess(k,parent) ) {
			HeapSwap( k, parent );
			k = parent;
		} else break;
	}
	return k;
}

size_t
iPathFinder::HeapRebuild( size_t ndx )
{
	// rebuild the heap up to the element 'ndx' inclusive
	size_t k = ndx;

	while( k != 0 ) {
		const size_t parent = k >> 1;
		if ( parent != 0 && HeapLess(k,parent) ) {
			HeapSwap( k, parent );
			k = parent;
		} else break;
	}
	return k;
}

//////////////////////////////////////////////////////////////////
//	Helper functions
//////////////////////////////////////////////////////////////////

// Fast (branchless) absolute difference for signer 32bit vars
// NB: Seems like the SUN has a patent on this method, fuck them!
inline sint32 FastAbsDifferenceI32( sint32 x, sint32 y )
{
	x -= y;
	return (x ^ (x>>31)) - (x>>31);
}

// Okay, continue with branchless code
inline sint32 FastMinI32( sint32 x, sint32 y )
{
	return ( (x+y) - FastAbsDifferenceI32(x,y) ) / 2;
}

inline sint32 sdif( sint32 a, sint32 b )
{ a -= b; return a < 0 ? -a : a; }
inline sint32 smin( sint32 a, sint32 b )
{ return a < b ? a : b; }

// Compute '8-connected' map distance, used as heuristic
// NB: See common A* heuristic definition rules
inline uint32 Distance(sint32 sposX, sint32 sposY, sint32 dposX, sint32 dposY) 
{ 
	// Please avoid using dumb-ass iDIF function
	const sint32 dX = sdif(sposX,dposX);
	const sint32 dY = sdif(sposY,dposY);
	const sint32 diagDist = smin( dX, dY );
	const sint32 manhDist  = dX + dY;

	const sint32 DiagCost = (14/2); //10*sqrt(2)
	const sint32 ForwCost = (10/2); //10*1 
	//return DiagCost * diagDist + ForwCost * ( manhDist - 2 * diagDist );
	return ForwCost * manhDist - ( 2*ForwCost - DiagCost ) * diagDist;
}

inline uint32 Distance( const SmallPnt s, sint32 dposX, sint32 dposY ) 
{ 
	const sint32 sposX = s.x;
	const sint32 sposY = s.y;
	return Distance( sposX, sposY, dposX, dposY );
}

// check whether the direction is passable given the bitmask
// NB: Notion of pass bits is changed to reverse, i.e. set bit = blocked
inline bool IsDirectionPassable( const uint8 bitmask, size_t bit ) 
{
	return (bitmask & (1<<bit)) == 0;
}

template< typename T >
inline T* ReallocPODArray( T* src, size_t oldSize, size_t newSize )
{
	size_t nsize = newSize * sizeof(T);
	if ( src == 0 ) {
		return (T*)malloc( nsize );
	} else if ( nsize == 0 ) {
		free( src );
		return (T*)0;
	} else {
		void* dst = malloc( nsize );
		size_t to_copy = nsize > oldSize ? oldSize : nsize;
		memcpy( dst, src, to_copy );
		free( src );
		return (T*)dst;
	}
	//return (T*) realloc( src, newSize * sizeof(T) );
}

bool MoveAndValidate( SmallPnt& org, uint32 offset, uint32 mapW, uint32 mapH )
{
	uint16 px = (uint16)org.x + (uint16)(offset & 0xffff);
	uint16 py = (uint16)org.y + (uint16)(offset >> 16);
	if ( px >= mapW || py >= mapH ) return false;

	org = SmallPnt( px, py );
	return true;
}

SmallPnt MovePoint( SmallPnt org, uint32 offset )
{
	uint16 px = (uint16)org.x + (uint16)(offset & 0xffff);
	uint16 py = (uint16)org.y + (uint16)(offset >> 16);
	return SmallPnt( px, py );
}

inline uint16 CalculateCost( uint16 cost, uint16 dirMult )
{
	return (uint16)((((uint32)cost) * dirMult) >> 1);
}

//////////////////////////////////////////////////////////////////
//	Diagnostic output
//////////////////////////////////////////////////////////////////

void __cdecl debugf( const WCHAR* fmt, ... )
{
	va_list args;
	WCHAR buf[256];
	va_start( args, fmt );
	vswprintf( buf, fmt, args );
	va_end( args );
	OutputDebugStringW( buf );
}

#if 0
#define DOUT debugf
#define DSTAT( x ) x
#else
#	if defined(__GNUC__)
#		define DOUT(...)
#	elif defined(__INTEL_COMPILER) || (_MSC_VER >= 1310)
#		define DOUT __noop
#	else
#		define DOUT ((void)0)
#	endif
#define DSTAT( x )
#endif

#if 0
#include "perf.h"
#define TIMING debugf
#define TSTART(x) TIMER_START(x)
#define TEND(x) TIMER_END(x)
#define TVAL(x) TIMER_VAL(x)
#else
#	if defined(__GNUC__)
#		define TIMING(...)
#	elif defined(__INTEL_COMPILER) || (_MSC_VER >= 1310)
#		define TIMING __noop
#	else
#		define TIMING ((void)0)
#	endif
#define TSTART(x)
#define TEND(x)
#define TVAL(x)	0
#endif


//
//

#if 1
void iPath::DumpPath() const
{
	debugf(L"Dumping path (%d,%d):\n", m_Start.x, m_Start.y);
	for (uint32 xx=0; xx<m_Path.GetSize(); ++xx) {
		debugf(L"--- Step (%d,%d) : %d [%d]\n", m_Path[xx].m_Pos.x, m_Path[xx].m_Pos.y, m_Path[xx].m_Cost, m_Path[xx].m_action);
	}
	debugf(L"End of Dumping path.\n", m_Start.x, m_Start.y);
}
#else
void iPath::DumpPath() const
{}
#endif

//
// Algorithmus
// Note node store the SmallPnt in yx

sint32 iPathFinder::FindPath(const iPoint& srcPos, const iPoint& dstPos, iPath& path, uint8 spMask, uint8 epMask)
{
	TSTART(findpath);
	SmallPnt startp( (uint8)srcPos.x, (uint8)srcPos.y );
	SmallPnt endp( (uint8)dstPos.x, (uint8)dstPos.y );
	DOUT( L"# routing from %d,%d to %d,%d\n", srcPos.x, srcPos.y, dstPos.x, dstPos.y );

	// init search
	size_t lastNode	 = 1;
	lastHeapNode= 1;

	nodes[1].yx		= startp.yx;
	nodes[1].g		= 0;

	uint32 node1f	= Distance( startp.yx, (uint32)dstPos.x, (uint32)dstPos.y ); 
	heap[1]			= HeapCombine ( node1f, lastNode );

	// enabled direction
	SetupEntryExitFlags( startp.yx, endp.yx, spMask, epMask );

	// check preconditions

	// begin search with first node (additional condition might be dropped)
	size_t nodesClosed = 0;
	size_t nodesOpened = 1;
	size_t nodesDeleted= 0;
	size_t nodesPeak   = 1;

	// used to track the position (in the heap) of the last inserted element (for fast erase proc)
	size_t lastInsertIndex = 1;
	// -- STILL HAVE OPEN ?
	while ( lastHeapNode != 0 ) {
		const size_t parentIdx = HeapGetIdx( HeapRoot() );

		// CLOSE this node
		SmallPnt parentPt( nodes[ parentIdx ].yx );
		uint16 parentG = nodes[ parentIdx ].g;

		if ( endp == parentPt ) break; // !!!! we reached the point.. 

		// Mark as closed & remove from the open nodes heap
		world[ parentPt.Idx(mapW) ].state = SClosed;
		DOUT( L"# picked up node #%d @ %d,%d\n", HeapGetVal( HeapRoot() ), parentPt.x, parentPt.y );
		HeapRemoveRoot();
		nodesClosed++;

		// Check if we current path cost is too high and check other possible path if so
		if ( parentG > MAX_PATH_COST ) {
			DOUT( L"## current path cost is too high, reject\n" );
			continue;
		}

		uint8 parentAccess = world[ parentPt.Idx(mapW) ].pass;
		// Now check all the directions 
		for( size_t dirCode = 0; dirCode != 8; ++dirCode ) {
			// skip inaccessible directions
			//check( parentAccess == 0xff );
			if ( (parentAccess & (1<<dirCode) ) ) {
				DOUT( L"### direction %d is blocked\n", dirCode );
				continue;
			}
			// Calculate new yx position
			// Validate position and skip the rest if point is unreachable
			DOUT( L"### checking direction %d\n", dirCode );
			SmallPnt npt = MovePoint( parentPt, dtab[ dirCode ].dyx );
			DOUT( L"### pos %d,%d\n", npt.x, npt.y);

			Mark& wpnt = world[ npt.Idx(mapW) ];
			const sint8 wcost = passMap[ npt.Idx(mapW) ];
			// NB:: Optimize - too much checks performed!
			if ( wpnt.state >= SClosed || wcost < 0 ) {
				DOUT( L"#### rejected due constraints\n" );
				continue;
			}

			const uint16 newG = parentG + CalculateCost( (uint16)wcost, dtab[dirCode].coef );

			// Map Point is reachable from this direction, proceed
			if ( wpnt.state == SUnknown ) {
				// OPEN THAT CELL

				// Mark as open and store direction
				wpnt.state	= SOpen;
				wpnt.dir	= dirCode;
				wpnt.g		= newG;

				// NB:: check if we beyond the limits, resize array if necessary
				// NB:: in this algorithm we do not delete unused nodes, the array might grow as large as whole map size
				if ( lastNode == maxNodesNum ) {
					nodes = ReallocPODArray( nodes, sizeof(Node) * maxNodesNum, sizeof(Node) * maxNodesNum * 2);
					maxNodesNum *= 2;
					DOUT( L"### Nodes array extended to hold up to %d elements", maxNodesNum );
				}
				Node& newNode = nodes[ ++lastNode ];
				// set node position and heurustuc 
				// NB:: check for overflow of uint16!
				newNode.yx	= npt.yx;
				newNode.g	= newG;
				uint32 newNodef	= newG + Distance( npt, dstPos.x, dstPos.y );

				DOUT( L"#### opened - cost %d, g:%d, f:%d\n", wcost, newNode.g, newNodef );
				// now insert new node to the open list
				lastInsertIndex = HeapInsert( HeapCombine(newNodef,lastNode) );
				nodesOpened++;

				// CHECK IF THERE IS BETTER WAY FOR ALREADY OPENED 
			} else if ( newG < wpnt.g  ) {
				check( wpnt.state == SOpen );
				lastInsertIndex = 0;
				// search in open list (heap) -- thats bad because of indirection!

				DOUT(L"#### looking for node in heap with coords %04x\n", npt.yx );
				check( lastHeapNode > 0 );
				size_t foundNdx = 1;
				do {
					DOUT(L"##### %03d > %03d : %04x\n", foundNdx, HeapGetIdx(heap[foundNdx]), nodes[HeapGetIdx(heap[foundNdx])].yx );
				} while ( !(npt.yx == nodes[ HeapGetIdx(heap[foundNdx]) ].yx) && foundNdx++ != lastHeapNode  );

				//check( prev != lastHeapNode+1);
				// actually we can get into the situation when the node is not on the open list
				// since its has limited capacity it could drop the worst 'f' elements
				// however its not fatal (less optimal path might be generated) 
				if ( foundNdx > lastHeapNode ) {
					DOUT(L"##### rare - node not on the open list\n" );
					continue;
				}

				// we know what the node really exists
				DOUT( L"#### updated - dir:%d > %d g:%d > %d\n", wpnt.dir, dirCode, wpnt.g, newG );				

				// update world
				wpnt.dir	= dirCode;
				wpnt.g		= newG;

				// update node and heap
				const HeapVal heapFound = heap[foundNdx];
				Node& updated = nodes[ HeapGetIdx(heapFound) ];
				//check( newG < updated.g); --- thats why no underflow here could happen
				const uint32 newF = HeapGetVal(heapFound) - (updated.g - newG);
				heap[foundNdx] = HeapCombine( newF, HeapGetIdx(heapFound) );
				updated.g = newG;
				// rebuild heap
				HeapRebuild( foundNdx );
			}
		}

		nodesPeak = iMAX( nodesPeak, lastNode );

		// NB:: here we can actually remove the closed 'parentNode'
		// howere this technique is limited to situation when we actually add a new node
		// so few closed node will remain on the list
		// copying: nodes[lastNode--] -> nodes[parentIdx] and
		// fixing the heap index: heap[lastInsertedIndex] = parentIdx
		if ( lastInsertIndex != 0 ) {
			nodes[parentIdx] = nodes[lastNode--];
			heap[lastInsertIndex] = HeapCombine( HeapGetVal(heap[lastInsertIndex]), parentIdx  );
			lastInsertIndex = 0;
			nodesDeleted++;
		}

	}
	TEND(findpath);
	TIMING( L"#$ find path completed in %d units\n", TVAL(findpath) );

	// -- WE WERE DONE SOMEHOW

	DOUT( L"# finished (opened=%d,closed=%d,deleted=%d,peak=%d,heapsz=%d)\n", nodesOpened, nodesClosed, nodesDeleted, nodesPeak, lastHeapNode );			
	if ( lastHeapNode != 0 ) {
		Node& dstNode = nodes[ HeapGetIdx(HeapRoot()) ];
		if ( endp == dstNode.yx && dstNode.g < MAX_PATH_COST ) {
			SmallPnt cur(endp);
			DOUT( L"# packing route from %d,%d to %d,%d\n", endp.x, endp.y, startp.x, startp.y );
			while ( !(cur == startp) ) {
				// ???should we take the opposite direction????
				size_t  idx = cur.Idx(mapW);
				uint16	dir = dtab[ world[ idx ].dir ].dir; // reverse direction
				uint16	cost= CalculateCost( (uint16)passMap[ idx ], dtab[ dir ].coef ); 
				
				path.AddMoveToPoint( cur.ToPoint(), cost );

				DOUT( L"### adding point %d,%d cost %d, dir %d\n", cur.x, cur.y, cost, dir );
				cur = MovePoint( cur, dtab[dir].dyx );
			}
			return 1;
		}
	}

	return -1;
}

//
// NB:: Masks are inverted, i.e. 1 bit means blocked 
void
iPathFinder::SetupEntryExitFlags( uint16 sp, uint16 ep, uint8 exitMask, uint8 entryMask )
{
	check( sp != ep );
	SmallPnt startp( sp );
	SmallPnt endp( ep );

	// invert masks - switch from pass to block notion
	entryMask = ~entryMask;
	exitMask  = ~exitMask;

	// start point is simple as
	world[ startp.Idx( mapW ) ].pass |= exitMask;
	// end point more complex
	uint8 accessible = world[ endp.Idx( mapW ) ].pass;
	// walk all neighbors and fix their pass map
	for( size_t d = 0; d != 8; d++ ) {
		// skip inaccessible
		if ( !IsDirectionPassable( accessible, d ) ) continue;
		SmallPnt neib = MovePoint( endp, dtab[ d ].dyx );
		// 
		size_t inv_dir = dtab[ d ].dir;
		if ( !IsDirectionPassable( entryMask, inv_dir ) ) {
			world[ neib.Idx( mapW ) ].pass |= (1UL<<(inv_dir));
		}
	}
}

void
iPathFinder::PrepareEmptyWorld()
{
	check( world != NULL );

	memset( world, 0, sizeof(Mark)*mapW * mapH );

	const size_t stride2 = mapW-2;
	size_t  k;
	Mark* wp = world;

	(wp++)->pass = (uint8)~0x38;		// 00111000
	for( k = 0; k != stride2; ++k )
		(wp++)->pass = (uint8)~0xf8;	// 11111000
	(wp++)->pass = (uint8)~0xe0;		// 11100000
	for( k = 0; k != stride2; ++k ) {
		(wp++)->pass = (uint8)~0x3e;	// 00111110
		wp += stride2;
		(wp++)->pass = (uint8)~0xe3;	// 11100011
	}
	(wp++)->pass = (uint8)~0x0e;		// 00001110
	for( k = 0; k != stride2; ++k )
		(wp++)->pass = (uint8)~0x8f;	// 10001111
	(wp++)->pass = (uint8)~0x83;		// 10000011
}

//
//
size_t			
iPathFinder::entryCount	= 0;

size_t
iPathFinder::maxNodesNum= 0;

iPathFinder::Node*
iPathFinder::nodes		= NULL;

iPathFinder::HeapVal*
iPathFinder::heap		= NULL;

size_t
iPathFinder::lastHeapNode= 0;

void
iPathFinder::Purge()
{
	maxNodesNum	= 0;
	free( nodes );
	nodes = NULL;
	free( heap );
	heap = NULL;
}

iPathFinder::iPathFinder(const iPassMap& pmap)
: mapW( pmap.GetWidth() )
, mapH( pmap.GetHeight() )
{
	check( mapW <= 256 );
	check( mapH <= 256 );

	// check for reentrance -- since we using static structures
	check( entryCount++ == 0 );

	// check and adjust nodes storage
	if ( maxNodesNum < TUNE_NODESINITIAL ) {
		maxNodesNum = TUNE_NODESINITIAL;
		nodes = ReallocPODArray( nodes, 0, sizeof(Node) * maxNodesNum );
	}
	nodes[0].yx = 0;
	nodes[0].g	= 0;

	// check and adjust heap storage
	if ( heap == NULL ) {
		heap = (HeapVal*)malloc( sizeof(HeapVal) * TUNE_MAXHEAPNODES );
	}
	heap[0]			= 0;
	lastHeapNode	= 0;

	// allocate world & init 
	world = (Mark*)malloc( sizeof(Mark) * mapW * mapH );

	PrepareEmptyWorld();

	passMap = pmap.GetPtr();
}

iPathFinder::~iPathFinder()
{
	free( world );

	// decrease entrance counter
	--entryCount;
}



