#pragma once
#include <chrono>
#include <Signal.h>
#include <vector>
#include <glm/glm.hpp>

//TO DO: Button update is currently looping over every button, maybe add a cache for used buttons to lower overhead
//TO DO: [DONE BUT TO BE TESTED] Handle joystick connection/disconnection, axis, buttons and id based input capturing 
//TO DO: Offer Mouse Horizontal Scroll Support

enum ButtonExpression : int
{
    NOT_PRESSED = 0,
    PRESSED = 1,
    TAPPED = 2,
    HELD = 4,
    RELEASED = 8,
    DOUBLE_TAPPED = 16
};

  
struct Button
{
    ButtonExpression expression = ButtonExpression::NOT_PRESSED;
    std::chrono::high_resolution_clock::time_point last_press_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point last_release_time = std::chrono::high_resolution_clock::now();
    //std::chrono::milliseconds hold_duration;
    //std::chrono::milliseconds double_tap_threshold; // max interval for double tap
};

struct Keyboard
{
    Button keys[106];
};

struct Axis
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Mouse
{
    int position_x = 0;
    int position_y = 0;

    int motion_x = 0;
    int motion_y = 0;

    int current_wheel = 0;
    int last_frame_wheel = 0;

    Button buttons[5];
};

struct Controller
{
    int id = 0;
    bool is_connected = false;
    Button buttons[15];
    Axis axis_left;
    Axis axis_right;
    //Trigger Axis have x and y as same value as triggers move only in 1 direction
    Axis trigger_left;
    Axis trigger_right;
};

struct InputManager
{
    Mouse mouse;

    Keyboard keyboard;

    Controller controllers[4];

    int controllerAxisDeadZone = 8000;

    long tap_duration = 200;
    long hold_duration = 500;

    bool isCursorHidden = false;
    bool isCursorBoundToWindow = false;

};


