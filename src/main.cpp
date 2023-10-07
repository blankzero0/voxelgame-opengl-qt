#include <QGuiApplication>
#include <QSurfaceFormat>
#include "window.h"
#include "player.h"
#include "physics_loop.h"
#include "world_builder.h"

int main(int argc, char* argv[])
{
	World world;
	Player player{{8, 16, 5}};
	Input input{};

	WorldBuilder world_builder(world);
	player.add_position_listener([&](const Point& position){world_builder.set_center(position);});

	QGuiApplication app(argc, argv);

	QSurfaceFormat format;
	format.setMajorVersion(4);
	format.setMinorVersion(5);
	format.setProfile(QSurfaceFormat::CoreProfile);
#ifndef NDEBUG
	format.setOption(QSurfaceFormat::DebugContext);
#endif
	format.setDepthBufferSize(24);
	format.setSwapInterval(1);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	Window window(world, player, input);
	window.show();

	PhysicsLoop physics_loop(player, input, world);

	return QGuiApplication::exec();
}
