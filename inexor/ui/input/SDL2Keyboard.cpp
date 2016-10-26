#include <SDL.h>
#include <include/cef_base.h>
#include "inexor/ui/input/SDL2Keyboard.hpp"

int getKeyboardModifiers(uint16_t const mod)
{
    int result = EVENTFLAG_NONE;

    if(mod & KMOD_SHIFT)
        result |= EVENTFLAG_SHIFT_DOWN;
    if(mod & KMOD_CTRL)
        result |= EVENTFLAG_CONTROL_DOWN;
    if(mod & KMOD_ALT)
        result |= EVENTFLAG_ALT_DOWN;
    // if(mod & KMOD_GUI) we don't handle the "Windows" button.
    if(mod & KMOD_NUM)
        result |= EVENTFLAG_NUM_LOCK_ON;
    if(mod & KMOD_CAPS)
        result |= EVENTFLAG_CAPS_LOCK_ON;
    return result;
}

void convertKeyCode(SDL_Event &e)
{
    SDL_Keysym &key = e.key.keysym; // key symbol in quasi unicode
    CefKeyEvent cef_event;
    cef_event.modifiers = getKeyboardModifiers(key.mod);

    cef_event.type = e.key.state==SDL_PRESSED ? KEYEVENT_KEYDOWN : KEYEVENT_KEYUP;
}
CefKeyEvent oldkeycode(SDL_Event &e)
{
    bool shift = (e.key.keysym.mod & KMOD_SHIFT) || (e.key.keysym.mod & KMOD_RSHIFT);
    bool ctrl = (e.key.keysym.mod & KMOD_CTRL) || (e.key.keysym.mod & KMOD_RCTRL);
    bool alt = (e.key.keysym.mod & KMOD_ALT) || (e.key.keysym.mod & KMOD_RALT);
    bool num_lock = !(e.key.keysym.mod & KMOD_NUM);
    bool caps_lock = e.key.keysym.mod & KMOD_CAPS;
    int modifiers = getKeyboardModifiers(e.key.keysym.mod);

    // Output codes
    int key_code = 0;
    int char_code = 0;
    bool is_char = false;

    if(e.type == SDL_TEXTINPUT)
    {
     //   spdlog::get("global")->debug() << "text: " << e.text.text << " char[0]: " << (int)e.text.text[0];
        is_char = true;
        if(e.text.text[0] >= 32 && e.text.text[0] <= SDLK_z)
            char_code = e.text.text[0];
        else
            switch(e.text.text[0])
            {
            case '(':
                char_code = JSC_PARENTHESES_LEFT;
                break;
            case ')':
                char_code = JSC_PARENTHESES_RIGHT;
                break;
            case '[':
                char_code = JSC_BRACKET_LEFT;
                break;
            case ']':
                char_code = JSC_BRACKET_RIGHT;
                break;
            case '{':
                char_code = JSC_BRACE_LEFT;
                break;
            case '}':
                char_code = JSC_BRACE_RIGHT;
                break;
            case ',':
                char_code = JSC_COMMA;
                break;
            case ';':
                char_code = JSC_SEMI_COLON;
                break;
            case '-':
                char_code = JSC_SUBTRACT;
                break;
            case '_':
                char_code = JSC_UNDERSCORE;
                break;
            case '#':
                char_code = JSC_HASH;
                break;
            case '\'':
                char_code = JSC_SINGLE_QUOTE;
                break;
            case '+':
                char_code = JSC_PLUS;
                break;
            case '*':
                char_code = JSC_ASTERISK;
                break;
            case '~':
                char_code = JSC_TILDE;
                break;
            case '?':
                char_code = JSC_QUESTION;
                break;
            case '\\':
                char_code = JSC_BACK_SLASH;
                break;
            case '|':
                char_code = JSC_PIPE;
                break;
            default:
                // char_code = e.text.text[0];
                break;
            }
    }
    else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
    {
        //spdlog::get("global")->debug("type: {0} mod: {1} sym: {2} ({3}) scan: {4} ({5})",
        //                             e.key.state, e.key.keysym.mod, e.key.keysym.sym,
        //                             SDL_GetKeyName(e.key.keysym.sym), e.key.keysym.scancode,
        //                             SDL_GetScancodeName(e.key.keysym.scancode));

        // Function Keys
        if(e.key.keysym.sym >= _SDLK_F1 && e.key.keysym.sym <= _SDLK_F12)
        {
            key_code = JSK_F1 + (e.key.keysym.sym - _SDLK_F1);
        }
        // Special Keys
        switch(e.key.keysym.sym)
        {
        case SDLK_BACKSPACE:
            key_code = JSK_BACKSPACE;
            break;
        case SDLK_TAB:
            key_code = JSK_TAB;
            break;
        case SDLK_RETURN:
            is_char = true;
            key_code = JSK_ENTER;
            char_code = JSK_ENTER;
            break;
        case SDLK_PAUSE:
            key_code = JSK_PAUSE;
            break;
        case _SDLK_SCROLL_LOCK:
            key_code = JSK_SCROLL_LOCK;
            break;
        case SDLK_ESCAPE:
            key_code = JSK_ESCAPE;
            break;
        case SDLK_LEFT:
            key_code = JSK_LEFT_ARROW;
            break;
        case SDLK_UP:
            key_code = JSK_UP_ARROW;
            break;
        case SDLK_RIGHT:
            key_code = JSK_RIGHT_ARROW;
            break;
        case SDLK_DOWN:
            key_code = JSK_DOWN_ARROW;
            break;
        case _SDLK_HOME:
            key_code = JSK_HOME;
            break;
        case _SDLK_END:
            key_code = JSK_END;
            break;
        case _SDLK_PAGEUP:
            key_code = JSK_PAGE_UP;
            break;
        case _SDLK_PAGEDOWN:
            key_code = JSK_PAGE_DOWN;
            break;
        case _SDLK_INSERT:
            key_code = JSK_INSERT;
            break;
        case SDLK_DELETE:
            is_char = true;
            key_code = JSK_DELETE;
            char_code = JSC_DELETE;
            break;
        case _SDLK_NUM_DIVIDE:
            is_char = true;
            key_code = JSK_DIVIDE;
            char_code = JSC_NUM_DIVIDE;
            break;
        case _SDLK_NUM_MULTIPLY:
            is_char = true;
            key_code = JSK_MULTIPLY;
            char_code = JSC_NUM_MULTIPLY;
            break;
        case _SDLK_NUM_SUBTRACT:
            is_char = true;
            key_code = JSK_SUBTRACT;
            char_code = JSC_NUM_SUBTRACT;
            break;
        case _SDLK_NUM_ADD:
            is_char = true;
            key_code = JSK_ADD;
            char_code = JSC_NUM_ADD;
            break;
        case _SDLK_NUM_DECIMAL:
            if(num_lock)
            {
                is_char = true;
                key_code = JSK_DECIMAL_POINT; // keyboard layout dependent!
                char_code = JSK_DELETE;
            }
            else
            {
                key_code = JSK_DELETE;
            }
            break;
        case _SDLK_KP0:
            key_code = JSK_INSERT;
            break;
        case _SDLK_KP1:
            key_code = JSK_END;
            break;
        case _SDLK_KP2:
            key_code = JSK_DOWN_ARROW;
            break;
        case _SDLK_KP3:
            key_code = JSK_PAGE_DOWN;
            break;
        case _SDLK_KP4:
            key_code = JSK_LEFT_ARROW;
            break;
        case _SDLK_KP5:
            key_code = JSK_NUM5_SECOND;
            break;
        case _SDLK_KP6:
            key_code = JSK_RIGHT_ARROW;
            break;
        case _SDLK_KP7:
            key_code = JSK_HOME;
            break;
        case _SDLK_KP8:
            key_code = JSK_UP_ARROW;
            break;
        case _SDLK_KP9:
            key_code = JSK_PAGE_UP;
            break;
        case _SDLK_CAPS_LOCK:
            key_code = JSK_CAPS_LOCK;
            break;
        case _SDLK_NUM:
            key_code = JSK_NUM_LOCK;
            break;
        case _SDLK_LCTRL:
        case _SDLK_RCTRL:
            key_code = JSK_CTRL;
            break;
        case _SDLK_LSHIFT:
        case _SDLK_RSHIFT:
            key_code = JSK_SHIFT;
            break;
        case _SDLK_LALT:
            key_code = JSK_LEFT_ALT;
            break;
        case _SDLK_LMETA:
            key_code = JSK_LEFT_WIN;
            break;
        case _SDLK_RALT:
            key_code = JSK_RIGHT_ALT;
            break;
        case _SDLK_RMETA:
            key_code = JSK_RIGHT_WIN;
            break;
        case _SDLK_SELECT:
            key_code = JSK_SELECT;
            break;
        }

    }

    // Still not mapped?
    if(key_code == 0)
    {
        key_code = e.key.keysym.sym;
    }

    // Fire key events to CEF
    if(e.key.state == SDL_PRESSED || e.type == SDL_TEXTINPUT)
    {
        // onkeydown
        CefKeyEvent key_event_key_down;
        key_event_key_down.type = KEYEVENT_KEYDOWN;
        key_event_key_down.modifiers = modifiers;
        key_event_key_down.windows_key_code = key_code;
        return key_event_key_down;
        //layer_manager->SendKeyEvent(key_event_key_down);
        // Fire a second key event for characters only
        if(is_char)
        {
            // onkeypress
            CefKeyEvent key_event_char;
            key_event_char.type = KEYEVENT_CHAR;
            key_event_char.modifiers = modifiers;
            key_event_char.character = char_code;
            return key_event_char;
           // layer_manager->SendKeyEvent(key_event_char);
        }
    }
    else if(e.key.state == SDL_RELEASED)
    {
        // onkeyup
        CefKeyEvent key_event_key_up;
        key_event_key_up.type = KEYEVENT_KEYUP;
        key_event_key_up.modifiers = modifiers;
        key_event_key_up.windows_key_code = key_code;
        return key_event_key_up;
      //  layer_manager->SendKeyEvent(key_event_key_up);
    }
}

