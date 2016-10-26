#include "inexor/ui/input/InexorKeyboardManager.hpp"
#include "inexor/ui/input/SDL2Keyboard.hpp"

#include <SDL.h>
#include <include/cef_base.h>

namespace inexor {
namespace ui {
namespace input {

bool InexorKeyboardManager::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    CEF_REQUIRE_RENDERER_THREAD();
    return false;
}

bool InexorKeyboardManager::Get(const CefString& name, const CefRefPtr<CefV8Value> object, CefRefPtr<CefV8Value>& return_value, CefString& exception)
{
    CEF_REQUIRE_RENDERER_THREAD();
    return false;
}

bool InexorKeyboardManager::Set(const CefString& name, const CefRefPtr<CefV8Value> object, const CefRefPtr<CefV8Value> value, CefString& exception)
{
    CEF_REQUIRE_RENDERER_THREAD();
    return false;
}

void InexorKeyboardManager::SendKeyEvent(SDL_Event &e)
{
    CefKeyEvent old_cef_event = oldkeycode(e); // this is the old event handling we have it here for the debugger, but unused.

    if(e.type == SDL_KEYDOWN)
    {
        CefKeyEvent cef_event;
        cef_event.modifiers = getKeyboardModifiers(e.key.keysym.mod);
        cef_event.windows_key_code = getWindowsKeyCode(e.key.keysym);

        cef_event.type = KEYEVENT_RAWKEYDOWN; // or shall we send keydown?
        layer_manager->SendKeyEvent(cef_event);

        cef_event.type = KEYEVENT_CHAR;
        layer_manager->SendKeyEvent(cef_event);
        return;
    }
    if(e.type == SDL_KEYUP)
    {
        CefKeyEvent cef_event;
        cef_event.modifiers = getKeyboardModifiers(e.key.keysym.mod);
        cef_event.windows_key_code = getWindowsKeyCode(e.key.keysym);

        cef_event.type = KEYEVENT_KEYUP;
        layer_manager->SendKeyEvent(cef_event);
        return;
    }
    if(e.type == SDL_TEXTINPUT) // SDL gives us more than one char as string, but individual key strikes as such.
    {
        //for(size_t i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; i++)
        //{
        //    char &curchar = e.text.text[i];
        //    CefKeyEvent cef_event;
        //    cef_event.type = cef_key_event_type_t::KEYEVENT_CHAR;
        //    cef_event.character = curchar;
        //    unsigned char windows_scan_code = keycode::HID_to_win_native(e.key.state);
        //    cef_event.modifiers = getKeyboardModifiers(e.key.keysym.mod);
        //    cef_event.windows_key_code = windows_scan_code;
        //    layer_manager->SendKeyEvent(cef_event); // fire event
        //    if(curchar == '\0') break;
        //}
        return; // we dont handle it atm.. seems not necessary or I understand it wrong.
    }


    //if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
    //{
    //    SDL_Keysym &key = e.key.keysym; // key symbol in quasi unicode
    //    CefKeyEvent cef_event;
        // cef **always** wants us to give it the **native windows** key code.
        // unsigned char windows_scan_code = keycode::HID_to_win_native(key.sym);
        // cef_event.windows_key_code = (windows_scan_code << 16) | 1; // key scan code << 16 | key repeat count

        // on linux and mac however it could require us to **additionally** give it the native codes.. check this if things dont work.
        // cef_event.native_key_code

        //if(e.type == SDL_KEYDOWN && key.sym >= SDLK_SPACE && key.sym <= SDLK_z)
        //{ //fire event for a _chararacter_. but since two signals come: just one for keydown.
        //    cef_event.character = key.sym;
        //    cef_event.unmodified_character = key.sym;
        //    cef_event.windows_key_code = key.sym;
        //    cef_event.type = KEYEVENT_CHAR;
        //    layer_manager->SendKeyEvent(cef_event);
        //}
        //else layer_manager->SendKeyEvent(cef_event); // fire event for non-character
    //    return;
    //}
}

bool InexorKeyboardManager::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& key_event, CefEventHandle os_event, bool* is_keyboard_shortcut) {
    CEF_REQUIRE_UI_THREAD();
    spdlog::get("global")->debug("InexorCefKeyboardManager::OnPreKeyEvent: key_event.type: {0} native_key_code: {1} windows_key_code: {2} is_system_key: {3}",
                                 key_event.type, key_event.native_key_code, key_event.windows_key_code, key_event.is_system_key);
    return false;
}

bool InexorKeyboardManager::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& key_event, CefEventHandle os_event) {
    CEF_REQUIRE_UI_THREAD();
    spdlog::get("global")->debug("InexorCefKeyboardManager::OnKeyEvent: key_event.type: {0} native_key_code: {1} windows_key_code: {2} is_system_key: {3}",
                                 key_event.type, key_event.native_key_code, key_event.windows_key_code, key_event.is_system_key);
    return false;
}

}
}
}