namespace InputFunctions
{
    static unsigned int KeyToIndex(SDL_Keycode p_key) {
        switch (p_key) {
        case SDLK_a: return 0;
        case SDLK_b: return 1;
        case SDLK_c: return 2;
        case SDLK_d: return 3;
        case SDLK_e: return 4;
        case SDLK_f: return 5;
        case SDLK_g: return 6;
        case SDLK_h: return 7;
        case SDLK_i: return 8;
        case SDLK_j: return 9;
        case SDLK_k: return 10;
        case SDLK_l: return 11;
        case SDLK_m: return 12;
        case SDLK_n: return 13;
        case SDLK_o: return 14;
        case SDLK_p: return 15;
        case SDLK_q: return 16;
        case SDLK_r: return 17;
        case SDLK_s: return 18;
        case SDLK_t: return 19;
        case SDLK_u: return 20;
        case SDLK_v: return 21;
        case SDLK_w: return 22;
        case SDLK_x: return 23;
        case SDLK_y: return 24;
        case SDLK_z: return 25;
        case SDLK_0: return 26;
        case SDLK_1: return 27;
        case SDLK_2: return 28;
        case SDLK_3: return 29;
        case SDLK_4: return 30;
        case SDLK_5: return 31;
        case SDLK_6: return 32;
        case SDLK_7: return 33;
        case SDLK_8: return 34;
        case SDLK_9: return 35;
        case SDLK_ESCAPE: return 36;
        case SDLK_F1: return 37;
        case SDLK_F2: return 38;
        case SDLK_F3: return 39;
        case SDLK_F4: return 40;
        case SDLK_F5: return 41;
        case SDLK_F6: return 42;
        case SDLK_F7: return 43;
        case SDLK_F8: return 44;
        case SDLK_F9: return 45;
        case SDLK_F10: return 46;
        case SDLK_F11: return 47;
        case SDLK_F12: return 48;
        case SDLK_RETURN: return 49;
        case SDLK_SPACE: return 50;
        case SDLK_BACKSPACE: return 51;
        case SDLK_TAB: return 52;
        case SDLK_LSHIFT: return 53;
        case SDLK_RSHIFT: return 54;
        case SDLK_LCTRL: return 55;
        case SDLK_RCTRL: return 56;
        case SDLK_LALT: return 57;
        case SDLK_RALT: return 58;
        case SDLK_DELETE: return 59;
        case SDLK_INSERT: return 60;
        case SDLK_HOME: return 61;
        case SDLK_END: return 62;
        case SDLK_PAGEUP: return 63;
        case SDLK_PAGEDOWN: return 64;
        case SDLK_UP: return 65;
        case SDLK_DOWN: return 66;
        case SDLK_LEFT: return 67;
        case SDLK_RIGHT: return 68;
        case SDLK_PERIOD: return 69;
        case SDLK_COMMA: return 70;
        case SDLK_QUOTE: return 71;
        case SDLK_SEMICOLON: return 72;
        case SDLK_SLASH: return 73;
        case SDLK_BACKSLASH: return 74;
        case SDLK_MINUS: return 75;
        case SDLK_EQUALS: return 76;
        case SDLK_LEFTBRACKET: return 77;
        case SDLK_RIGHTBRACKET: return 78;
        case SDLK_BACKQUOTE: return 79;
        case SDLK_CAPSLOCK: return 80;
        case SDLK_NUMLOCKCLEAR: return 81;
        case SDLK_SCROLLLOCK: return 82;
        case SDLK_LGUI: return 83;
        case SDLK_RGUI: return 84;
        case SDLK_APPLICATION: return 85;
        case SDLK_PRINTSCREEN: return 86;
        case SDLK_PAUSE: return 87;
        case SDLK_KP_0: return 88;
        case SDLK_KP_1: return 89;
        case SDLK_KP_2: return 90;
        case SDLK_KP_3: return 91;
        case SDLK_KP_4: return 92;
        case SDLK_KP_5: return 93;
        case SDLK_KP_6: return 94;
        case SDLK_KP_7: return 95;
        case SDLK_KP_8: return 96;
        case SDLK_KP_9: return 97;
        case SDLK_KP_DIVIDE: return 98;
        case SDLK_KP_MULTIPLY: return 99;
        case SDLK_KP_MINUS: return 100;
        case SDLK_KP_PLUS: return 101;
        case SDLK_KP_ENTER: return 102;
        case SDLK_KP_DECIMAL: return 103;
        case SDLK_KP_EQUALS: return 104;
        case SDLK_KP_CLEAR: return 105;
        default: return 0;
        }
    }

