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

struct glfw_window_deleter
{
    void operator()(GLFWwindow* glfw_window) { glfwDestroyWindow(glfw_window); }
};

class window::window_impl
{
public:
    window_impl(unsigned int width, unsigned int height, const std::string& title, bool fullscreen, bool resizable)
    {
        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW.");

        glfwSetWindowUserPointer(glfw_window.get(), this);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, (resizable ? GL_TRUE : GL_FALSE));

        glfw_window.reset(glfwCreateWindow(width, height, title.c_str(), (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr));
        if (!glfw_window.get()) throw std::runtime_error("Failed to create an OpenGL 3.3 context.");

        make_current();

        if (!tdpiLoadGL()) throw std::runtime_error("Failed to load OpenGL functions.");
    }

    ~window_impl()
    {
        glfwSetScrollCallback(glfw_window.get(), nullptr);
        glfwSetWindowFocusCallback(glfw_window.get(), nullptr);
    }

    void make_current() noexcept
    {
        glfwMakeContextCurrent(glfw_window.get());

        glfwSetScrollCallback(glfw_window.get(), set_scroll_offset);
        glfwSetWindowFocusCallback(glfw_window.get(), set_focused);
    }

    void resize(unsigned int width, unsigned int height) noexcept { glfwSetWindowSize(glfw_window.get(), width, height); }
    void title(const std::string& new_title) { glfwSetWindowTitle(glfw_window.get(), new_title.c_str()); }

    bool button_is_pressed(unsigned int key) const noexcept { return glfwGetKey(glfw_window.get(), key); }

    double scroll_offset() const noexcept { return scroll_offset_; }
    bool focused() const noexcept { return focused_; }

    cursor_pos cursor_position() const noexcept
    {
        cursor_pos cursor_pos_{0, 0};
        glfwGetCursorPos(glfw_window.get(), &cursor_pos_.x, &cursor_pos_.y);

        return cursor_pos_;
    }

    void cursor_position(cursor_pos cursor_pos_) noexcept { glfwSetCursorPos(glfw_window.get(), cursor_pos_.x, cursor_pos_.y); }

    std::string clipboard_string() { return glfwGetClipboardString(glfw_window.get()); }
    void clipboard_string(const std::string& new_clipboard_string) { glfwSetClipboardString(glfw_window.get(), new_clipboard_string.c_str()); }
private:
    std::unique_ptr<GLFWwindow, glfw_window_deleter> glfw_window;

    static void set_scroll_offset(GLFWwindow* window_ptr, double new_scroll_offset, double) noexcept { static_cast<window_impl*>(glfwGetWindowUserPointer(window_ptr)) -> scroll_offset_ = new_scroll_offset; }
    static void set_focused(GLFWwindow* window_ptr, int is_window_in_focus) { static_cast<window_impl*>(glfwGetWindowUserPointer(window_ptr)) -> focused_ = is_window_in_focus; }
    double scroll_offset_ = 0;
    bool focused_ = false;
};

window::window(unsigned int width, unsigned int height, const std::string& title, bool fullscreen, bool resizable) : window_impl_(std::make_unique<window_impl>(width, height, title, fullscreen, resizable))
{
    (reference_count > 1) ? throw std::runtime_error("Multiple windows aren't yet supported.") : reference_count++;
    current_window.reset(this);
}
window::~window()
{
    --reference_count;
    if (reference_count == 0) glfwTerminate();

    current_window.reset();
}

void window::make_current() noexcept
{
    window_impl_ -> make_current();
    current_window.reset(this);
}

void window::resize(unsigned int width, unsigned int height) noexcept { window_impl_ -> resize(width, height); }
void window::title(const std::string& new_title) { window_impl_ -> title(new_title); }

bool window::button_is_pressed(button button_) const noexcept { return window_impl_ -> button_is_pressed(static_cast<unsigned int>(button_)); }

double window::scroll_offset() const noexcept { return window_impl_ -> scroll_offset(); }
bool window::focused() const noexcept { return window_impl_ -> focused(); }

cursor_pos window::cursor_position() const noexcept { return window_impl_ -> cursor_position(); }
void window::cursor_position(cursor_pos cursor_pos_) noexcept { window_impl_ -> cursor_position(cursor_pos_); }

std::string window::clipboard_string() { return window_impl_ -> clipboard_string(); }
void window::clipboard_string(const std::string& new_clipboard_string) { window_impl_ -> clipboard_string(new_clipboard_string); }

unsigned int window::reference_count = 0;
std::unique_ptr<window> window::current_window = nullptr;
