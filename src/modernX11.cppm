// Copyright (C) 2026 mxreal64
// Licensed under the GPL-3.0 License

module;
#include <X11/Xlib.h>
#include <X11/Xutil.h>

export module modernX11;

import std;

namespace InternalUI {
    [[nodiscard]] int get_default_screen(void* d) noexcept { 
        return DefaultScreen(static_cast<::Display*>(d)); 
    }
    [[nodiscard]] ::Window get_root_window(void* d, int s) noexcept { 
        return RootWindow(static_cast<::Display*>(d), s); 
    }
    [[nodiscard]] unsigned long get_black_pixel(void* d, int s) noexcept { 
        return BlackPixel(static_cast<::Display*>(d), s); 
    }
    [[nodiscard]] unsigned long get_white_pixel(void* d, int s) noexcept { 
        return WhitePixel(static_cast<::Display*>(d), s); 
    }

    // 1. CAPTURE FIRST: Store raw macro values safely
    constexpr long tmp_structure_notify_mask = StructureNotifyMask;
    constexpr long tmp_exposure_mask = ExposureMask;

    constexpr int tmp_expose = Expose;
    constexpr int tmp_configure_notify = ConfigureNotify;
    constexpr int tmp_destroy_notify = DestroyNotify;
    constexpr int tmp_client_message = ClientMessage;
}

// 2. PURGE SECOND: Exterminate the legacy preprocessor macros
#undef StructureNotifyMask
#undef ExposureMask
#undef Expose
#undef ConfigureNotify
#undef DestroyNotify
#undef ClientMessage

export namespace X11 {
    struct WindowHandle {
        unsigned long xid = 0;
        [[nodiscard]] explicit constexpr operator bool() const noexcept { return xid != 0; }
    };

    using DisplayHandle = void*;

    // 3. EXPORT THIRD: Clean, namespaced modern C++ symbols
    constexpr long StructureNotifyMask = InternalUI::tmp_structure_notify_mask;
    constexpr long ExposureMask        = InternalUI::tmp_exposure_mask;

    enum class EventType : int {
        Expose          = InternalUI::tmp_expose,
        ConfigureNotify = InternalUI::tmp_configure_notify,
        DestroyNotify   = InternalUI::tmp_destroy_notify,
        ClientMessage   = InternalUI::tmp_client_message,
        Unknown         = -1
    };

    struct ResizeEvent {
        int width = 0;
        int height = 0;
    };

    [[nodiscard]] inline DisplayHandle open_display(const char* name) noexcept { 
        return static_cast<DisplayHandle>(::XOpenDisplay(name)); 
    }
    inline int close_display(DisplayHandle d) noexcept { 
        return ::XCloseDisplay(static_cast<::Display*>(d)); 
    }
    [[nodiscard]] inline WindowHandle create_window(DisplayHandle d, WindowHandle root, int x, int y, unsigned int w, unsigned int h, unsigned long b_px, unsigned long w_px) noexcept {
        return WindowHandle{ ::XCreateSimpleWindow(static_cast<::Display*>(d), root.xid, x, y, w, h, 1, b_px, w_px) };
    }
    inline void map_window(DisplayHandle d, WindowHandle w) noexcept { ::XMapWindow(static_cast<::Display*>(d), w.xid); }
    inline void flush(DisplayHandle d) noexcept { ::XFlush(static_cast<::Display*>(d)); }
    inline void destroy_window(DisplayHandle d, WindowHandle w) noexcept { ::XDestroyWindow(static_cast<::Display*>(d), w.xid); }
}

export namespace CoreUI {

    class ContextWindow {
    private:
        X11::DisplayHandle display_ = nullptr;
        X11::WindowHandle win_handle_{};
        unsigned long wm_delete_window_ = 0; // Stores the WM_DELETE_WINDOW Atom
        
        int width_ = 0;
        int height_ = 0;
        bool should_close_ = false;

        std::function<void()> on_paint_cb_;
        std::function<void(X11::ResizeEvent)> on_resize_cb_;

    public:
        ContextWindow(std::string_view title, int width, int height) 
            : width_(width), height_(height) 
        {
            display_ = X11::open_display(nullptr);
            if (!display_) throw std::runtime_error("X11 connection failed.");

            int screen = InternalUI::get_default_screen(display_);
            auto root_xid = InternalUI::get_root_window(display_, screen);
            
            win_handle_ = X11::create_window(
                display_, X11::WindowHandle{root_xid}, 100, 100, width_, height_,
                InternalUI::get_black_pixel(display_, screen),
                InternalUI::get_white_pixel(display_, screen)
            );

            auto* dpy = static_cast<::Display*>(display_);

            // Register the close button intercept protocol atom
            wm_delete_window_ = ::XInternAtom(dpy, "WM_DELETE_WINDOW", False);
            ::XSetWMProtocols(dpy, win_handle_.xid, &wm_delete_window_, 1);

            ::XStoreName(dpy, win_handle_.xid, std::string(title).c_str());
            ::XSelectInput(dpy, win_handle_.xid, X11::StructureNotifyMask | X11::ExposureMask);
            X11::map_window(display_, win_handle_);
            X11::flush(display_);
        }

        ~ContextWindow() {
            if (display_ && win_handle_) {
                X11::destroy_window(display_, win_handle_);
                X11::close_display(display_);
            }
        }

        void set_paint_callback(std::function<void()>&& cb) noexcept { on_paint_cb_ = std::move(cb); }
        void set_resize_callback(std::function<void(X11::ResizeEvent)>&& cb) noexcept { on_resize_cb_ = std::move(cb); }

        [[nodiscard]] bool running() const noexcept { return !should_close_; }

        void poll_events() noexcept {
            ::XEvent x_event;
            auto* dpy = static_cast<::Display*>(display_);

            while (::XPending(dpy)) {
                ::XNextEvent(dpy, &x_event);

                // Safely cast the raw integer event type to our type-safe C++ enum
                const auto current_event = static_cast<X11::EventType>(x_event.type);

                switch (current_event) {
                    case X11::EventType::Expose: {
                        if (on_paint_cb_) on_paint_cb_();
                        break;
                    }
                    case X11::EventType::ConfigureNotify: {
                        width_ = x_event.xconfigure.width;
                        height_ = x_event.xconfigure.height;
                        if (on_resize_cb_) {
                            on_resize_cb_(X11::ResizeEvent{width_, height_});
                        }
                        break;
                    }
                    case X11::EventType::DestroyNotify: {
                        should_close_ = true;
                        break;
                    }
                    case X11::EventType::ClientMessage: {
                        // Check if the message data matches our close atom payload
                        if (static_cast<unsigned long>(x_event.xclient.data.l[0]) == wm_delete_window_) {
                            should_close_ = true;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };
}