    static SDL_Keycode IndexToKey(unsigned int p_id) {
        switch (p_id) {
        case 0: return SDLK_a;
        case 1: return SDLK_b;
        case 2: return SDLK_c;
        case 3: return SDLK_d;
        case 4: return SDLK_e;
        case 5: return SDLK_f;
        case 6: return SDLK_g;
        case 7: return SDLK_h;
        case 8: return SDLK_i;
        case 9: return SDLK_j;
        case 10: return SDLK_k;
        case 11: return SDLK_l;
        case 12: return SDLK_m;
        case 13: return SDLK_n;
        case 14: return SDLK_o;
        case 15: return SDLK_p;
        case 16: return SDLK_q;
        case 17: return SDLK_r;
        case 18: return SDLK_s;
        case 19: return SDLK_t;
        case 20: return SDLK_u;
        case 21: return SDLK_v;
        case 22: return SDLK_w;
        case 23: return SDLK_x;
        case 24: return SDLK_y;
        case 25: return SDLK_z;
        case 26: return SDLK_0;
        case 27: return SDLK_1;
        case 28: return SDLK_2;
        case 29: return SDLK_3;
        case 30: return SDLK_4;
        case 31: return SDLK_5;
        case 32: return SDLK_6;
        case 33: return SDLK_7;
        case 34: return SDLK_8;
        case 35: return SDLK_9;
        case 36: return SDLK_ESCAPE;
        case 37: return SDLK_F1;
        case 38: return SDLK_F2;
        case 39: return SDLK_F3;
        case 40: return SDLK_F4;
        case 41: return SDLK_F5;
        case 42: return SDLK_F6;
        case 43: return SDLK_F7;
        case 44: return SDLK_F8;
        case 45: return SDLK_F9;
        case 46: return SDLK_F10;
        case 47: return SDLK_F11;
        case 48: return SDLK_F12;
        case 49: return SDLK_RETURN;
        case 50: return SDLK_SPACE;
        case 51: return SDLK_BACKSPACE;
        case 52: return SDLK_TAB;
        case 53: return SDLK_LSHIFT;
        case 54: return SDLK_RSHIFT;
        case 55: return SDLK_LCTRL;
        case 56: return SDLK_RCTRL;
        case 57: return SDLK_LALT;
        case 58: return SDLK_RALT;
        case 59: return SDLK_DELETE;
        case 60: return SDLK_INSERT;
        case 61: return SDLK_HOME;
        case 62: return SDLK_END;
        case 63: return SDLK_PAGEUP;
        case 64: return SDLK_PAGEDOWN;
        case 65: return SDLK_UP;
        case 66: return SDLK_DOWN;
        case 67: return SDLK_LEFT;
        case 68: return SDLK_RIGHT;
        case 69: return SDLK_PERIOD;
        case 70: return SDLK_COMMA;
        case 71: return SDLK_QUOTE;
        case 72: return SDLK_SEMICOLON;
        case 73: return SDLK_SLASH;
        case 74: return SDLK_BACKSLASH;
        case 75: return SDLK_MINUS;
        case 76: return SDLK_EQUALS;
        case 77: return SDLK_LEFTBRACKET;
        case 78: return SDLK_RIGHTBRACKET;
        case 79: return SDLK_BACKQUOTE;
        case 80: return SDLK_CAPSLOCK;
        case 81: return SDLK_NUMLOCKCLEAR;
        case 82: return SDLK_SCROLLLOCK;
        case 83: return SDLK_LGUI;
        case 84: return SDLK_RGUI;
        case 85: return SDLK_APPLICATION;
        case 86: return SDLK_PRINTSCREEN;
        case 87: return SDLK_PAUSE;
        case 88: return SDLK_KP_0;
        case 89: return SDLK_KP_1;
        case 90: return SDLK_KP_2;
        case 91: return SDLK_KP_3;
        case 92: return SDLK_KP_4;
        case 93: return SDLK_KP_5;
        case 94: return SDLK_KP_6;
        case 95: return SDLK_KP_7;
        case 96: return SDLK_KP_8;
        case 97: return SDLK_KP_9;
        case 98: return SDLK_KP_DIVIDE;
        case 99: return SDLK_KP_MULTIPLY;
        case 100: return SDLK_KP_MINUS;
        case 101: return SDLK_KP_PLUS;
        case 102: return SDLK_KP_ENTER;
        case 103: return SDLK_KP_DECIMAL;
        case 104: return SDLK_KP_EQUALS;
        case 105: return SDLK_KP_CLEAR;
        default: return SDLK_UNKNOWN;
        }
    }

    static unsigned int MouseButtonToIndex(Uint8 p_button) {
        switch (p_button) {
        case SDL_BUTTON_LEFT: return 0;
        case SDL_BUTTON_MIDDLE: return 1;
        case SDL_BUTTON_RIGHT: return 2;
        case SDL_BUTTON_X1: return 3;
        case SDL_BUTTON_X2: return 4;
        default: return 0;
        }
    }

    static Uint8 IndexToMouseButton(unsigned int p_id) {
        switch (p_id) {
        case 0: return SDL_BUTTON_LEFT;
        case 1: return SDL_BUTTON_MIDDLE;
        case 2: return SDL_BUTTON_RIGHT;
        case 3: return SDL_BUTTON_X1;
        case 4: return SDL_BUTTON_X2;
        default: return 0;  // Return 0 for invalid button
        }
    }

