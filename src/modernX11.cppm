// Copyright (C) 2026 mxreal64
// Licensed under the GPL-3.0 License

module;

// Isolate toxic legacy headers inside the global module fragment
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <gdk/gdkx.h>

export module modernX11;

import std;

// 1. NON-EXPORTED SECTION: Capture macro states before destroying them
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

    // Capture the raw layout integer constants cleanly right here
    constexpr long tmp_structure_notify_mask = StructureNotifyMask;
    constexpr long tmp_exposure_mask = ExposureMask;
}

// 2. THE PURGE: Forcefully execute preprocessor wipes on the X11 macros.
// This clears the identifiers completely so they can be reused as legal C++ labels.
#undef StructureNotifyMask
#undef ExposureMask

export namespace X11 {
    using DisplayHandle = void*;
    using Window = ::Window;
    using GtkWidget = ::GtkWidget;
    using GdkWindow = ::GdkWindow;

    // 3. EXPORT CLEAN CONSTANTS: They are now completely free of preprocessor text mutations!
    constexpr long StructureNotifyMask = InternalUI::tmp_structure_notify_mask;
    constexpr long ExposureMask = InternalUI::tmp_exposure_mask;

    inline DisplayHandle open_display(const char* name) noexcept { 
        return static_cast<DisplayHandle>(::XOpenDisplay(name)); 
    }
    inline int close_display(DisplayHandle d) noexcept { 
        return ::XCloseDisplay(static_cast<::Display*>(d)); 
    }
    
    inline ::Window create_simple_window(DisplayHandle d, ::Window r, int x, int y, unsigned int w, unsigned int h, unsigned int b, unsigned long b_px, unsigned long w_px) noexcept {
        return ::XCreateSimpleWindow(static_cast<::Display*>(d), r, x, y, w, h, b, b_px, w_px);
    }
    
    inline int store_name(DisplayHandle d, ::Window w, const char* name) noexcept { 
        return ::XStoreName(static_cast<::Display*>(d), w, name); 
    }
    inline int select_input(DisplayHandle d, ::Window w, long mask) noexcept { 
        return ::XSelectInput(static_cast<::Display*>(d), w, mask); 
    }
    inline int map_window(DisplayHandle d, ::Window w) noexcept { 
        return ::XMapWindow(static_cast<::Display*>(d), w); 
    }
    inline int flush(DisplayHandle d) noexcept { 
        return ::XFlush(static_cast<::Display*>(d)); 
    }
    inline int destroy_window(DisplayHandle d, ::Window w) noexcept { 
        return ::XDestroyWindow(static_cast<::Display*>(d), w); 
    }
    
    using ::gtk_init;
    using ::gtk_window_new;
    using ::gtk_widget_realize;
    using ::gtk_widget_get_window;
    using ::webkit_web_view_new;
    using ::webkit_web_view_load_html;
    using ::webkit_web_view_get_user_content_manager;
    using ::webkit_user_content_manager_register_script_message_handler;
    
    using ::g_main_context_iteration;
    using ::g_main_context_default;
}

export namespace SysUI {
    [[nodiscard]] inline int default_screen(X11::DisplayHandle d) noexcept { 
        return InternalUI::get_default_screen(d); 
    }
    [[nodiscard]] inline ::Window root_window(X11::DisplayHandle d, int s) noexcept { 
        return InternalUI::get_root_window(d, s); 
    }
    [[nodiscard]] inline unsigned long black_pixel(X11::DisplayHandle d, int s) noexcept { 
        return InternalUI::get_black_pixel(d, s); 
    }
    [[nodiscard]] inline unsigned long white_pixel(X11::DisplayHandle d, int s) noexcept { 
        return InternalUI::get_white_pixel(d, s); 
    }
    [[nodiscard]] inline ::Window gdk_window_xid(X11::GdkWindow* w) noexcept { 
        return GDK_WINDOW_XID(w); 
    }
}
