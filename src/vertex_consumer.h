#ifndef VOXELGAME_OPENGL_QT_VERTEX_CONSUMER_H
#define VOXELGAME_OPENGL_QT_VERTEX_CONSUMER_H


#include <utility>
#include "geometry.h"

template<typename Vertex>
class VertexConsumer {
public:
	virtual void consume_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&&) = 0;
};


#endif //VOXELGAME_OPENGL_QT_VERTEX_CONSUMER_H
