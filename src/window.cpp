/* /~\_.-~-._,--.._
  |                ~-.._
   |     .     .        \
   |                    /
   |     --....,       | \
   /                      \
  |                        |
  |                        \
  /                         |
  \_.-._  __...___.._______/
        ~~                   */

#include <stdexcept>

#include <tdpi/tdpi.h>
#include <GLFW/glfw3.h>

#include "tppi/window.hpp"

using namespace tppi;

struct glfw_window_deleter {
    void operator()(GLFWwindow* glfw_window) { glfwDestroyWindow(glfw_window); }
};

class window::window_implementation {
public:
    window_implementation(const unsigned int width, const unsigned int height, const std::string& title, const window_mode window_mode_, const resizable resizable_) {
        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW.");

        glfwSetWindowUserPointer(glfw_window.get(), this);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, (static_cast<bool>(resizable_) ? GL_TRUE : GL_FALSE));

        if (window_mode_ == window_mode::windowed || window_mode_ == window_mode::fullscreen) glfw_window.reset(glfwCreateWindow(width, height, title.c_str(), (static_cast<bool>(window_mode_) ? glfwGetPrimaryMonitor() : nullptr), nullptr));
        else {
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);
            glfw_window.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
        }

        if (!glfw_window.get()) throw std::runtime_error("Failed to create an OpenGL 3.3 context.");

        make_current();

        if (!tdpiLoadGL()) throw std::runtime_error("Failed to load OpenGL functions.");
    }

    ~window_implementation() {
        glfwSetScrollCallback(glfw_window.get(), nullptr);
        glfwSetWindowFocusCallback(glfw_window.get(), nullptr);
    }

    void make_current() noexcept {
        glfwMakeContextCurrent(glfw_window.get());

        glfwSetScrollCallback(glfw_window.get(), set_scroll_offset);
        glfwSetWindowFocusCallback(glfw_window.get(), set_focused);
    }

    void resize(const unsigned int width, const unsigned int height) noexcept { glfwSetWindowSize(glfw_window.get(), width, height); }
    void title(const std::string& new_title) { glfwSetWindowTitle(glfw_window.get(), new_title.c_str()); }

    void update() noexcept {
        glfwPollEvents();
        glfwSwapBuffers(glfw_window.get());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool button_is_pressed(const unsigned int key) const noexcept { return glfwGetKey(glfw_window.get(), key); }

    double scroll_offset() const noexcept { return scroll_offset_; }
    bool focused() const noexcept { return focused_; }

    coordinate_2d cursor_position() const noexcept {
        coordinate_2d new_position{0, 0};
        glfwGetCursorPos(glfw_window.get(), &new_position.x, &new_position.y);

        return new_position;
    }

    void cursor_position(const coordinate_2d new_position) noexcept { glfwSetCursorPos(glfw_window.get(), new_position.x, new_position.y); }

    std::string clipboard_string() { return glfwGetClipboardString(glfw_window.get()); }
    void clipboard_string(const std::string& new_clipboard_string) { glfwSetClipboardString(glfw_window.get(), new_clipboard_string.c_str()); }
private:
    std::unique_ptr<GLFWwindow, glfw_window_deleter> glfw_window;

    static void set_scroll_offset(GLFWwindow* window_ptr, const double new_scroll_offset, double) noexcept { static_cast<window_implementation*>(glfwGetWindowUserPointer(window_ptr))->scroll_offset_ = new_scroll_offset; }
    static void set_focused(GLFWwindow* window_ptr, const int is_window_in_focus) { static_cast<window_implementation*>(glfwGetWindowUserPointer(window_ptr))->focused_ = is_window_in_focus; }

    double scroll_offset_ = 0;
    bool focused_ = false;
};

window::window(const unsigned int width, const unsigned int height, const std::string& title, window_mode window_mode_, resizable resizable_) : window_implementation_(std::make_unique<window_implementation>(width, height, title, window_mode_, resizable_)) {
    if (reference_count > 0) throw std::runtime_error("Multiple windows aren't yet supported.");

    ++reference_count;
    current_window.reset(this);
}

window::~window() {
    --reference_count;
    if (reference_count == 0) glfwTerminate();

    current_window.reset();
}

void window::make_current() noexcept {
    window_implementation_->make_current();
    current_window.reset(this);
}

void window::resize(const unsigned int width, const unsigned int height) noexcept { window_implementation_->resize(width, height); }
void window::title(const std::string& new_title) { window_implementation_->title(new_title); }

void window::update() noexcept { window_implementation_->update(); }

bool window::button_is_pressed(const button button_) const noexcept { return window_implementation_->button_is_pressed(static_cast<unsigned int>(button_)); }

double window::scroll_offset() const noexcept { return window_implementation_->scroll_offset(); }
bool window::focused() const noexcept { return window_implementation_->focused(); }

coordinate_2d window::cursor_position() const noexcept { return window_implementation_->cursor_position(); }
void window::cursor_position(const coordinate_2d& new_position) noexcept { window_implementation_->cursor_position(new_position); }

std::string window::clipboard_string() const { return window_implementation_->clipboard_string(); }
void window::clipboard_string(const std::string& new_clipboard_string) { window_implementation_->clipboard_string(new_clipboard_string); }

int window::count() noexcept { return reference_count; }

unsigned int window::reference_count = 0;
std::unique_ptr<window> window::current_window = nullptr;
