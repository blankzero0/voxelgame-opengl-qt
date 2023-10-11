#ifndef VOXELGAME_OPENGL_QT_UPDATE_CONNECTOR_H
#define VOXELGAME_OPENGL_QT_UPDATE_CONNECTOR_H


#include "geometry.h"

class UpdateConnector {
public:
	virtual ~UpdateConnector() = default;

	virtual void connect(const ChunkPosition& position) = 0;
	virtual void disconnect(const ChunkPosition& position) = 0;
};


#endif //VOXELGAME_OPENGL_QT_UPDATE_CONNECTOR_H