void convertKeyCode(const char *input, size_t len)
{
    // if(e.type == SDL_TEXTINPUT) convertKeyCode(e.text.text, 32);
    for(size_t i = 0; i < len; i++)
    {
        CefKeyEvent cef_event;
        cef_event.type = cef_key_event_type_t::KEYEVENT_CHAR;
        cef_event.character = input[i];
        // fire event.
    }
}

int getWindowsKeyCode(SDL_Keysym const &key)
{
    int result = 0;

    bool shift = !!(key.mod & KMOD_SHIFT);
    bool caps = !!(key.mod & KMOD_CAPS);
    bool alt_gr = !!(key.mod & KMOD_RALT);
    bool uppercase = (caps && !shift) || (shift && !caps);

    // mapped from azerty windows 8 asus laptop
    switch(key.sym)
    {
    case SDLK_RETURN:
        result = 13;
        break;
    case SDLK_ESCAPE:
        result = 27;
        break;
    case SDLK_BACKSPACE:
        result = 8;
        break;
    case SDLK_TAB:
        result = 9;
        break;
    case SDLK_SPACE:
        result = 32;
        break;
    case SDLK_EXCLAIM:
        result = uppercase ? 167 : 33; // § : !
        break;

    case SDLK_QUOTEDBL:
        result = 34;
        break;
    case SDLK_HASH:
        result = 35;
        break;
    case SDLK_DOLLAR:
        result = 36;
        break;
    case SDLK_PERCENT:
        result = 37;
        break;
    case SDLK_AMPERSAND:
        result = 38;
        break;
    case SDLK_QUOTE:
        result = 39;
        break;
    case SDLK_LEFTPAREN:
        result = 40;
        break;
    case SDLK_RIGHTPAREN:
        result = alt_gr ? 93 : uppercase ? 176 : 41; // ] ? ° : )
        break;
    case SDLK_ASTERISK:
        result = uppercase ? 181 : 42; // µ : *
        break;
    case SDLK_PLUS:
        result = 43;
        break;
    case SDLK_COMMA:
        result = uppercase ? 63 : 44; // '?' : ,
        break;
    case SDLK_MINUS:
        result = 45;
        break;
    case SDLK_PERIOD:
        result = 46;
        break;
    case SDLK_SLASH:
        result = 47;
        break;

    case SDLK_0:
        result = alt_gr ? 64 : uppercase ? 48 : 224; // @ ? 0 : à
        break;
    case SDLK_1:
        result = uppercase ? 49 : 38; // 1 : & (KO)
        break;
    case SDLK_2:
        result = alt_gr ? 126 : uppercase ? 50 : 233; // ~ ? 2 : é
        break;
    case SDLK_3:
        result = alt_gr ? 35 : uppercase ? 51 : 34; // # ? 3 : "
        break;
    case SDLK_4:
        result = alt_gr ? 123 : uppercase ? 52 : 39; // { ? 4 : '
        break;
    case SDLK_5:
        result = alt_gr ? 91 : uppercase ? 53 : 40; // [ ? 5 : ( (KO)
        break;
    case SDLK_6:
        result = alt_gr ? 124 : uppercase ? 54 : 45; // | ? 6 : -
        break;
    case SDLK_7:
        result = alt_gr ? 96 : uppercase ? 55 : 232; // ` ? 7 : è
        break;
    case SDLK_8:
        result = alt_gr ? 92 : uppercase ? 56 : 95; // \ ? 8 : _
        break;
    case SDLK_9:
        result = alt_gr ? 94 : uppercase ? 57 : 231; // ^ ? 9 : ç
        break;

    case SDLK_COLON:
        result = uppercase ? 47 : 58; // / : :
        break;
    case SDLK_SEMICOLON:
        result = uppercase ? 46 : 59; // . (KO) : ;
        break;
    case SDLK_LESS:
        result = uppercase ? 62 : 60; // > : <
        break;
    case SDLK_EQUALS:
        result = alt_gr ? 125 : uppercase ? 43 : 61; // } ? + : =
        break;
    case SDLK_GREATER:
        result = 62;
        break;
    case SDLK_QUESTION:
        result = 63;
        break;
    case SDLK_AT:
        result = 64;
        break;
    case SDLK_LEFTBRACKET:
        result = 91;
        break;
    case SDLK_BACKSLASH:
        result = 92;
        break;
    case SDLK_RIGHTBRACKET:
        result = 93;
        break;
    case SDLK_CARET:
        result = uppercase ? 168 : 94; // ^ : ¨
        break;
    case SDLK_UNDERSCORE:
        result = 95;
        break;
    case SDLK_BACKQUOTE:
        result = 96;
        break;

    case SDLK_a:
        result = uppercase ? 65 : 97;
        break;
    case SDLK_b:
        result = uppercase ? 66 : 98;
        break;
    case SDLK_c:
        result = uppercase ? 67 : 99;
        break;
    case SDLK_d:
        result = uppercase ? 68 : 100;
        break;
    case SDLK_e:
        result = uppercase ? 69 : 101;
        break;
    case SDLK_f:
        result = uppercase ? 70 : 102;
        break;
    case SDLK_g:
        result = uppercase ? 71 : 103;
        break;
    case SDLK_h:
        result = uppercase ? 72 : 104;
        break;
    case SDLK_i:
        result = uppercase ? 73 : 105;
        break;
    case SDLK_j:
        result = uppercase ? 74 : 106;
        break;
    case SDLK_k:
        result = uppercase ? 75 : 107;
        break;
    case SDLK_l:
        result = uppercase ? 76 : 108;
        break;
    case SDLK_m:
        result = uppercase ? 77 : 109;
        break;
    case SDLK_n:
        result = uppercase ? 78 : 110;
        break;
    case SDLK_o:
        result = uppercase ? 79 : 111;
        break;
    case SDLK_p:
        result = uppercase ? 80 : 112;
        break;
    case SDLK_q:
        result = uppercase ? 81 : 113;
        break;
    case SDLK_r:
        result = uppercase ? 82 : 114;
        break;
    case SDLK_s:
        result = uppercase ? 83 : 115;
        break;
    case SDLK_t:
        result = uppercase ? 84 : 116;
        break;
    case SDLK_u:
        result = uppercase ? 85 : 117;
        break;
    case SDLK_v:
        result = uppercase ? 86 : 118;
        break;
    case SDLK_w:
        result = uppercase ? 87 : 119;
        break;
    case SDLK_x:
        result = uppercase ? 88 : 120;
        break;
    case SDLK_y:
        result = uppercase ? 89 : 121;
        break;
    case SDLK_z:
        result = uppercase ? 90 : 122;
        break;
    }
    return result;
}
