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

#pragma once

#include <array>
#include <memory>
#include <string>

namespace tppi
{
    enum class button : unsigned int
    {
        keyboard_key_space = 32,
        keyboard_key_apostrophe = 39,
        keyboard_comma = 44,
        keyboard_minus = 45,
        keyboard_period = 46,
        keyboard_slash = 47,
        keyboard_zero = 48,
        keyboard_one = 49,
        keyboard_two = 50,
        keyboard_three = 51,
        keyboard_four = 52,
        keyboard_five = 53,
        keyboard_six = 54,
        keyboard_seven = 55,
        keyboard_eight = 56,
        keyboard_nine = 57,
        keyboard_semicolon = 59,
        keyboard_equal = 61,
        keyboard_a = 65,
        keyboard_b = 66,
        keyboard_c = 67,
        keyboard_d = 68,
        keyboard_e = 69,
        keyboard_f = 70,
        keyboard_g = 71,
        keyboard_h = 72,
        keyboard_i = 73,
        keyboard_j = 74,
        keyboard_k = 75,
        keyboard_l = 76,
        keyboard_m = 77,
        keyboard_n = 78,
        keyboard_o = 79,
        keyboard_p = 80,
        keyboard_q = 81,
        keyboard_r = 82,
        keyboard_s = 83,
        keyboard_t = 84,
        keyboard_u = 85,
        keyboard_v = 86,
        keyboard_w = 87,
        keyboard_x = 88,
        keyboard_y = 89,
        keyboard_z = 90,
        keyboard_left_bracket = 91,
        keyboard_backslash = 92,
        keyboard_right_bracket = 93,
        keyboard_grave_accent = 96,

        keyboard_escape = 256,
        keyboard_enter = 257,
        keyboard_tab = 258,
        keyboard_backspace = 259,
        keyboard_insert = 260,
        keyboard_delete = 261,
        keyboard_right = 262,
        keyboard_left = 263,
        keyboard_down = 264,
        keyboard_up = 265,
        keyboard_page_up = 266,
        keyboard_page_down = 267,
        keyboard_home = 268,
        keyboard_end = 269,
        keyboard_caps_lock = 280,
        keyboard_scroll_lock = 281,
        keyboard_num_lock = 282,
        keyboard_print_screen = 283,
        keyboard_pause = 284,

        keyboard_f1 = 290,
        keyboard_f2 = 291,
        keyboard_f3 = 292,
        keyboard_f4 = 293,
        keyboard_f5 = 294,
        keyboard_f6 = 295,
        keyboard_f7 = 296,
        keyboard_f8 = 297,
        keyboard_f9 = 298,
        keyboard_f10 = 299,
        keyboard_f11 = 300,
        keyboard_f12 = 301,
        keyboard_f13 = 302,
        keyboard_f14 = 303,
        keyboard_f15 = 304,
        keyboard_f16 = 305,
        keyboard_f17 = 306,
        keyboard_f18 = 307,
        keyboard_f19 = 308,
        keyboard_f20 = 309,
        keyboard_f21 = 310,
        keyboard_f22 = 311,
        keyboard_f23 = 312,
        keyboard_f24 = 313,
        keyboard_f25 = 314,

        keyboard_keypad_zero = 320,
        keyboard_keypad_one = 321,
        keyboard_keypad_two = 322,
        keyboard_keypad_three = 323,
        keyboard_keypad_four = 324,
        keyboard_keypad_five = 325,
        keyboard_keypad_six = 326,
        keyboard_keypad_seven = 327,
        keyboard_keypad_eight = 328,
        keyboard_keypad_nine = 329,
        keyboard_keypad_decimal_point = 330,
        keyboard_keypad_divide = 331,
        keyboard_keypad_multiply = 332,
        keyboard_keypad_subtract = 333,
        keyboard_keypad_add = 334,
        keyboard_keypad_enter = 335,
        keyboard_keypad_equal = 336,

        keyboard_left_shift = 340,
        keyboard_left_control = 341,
        keyboard_left_alt = 342,
        keyboard_left_super = 343,
        keyboard_right_shift = 344,
        keyboard_right_control = 345,
        keyboard_right_alt = 346,
        keyboard_right_super = 347,
        keyboard_menu = 348,

        mouse_one = 1001,
        mouse_two = 1002,
        mouse_three = 1003,

        mouse_four = 1004,
        mouse_five = 1005,
        mouse_six = 1006,
        mouse_seven = 1007,
        mouse_eight = 1008
    };

    struct cursor_pos
    {
        double x, y;
    };

    class window
    {
    public:
        window(unsigned int width, unsigned int height, const std::string& title, bool fullscreen = false, bool resizable = false);
        ~window();

        void make_current() noexcept;
        void resize() noexcept;
        void set_title(std::string& new_title);

        bool button_is_pressed(button button_) const noexcept;

        double scroll_offset() const noexcept;
        bool is_in_focus() const noexcept;

        cursor_pos cursor_position() const noexcept;
        void set_cursor_position(cursor_pos cursor_pos_) noexcept;

        std::string clipboard_string();
        void set_clipboard_string(const std::string& new_clipboard_string);
    private:
        class window_impl;
        std::unique_ptr<window_impl> window_impl_;

        static unsigned int reference_count;
        static std::unique_ptr<window> current_window;

        static void set_scroll_offset(double new_scroll_wheel_value) noexcept;
        static void set_focus_flag(bool is_window_in_focus) noexcept;

        static double scroll_wheel_value_;
        static bool focus_flag;
    };
}
