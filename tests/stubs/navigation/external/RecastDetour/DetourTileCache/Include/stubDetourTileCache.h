// AUTOGENERATED file, created by the tool generate_stub.py, don't edit!
// If you want to add your own functionality, instead edit the stubDetourTileCache_custom.h file.

#include "navigation/external/RecastDetour/DetourTileCache/Include/DetourTileCache.h"
#include "stubDetourTileCache_custom.h"

#ifndef STUB_NAVIGATION_EXTERNAL_RECASTDETOUR_DETOURTILECACHE_INCLUDE_DETOURTILECACHE_H
#define STUB_NAVIGATION_EXTERNAL_RECASTDETOUR_DETOURTILECACHE_INCLUDE_DETOURTILECACHE_H




#ifndef STUB_dtTileCacheMeshProcess_process
//#define STUB_dtTileCacheMeshProcess_process
  void dtTileCacheMeshProcess::process(struct dtNavMeshCreateParams* params, unsigned char* polyAreas, unsigned short* polyFlags)
  {
    
  }
#endif //STUB_dtTileCacheMeshProcess_process


#ifndef STUB_dtTileCache_dtTileCache
//#define STUB_dtTileCache_dtTileCache
   dtTileCache::dtTileCache()
    : m_nextFreeTile(nullptr),m_tiles(nullptr),m_talloc(nullptr),m_tcomp(nullptr),m_tmproc(nullptr),m_obstacles(nullptr),m_nextFreeObstacle(nullptr)
  {
    
  }
#endif //STUB_dtTileCache_dtTileCache

#ifndef STUB_dtTileCache_dtTileCache_DTOR
//#define STUB_dtTileCache_dtTileCache_DTOR
   dtTileCache::~dtTileCache()
  {
    
  }
#endif //STUB_dtTileCache_dtTileCache_DTOR

#ifndef STUB_dtTileCache_getObstacleByRef
//#define STUB_dtTileCache_getObstacleByRef
  const dtTileCacheObstacle* dtTileCache::getObstacleByRef(dtObstacleRef ref)
  {
    return nullptr;
  }
#endif //STUB_dtTileCache_getObstacleByRef

#ifndef STUB_dtTileCache_getObstacleRef
//#define STUB_dtTileCache_getObstacleRef
  dtObstacleRef dtTileCache::getObstacleRef(const dtTileCacheObstacle* obmin) const
  {
    return *static_cast<dtObstacleRef*>(nullptr);
  }
#endif //STUB_dtTileCache_getObstacleRef

#ifndef STUB_dtTileCache_init
//#define STUB_dtTileCache_init
  dtStatus dtTileCache::init(const dtTileCacheParams* params, struct dtTileCacheAlloc* talloc, struct dtTileCacheCompressor* tcomp, struct dtTileCacheMeshProcess* tmproc)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_init

#ifndef STUB_dtTileCache_getTilesAt
//#define STUB_dtTileCache_getTilesAt
  int dtTileCache::getTilesAt(const int tx, const int ty, dtCompressedTileRef* tiles, const int maxTiles) const
  {
    return 0;
  }
#endif //STUB_dtTileCache_getTilesAt

#ifndef STUB_dtTileCache_getTileAt
//#define STUB_dtTileCache_getTileAt
  dtCompressedTile* dtTileCache::getTileAt(const int tx, const int ty, const int tlayer)
  {
    return nullptr;
  }
#endif //STUB_dtTileCache_getTileAt

#ifndef STUB_dtTileCache_getTileRef
//#define STUB_dtTileCache_getTileRef
  dtCompressedTileRef dtTileCache::getTileRef(const dtCompressedTile* tile) const
  {
    return *static_cast<dtCompressedTileRef*>(nullptr);
  }
#endif //STUB_dtTileCache_getTileRef

#ifndef STUB_dtTileCache_getTileByRef
//#define STUB_dtTileCache_getTileByRef
  const dtCompressedTile* dtTileCache::getTileByRef(dtCompressedTileRef ref) const
  {
    return nullptr;
  }
#endif //STUB_dtTileCache_getTileByRef

#ifndef STUB_dtTileCache_addTile
//#define STUB_dtTileCache_addTile
  dtStatus dtTileCache::addTile(unsigned char* data, const int dataSize, unsigned char flags, dtCompressedTileRef* result)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_addTile

#ifndef STUB_dtTileCache_removeTile
//#define STUB_dtTileCache_removeTile
  dtStatus dtTileCache::removeTile(dtCompressedTileRef ref, unsigned char** data, int* dataSize)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_removeTile

#ifndef STUB_dtTileCache_addObstacle
//#define STUB_dtTileCache_addObstacle
  dtStatus dtTileCache::addObstacle(const float* pos, const float radius, const float height, dtObstacleRef* result)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_addObstacle

#ifndef STUB_dtTileCache_removeObstacle
//#define STUB_dtTileCache_removeObstacle
  dtStatus dtTileCache::removeObstacle(const dtObstacleRef ref)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_removeObstacle

#ifndef STUB_dtTileCache_queryTiles
//#define STUB_dtTileCache_queryTiles
  dtStatus dtTileCache::queryTiles(const float* bmin, const float* bmax, dtCompressedTileRef* results, int* resultCount, const int maxResults) const
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_queryTiles

#ifndef STUB_dtTileCache_update
//#define STUB_dtTileCache_update
  dtStatus dtTileCache::update(const float /*dt*/, class dtNavMesh* navmesh)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_update

#ifndef STUB_dtTileCache_buildNavMeshTilesAt
//#define STUB_dtTileCache_buildNavMeshTilesAt
  dtStatus dtTileCache::buildNavMeshTilesAt(const int tx, const int ty, class dtNavMesh* navmesh)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_buildNavMeshTilesAt

#ifndef STUB_dtTileCache_buildNavMeshTile
//#define STUB_dtTileCache_buildNavMeshTile
  dtStatus dtTileCache::buildNavMeshTile(const dtCompressedTileRef ref, class dtNavMesh* navmesh)
  {
    return *static_cast<dtStatus*>(nullptr);
  }
#endif //STUB_dtTileCache_buildNavMeshTile

#ifndef STUB_dtTileCache_calcTightTileBounds
//#define STUB_dtTileCache_calcTightTileBounds
  void dtTileCache::calcTightTileBounds(const struct dtTileCacheLayerHeader* header, float* bmin, float* bmax) const
  {
    
  }
#endif //STUB_dtTileCache_calcTightTileBounds

#ifndef STUB_dtTileCache_getObstacleBounds
//#define STUB_dtTileCache_getObstacleBounds
  void dtTileCache::getObstacleBounds(const struct dtTileCacheObstacle* ob, float* bmin, float* bmax) const
  {
    
  }
#endif //STUB_dtTileCache_getObstacleBounds


#endif