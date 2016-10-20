#include "inexor/ui/CefSubsystem.hpp"
#include "inexor/ui/cefsettings.hpp"

using namespace inexor::util;

CefRefPtr<InexorCefApp> cef_app;

// TODO: Namespace; encapsulate in the class; damint

SUBSYSTEM_REGISTER(cef, inexor::ui::CefSubsystem);

namespace inexor {
namespace ui {

CefSubsystem::CefSubsystem() {
    ::cef_app = new InexorCefApp(1024,1024);

    const CefMainArgs args;
    int exit_code = CefExecuteProcess(args, cef_app, NULL);
    if (exit_code >= 0) {
        /*
        std::string msg = fmt << "Forking the CEF process "
            << "yielded a non zero exit code: "
            << exit_code << ".";
        */
        std::string msg = "Forking the CEF process yielded a non zero exit code";
        throw CefProcessException(msg);
    }

    tick();
}

CefSubsystem::~CefSubsystem() {
    ::cef_app->Destroy();

    // Make sure any outstanding IPC messages are processed
    tick();

    // Stop any forked processes
    CefShutdown();

    // Destroy the cef app
    ::cef_app = nullptr;
}

void CefSubsystem::tick() {
    CefDoMessageLoopWork();
}

void CefSubsystem::paint() {
    ::cef_app->Render();
}


}
}