    static unsigned int ControllerButtonToIndex(Uint8 p_button) {
        switch (p_button) {
        case SDL_CONTROLLER_BUTTON_A: return 0;
        case SDL_CONTROLLER_BUTTON_B: return 1;
        case SDL_CONTROLLER_BUTTON_X: return 2;
        case SDL_CONTROLLER_BUTTON_Y: return 3;
        case SDL_CONTROLLER_BUTTON_BACK: return 4;
        case SDL_CONTROLLER_BUTTON_GUIDE: return 5;
        case SDL_CONTROLLER_BUTTON_START: return 6;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: return 7;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return 8;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return 9;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return 10;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: return 11;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return 12;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return 13;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return 14;
        default: return 0;
        }
    }

    static Uint8 IndexToControllerButton(unsigned int p_id) {
        switch (p_id) {
        case 0: return SDL_CONTROLLER_BUTTON_A;
        case 1: return SDL_CONTROLLER_BUTTON_B;
        case 2: return SDL_CONTROLLER_BUTTON_X;
        case 3: return SDL_CONTROLLER_BUTTON_Y;
        case 4: return SDL_CONTROLLER_BUTTON_BACK;
        case 5: return SDL_CONTROLLER_BUTTON_GUIDE;
        case 6: return SDL_CONTROLLER_BUTTON_START;
        case 7: return SDL_CONTROLLER_BUTTON_LEFTSTICK;
        case 8: return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
        case 9: return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        case 10: return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case 11: return SDL_CONTROLLER_BUTTON_DPAD_UP;
        case 12: return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        case 13: return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        case 14: return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        default: return SDL_CONTROLLER_BUTTON_INVALID;  // Return invalid button if ID is out of range
        }
    }

    static unsigned int AxisToIndex(SDL_GameControllerAxis p_axis) {
        switch (p_axis) {
        case SDL_CONTROLLER_AXIS_LEFTX: return 0;
        case SDL_CONTROLLER_AXIS_LEFTY: return 1;
        case SDL_CONTROLLER_AXIS_RIGHTX: return 2;
        case SDL_CONTROLLER_AXIS_RIGHTY: return 3;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return 4;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return 5;
        default: return 0;
        }
    }

    static SDL_GameControllerAxis IndexToAxis(unsigned int p_id) {
        switch (p_id) {
        case 0: return SDL_CONTROLLER_AXIS_LEFTX;
        case 1: return SDL_CONTROLLER_AXIS_LEFTY;
        case 2: return SDL_CONTROLLER_AXIS_RIGHTX;
        case 3: return SDL_CONTROLLER_AXIS_RIGHTY;
        case 4: return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
        case 5: return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
        default: return SDL_CONTROLLER_AXIS_INVALID;  // Return invalid axis if ID is out of range
        }
    }

    void AddExpressionState(Button& p_button, ButtonExpression p_expression)
    {
        p_button.expression = static_cast<ButtonExpression>(p_button.expression | p_expression);
    }

    void RemoveExpressionState(Button& p_button, ButtonExpression p_expression)
    {
        p_button.expression = static_cast<ButtonExpression>(p_button.expression & ~p_expression);
    }

    bool HasExpressionState(Button& p_button, ButtonExpression p_expression)
    {
        return (p_button.expression & p_expression) != 0;
    }

