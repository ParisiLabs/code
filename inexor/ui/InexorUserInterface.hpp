#ifndef INEXOR_UI_INEXOR_FRAME_HEADER
#define INEXOR_UI_INEXOR_FRAME_HEADER

#include "include/cef_app.h"

#include "inexor/ui/InexorContextProvider.hpp"
#include "inexor/ui/InexorLayerProvider.hpp"

/**
 * The main user interface of inexor.
 */
class InexorUserInterface : public InexorContextProvider,
                            public AbstractInexorLayerProvider
{

    public:
        InexorUserInterface(std::string &name, std::string &url) : AbstractInexorLayerProvider(name, url), _name(name), _url(url) {};

        // InexorCefContextProvider
        void InitializeContext();
        bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception);
        bool Get(const CefString& name, const CefRefPtr<CefV8Value> object, CefRefPtr<CefV8Value>& retval, CefString& exception);
        bool Set(const CefString& name, const CefRefPtr<CefV8Value> object, const CefRefPtr<CefV8Value> value, CefString& exception);
        std::string GetContextName() { return _name; };

        void Reload() {
            std::string url = GetUrl();
            SetUrl(url);
        };

        void Resize(int x, int y, int width, int height) {
            if (layer.get()) {
                layer->GetInexorRenderHandler()->SetViewRect(x, y, width, height);
                layer->GetBrowser()->GetHost()->WasResized();
            }
        };

        void AutoResize(int width, int height) {
            if (layer.get()) {
                if (width != layer->GetInexorRenderHandler()->GetViewWidth() || height != layer->GetInexorRenderHandler()->GetViewHeight()) {
                    Resize(0, 0, width, height);
                }
            }
        }

    private:
        std::string _name;
        std::string _url;

        // Include the default reference counting implementation.
        IMPLEMENT_REFCOUNTING(InexorUserInterface);
};

#endif
