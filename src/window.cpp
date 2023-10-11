#include "window.h"
#include "raycast.h"
#include <stdexcept>
#include <QOpenGLBuffer>
#include <QMouseEvent>



void Window::initializeGL()
{
	if (!logger.initialize()) {
		throw std::runtime_error("logger init failed");
	}
	logger.disableMessages(
			QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::NotificationSeverity
	);
	logger.startLogging();

	if (!initializeOpenGLFunctions()) {
		throw std::runtime_error("gl initialization failed");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glClearColor(113 / 255.0, 188 / 255.0, 225 / 255.0, 1);

	world_renderer.emplace(world, player, time_bounded_gl_executor);
	selection_renderer.emplace();
}

void Window::resizeGL(int w, int h)
{
	projection_matrix.setToIdentity();
	projection_matrix.perspective(70, static_cast<float>(w) / static_cast<float>(h), 0.1, 300);
}

void Window::paintGL()
{
	std::chrono::time_point current_render = std::chrono::steady_clock::now();
	std::chrono::duration render_time = current_render - last_render;

	const Point& player_position = player.get_position();
	Point eye_position{player_position.x, player_position.y + Player::eye_height, player_position.z};

	QMatrix4x4 view;
	view.rotate(-player.pitch_degrees(), 1, 0, 0);
	view.rotate(-player.yaw_degrees(), 0, -1, 0);
	view.translate(-eye_position.x, -eye_position.y, -eye_position.z);

	QMatrix4x4 view_projection = projection_matrix * view;
	world_renderer->set_view_projection(view_projection);
	selection_renderer->set_view_projection(view_projection);

	time_bounded_gl_executor.run();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world_renderer->set_center(player_position);
	world_renderer->render();

	Vector look_direction = rotation_to_forward_vector(player.get_yaw(), player.get_pitch());
	auto looking_at = raycast(world, eye_position, look_direction, 10);
	if (looking_at) {
		glClear(GL_DEPTH_BUFFER_BIT);
		selection_renderer->set_position(std::get<0>(*looking_at));
		selection_renderer->render();
	}

	last_render = current_render;
}

Window::Window(World& world, Player& player, Input& input)
	: player(player), input(input), world(world)
{
	connect(&logger, &QOpenGLDebugLogger::messageLogged, [](const QOpenGLDebugMessage& message) { qDebug() << message; });
	connect(this, &Window::frameSwapped, this, qOverload<>(&Window::update));
	setCursor(Qt::BlankCursor);
}

void Window::mouseMoveEvent(QMouseEvent* event)
{
	QPoint center = geometry().center();
	QPointF delta = event->globalPosition() - center;

	player.turn(delta.x() / 300, -delta.y() / 300);

	QCursor::setPos(center);
}

void Window::keyPressEvent(QKeyEvent* event)
{
	std::optional<Input::setter> setter = Input::get_setter_from_keycode(event->key());
	if (!setter)
		return;
	(input.*setter.value())(true);
}

void Window::keyReleaseEvent(QKeyEvent* event)
{
	std::optional<Input::setter> setter = Input::get_setter_from_keycode(event->key());
	if (!setter)
		return;
	(input.*setter.value())(false);
}

Window::~Window()
{
	makeCurrent();
}

void Window::mousePressEvent(QMouseEvent* event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		input.set_destroy(true);
		break;
	case Qt::RightButton:
		input.set_place(true);
		break;
	default:
		break;
	}
}

void Window::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		input.set_destroy(false);
		break;
	case Qt::RightButton:
		input.set_place(false);
		break;
	default:
		break;
	}
}
