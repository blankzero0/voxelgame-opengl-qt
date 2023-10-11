#ifndef VOXELGAME_OPENGL_QT_VERTEX_BUILDER_H
#define VOXELGAME_OPENGL_QT_VERTEX_BUILDER_H


#include "vertex_producer.h"
#include "chunk_surrounder.h"

template<typename Vertex>
class VertexBuilder : public VertexProducer<Vertex>, public ChunkRequester {};


#endif //VOXELGAME_OPENGL_QT_VERTEX_BUILDER_H
