#ifndef VOXELGAME_OPENGL_QT_VERTEX_PRODUCER_H
#define VOXELGAME_OPENGL_QT_VERTEX_PRODUCER_H


#include <cassert>
#include "vertex_consumer.h"

template<typename Vertex>
class VertexProducer {
public:
	void set_vertex_consumer(VertexConsumer<Vertex>& vertex_consumer)
	{
		VertexProducer::vertex_consumer = &vertex_consumer;
	}

protected:
	VertexConsumer<Vertex>& get_vertex_consumer()
	{
		assert(vertex_consumer);
		return *vertex_consumer;
	}

private:
	VertexConsumer<Vertex>* vertex_consumer = nullptr;
};


#endif //VOXELGAME_OPENGL_QT_VERTEX_PRODUCER_H