    void ButtonPress(Button& p_btn, InputManager& p_input_manager)
    {
        AddExpressionState(p_btn, ButtonExpression::PRESSED);
        RemoveExpressionState(p_btn, ButtonExpression::RELEASED);
        std::cout << "pressed" << "\n";

        //p_btn.buttonStateSignal.Emit(p_btn.expression);

        auto elapsedPressTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - p_btn.last_press_time);
        auto elapsedReleaseTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - p_btn.last_release_time);

        if (HasExpressionState(p_btn, ButtonExpression::TAPPED) && elapsedPressTime > std::chrono::milliseconds(p_input_manager.tap_duration))
        {
            RemoveExpressionState(p_btn, ButtonExpression::TAPPED);
            RemoveExpressionState(p_btn, ButtonExpression::DOUBLE_TAPPED);
        }


        p_btn.last_press_time = std::chrono::high_resolution_clock::now();

    }

    void ButtonRelease(Button& p_btn, InputManager& p_input_manager)
    {
        AddExpressionState(p_btn, ButtonExpression::RELEASED);
        RemoveExpressionState(p_btn, ButtonExpression::PRESSED);

        auto elapsed_press_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - p_btn.last_press_time);

        if (elapsed_press_time < std::chrono::milliseconds(p_input_manager.tap_duration))
        {
            if (!HasExpressionState(p_btn, ButtonExpression::TAPPED))
            {
                AddExpressionState(p_btn, ButtonExpression::TAPPED); std::cout << "tap" << "\n";
            }
            else
            {
                RemoveExpressionState(p_btn, ButtonExpression::TAPPED);
                AddExpressionState(p_btn, ButtonExpression::DOUBLE_TAPPED); std::cout << "DDDTAp" << "\n";
            }

        }
        else
        {
            RemoveExpressionState(p_btn, ButtonExpression::TAPPED);
            RemoveExpressionState(p_btn, ButtonExpression::DOUBLE_TAPPED);
        }

        if (HasExpressionState(p_btn, ButtonExpression::HELD))
        {
            std::cout << "Hold Release" << "\n";
        }

        RemoveExpressionState(p_btn, ButtonExpression::HELD);

        p_btn.last_release_time = std::chrono::high_resolution_clock::now();
    }

    static void ButtonUpdate(Button& p_btn, InputManager& p_input_manager)
    {
        if (HasExpressionState(p_btn, ButtonExpression::PRESSED))
        {
            auto elapsed_press_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - p_btn.last_press_time);
            auto elapsed_release_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - p_btn.last_release_time);

            if (elapsed_press_time > std::chrono::milliseconds(p_input_manager.hold_duration) && !HasExpressionState(p_btn, ButtonExpression::HELD))
            {
                AddExpressionState(p_btn, ButtonExpression::HELD);
                std::cout << "Hold Begin" << "\n";
            }

        }
    }


    // Initialize all connected controllers on startup
    static void InitializeConnectedControllers(InputManager& p_input_manager) {
        int num_joysticks = SDL_NumJoysticks();
        int controller_count = 0;

        for (int i = 0; i < num_joysticks && controller_count < 4; ++i) {
            if (SDL_IsGameController(i)) {
                SDL_GameController* controller = SDL_GameControllerOpen(i);
                if (controller) {
                    p_input_manager.controllers[controller_count].id = i;
                    controller_count++;
                    std::cout << "Controller initialized with ID: " << i << "\n";
                }
            }
        }
    }

    // Handle a new controller being connected
    static void OnControllerAdded(int p_device_index, InputManager& p_input_manager) {
        int controller_id = -1;

        // Find an available slot in InputManager's controllers array
        for (int i = 0; i < 4; ++i) {
            if (p_input_manager.controllers[i].id >= 0) { 
                controller_id = i;
                break;
            }
        }

        if (controller_id != -1 && SDL_IsGameController(p_device_index)) {
            SDL_GameController* controller = SDL_GameControllerOpen(p_device_index);
            if (controller) {
                p_input_manager.controllers[controller_id].id = p_device_index;
                p_input_manager.controllers[controller_id].is_connected = true;
                std::cout << "Controller added with ID: " << p_device_index << "\n";
            }
        }
    }

    // Handle a controller being removed
    static void OnControllerRemoved(int p_controller_id, InputManager& p_input_manager) {
        SDL_GameControllerClose(SDL_GameControllerFromInstanceID(p_controller_id));
        p_input_manager.controllers[p_controller_id].id = -1;  // Mark as unassigned with negative ID
        p_input_manager.controllers[p_controller_id].is_connected = false;
        std::cout << "Controller removed with ID: " << p_controller_id << "\n";
    }

    // Update function to process SDL events for controller add/remove
    static void HandleControllerEvents(SDL_Event& p_event, InputManager& p_input_manager) {
        switch (p_event.type) {
        case SDL_CONTROLLERDEVICEADDED:
            OnControllerAdded(p_event.cdevice.which, p_input_manager);
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            OnControllerRemoved(p_event.cdevice.which, p_input_manager);
            break;

        default:
            break;
        }
    }

    static void UpdateInputStatesPrePoll(InputManager& p_input_manager)
    {

        p_input_manager.mouse.motion_x = 0;
        p_input_manager.mouse.motion_y = 0;
    }

    static void UpdateInputStatesPostPoll(InputManager& p_input_manager)
    {

        //Reset Wheel Value Frame AfterMove
        if(p_input_manager.mouse.current_wheel != 0 && p_input_manager.mouse.last_frame_wheel != 0)
        {
            p_input_manager.mouse.current_wheel = 0;
            //std::cout << "Reset MouseWheel" << '\n';
        }
        
        p_input_manager.mouse.last_frame_wheel = p_input_manager.mouse.current_wheel;
        

        for (auto& btn : p_input_manager.mouse.buttons) {
            InputFunctions::ButtonUpdate(btn, p_input_manager);
        }

        for (auto& btn : p_input_manager.keyboard.keys) {
            InputFunctions::ButtonUpdate(btn, p_input_manager);
        }

        for (size_t i = 0; i < 4; i++)
        {
            if(p_input_manager.controllers[i].is_connected)
            {
                for (auto& btn : p_input_manager.controllers[i].buttons)
                {
                    InputFunctions::ButtonUpdate(btn, p_input_manager);
                }
            }

        }

    }

    static void UpdateInputEvent(SDL_Event& p_event, InputManager& p_input_manager)
    {
        bool mouse_button_down = p_event.type == SDL_MOUSEBUTTONDOWN;
        bool mouse_button_input = (mouse_button_down || p_event.type == SDL_MOUSEBUTTONUP);

        bool mouse_wheel = p_event.type == SDL_MOUSEWHEEL;

        bool key_pressed_down = p_event.type == SDL_KEYDOWN;
        bool key_input = (key_pressed_down || p_event.type == SDL_KEYUP);

        bool mouse_motion = p_event.type == SDL_MOUSEMOTION;

        bool controller_button_down = p_event.type == SDL_CONTROLLERBUTTONDOWN;
        bool controller_input = (controller_button_down || p_event.type == SDL_CONTROLLERBUTTONUP);

        bool axis_motion = false;
        if((p_event.type == SDL_CONTROLLERAXISMOTION) && glm::abs(p_event.caxis.value) > p_input_manager.controllerAxisDeadZone)
        {
            axis_motion = true;
        }

        if(!mouse_button_input && !mouse_wheel && !key_input && !mouse_motion && !controller_input && !axis_motion)
        {
            return;
        }

        if (mouse_motion)
        {
            SDL_GetMouseState(&p_input_manager.mouse.position_x, &p_input_manager.mouse.position_y);

            p_input_manager.mouse.motion_x = p_event.motion.xrel;
            p_input_manager.mouse.motion_y = p_event.motion.yrel;
        }
        else
        {
            if (mouse_button_input)
            {
                bool is_down = mouse_button_down;

                int internal__array_index = MouseButtonToIndex(p_event.button.button);

                if (is_down)
                {
                    InputFunctions::ButtonPress(p_input_manager.mouse.buttons[internal__array_index], p_input_manager);
                    //std::cout << p_input_manager.mouse.position_x << " | " << p_input_manager.mouse.position_y << '\n';
                }
                else
                {
                    InputFunctions::ButtonRelease(p_input_manager.mouse.buttons[internal__array_index], p_input_manager);
                    //std::cout << p_input_manager.mouse.motion_x << " | " << p_input_manager.mouse.motion_y << '\n';
                }

            }else if(mouse_wheel)
            {
                
                //int scroll_x = p_event.wheel.x;  // Horizontal scroll amount

                int scroll_y = p_event.wheel.y;

                p_input_manager.mouse.current_wheel = scroll_y;

                if(scroll_y > 0)
                {
                    std::cout << scroll_y << "UP" << '\n';
                }

                if (scroll_y < 0) 
                {
                    std::cout <<  scroll_y << "DOWN" << '\n';
                }

            }
            else if (key_input)
            {
                bool is_down = key_pressed_down;

                int internal__array_index = KeyToIndex(p_event.key.keysym.sym);

                if(is_down && HasExpressionState(p_input_manager.keyboard.keys[internal__array_index], ButtonExpression::PRESSED))
                {
                    return;
                }

                if (is_down)
                {
                    InputFunctions::ButtonPress(p_input_manager.keyboard.keys[internal__array_index], p_input_manager);
                }
                else
                {
                    InputFunctions::ButtonRelease(p_input_manager.keyboard.keys[internal__array_index], p_input_manager);
                }
            }
            else if (controller_input)
            {
                bool is_down = controller_button_down;

                int internal__array_index = ControllerButtonToIndex(p_event.cbutton.button);
                int controller_id = p_event.cbutton.which;

                if (is_down)
                {
                    InputFunctions::ButtonPress(p_input_manager.controllers[controller_id].buttons[internal__array_index], p_input_manager);
                }
                else
                {
                    InputFunctions::ButtonRelease(p_input_manager.controllers[controller_id].buttons[internal__array_index], p_input_manager);
                }
            }else if(axis_motion)
            {
                int controller_id = p_event.cbutton.which;

                int axis = p_event.caxis.axis;
                int value = p_event.caxis.value;

                Controller* ctrl = &p_input_manager.controllers[controller_id];

                ctrl->axis_left.x = 0;
                ctrl->axis_left.y = 0;
                ctrl->axis_right.x = 0;
                ctrl->axis_right.y = 0;
                ctrl->trigger_left.x = 0;
                ctrl->trigger_left.y = 0;
                ctrl->trigger_right.x = 0;
                ctrl->trigger_right.y = 0;

                if(axis == SDL_CONTROLLER_AXIS_LEFTX)
                {
                    ctrl->axis_left.x = value;
                    std::cout << "LX : " << ctrl->axis_left.x << " LY : " << ctrl->axis_left.y << " RX :" << ctrl->axis_right.x << " RY :" << ctrl->axis_right.y << "\n";
                }
                else if (axis == SDL_CONTROLLER_AXIS_LEFTY) {
                    ctrl->axis_left.y = value;
                    std::cout << "LX : " << ctrl->axis_left.x << " LY : " << ctrl->axis_left.y << " RX :" << ctrl->axis_right.x << " RY :" << ctrl->axis_right.y << "\n";
                }
                else if (axis == SDL_CONTROLLER_AXIS_RIGHTX) {
                    ctrl->axis_right.x = value;
                    std::cout << "LX : " << ctrl->axis_left.x << " LY : " << ctrl->axis_left.y << " RX :" << ctrl->axis_right.x << " RY :" << ctrl->axis_right.y << "\n";
                }
                else if (axis == SDL_CONTROLLER_AXIS_RIGHTY) {
                    ctrl->axis_right.y = value;
                    std::cout << "LX : " << ctrl->axis_left.x << " LY : " << ctrl->axis_left.y << " RX :" << ctrl->axis_right.x << " RY :" << ctrl->axis_right.y << "\n";
                }
                else if (axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
                    ctrl->trigger_left.x = value;
                    ctrl->trigger_left.y = value;
                }
                else if (axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                    ctrl->trigger_right.x = value;
                    ctrl->trigger_right.y = value;
                }
                
                
               
               
            }
        }
    }


    static void MoveMouseToPositionInWindow(SDL_Window* p_window_ptr,int p_x,int p_y)
    {
        SDL_WarpMouseInWindow(p_window_ptr, p_x, p_y);
    }

    static void MoveMouseToCentreWhenOutOfWindowBounds(int p_width, int p_height, SDL_Window* p_window_ptr, InputManager& p_inputManager)
    {
        // Check if the mouse is outside the window
        if (p_inputManager.mouse.position_x < 0 || p_inputManager.mouse.position_x >= p_width || p_inputManager.mouse.position_y < 0 || p_inputManager.mouse.position_y >= p_height) {
            // Warp mouse to the center of the window
            MoveMouseToPositionInWindow(p_window_ptr, p_width / 2, p_height / 2);
        }
    }
}