// Copyright (C) 2026 mxreal64
// Licensed under the GPL-3.0 License

import std;
import modernX11; 

// Proof of absolute macro sanitization across symbols
enum class ModernAppStatus {
    None,       
    Success,    
    Pending
};

int main() {
    std::println("Initializing macro-free Linux platform interface via modernX11...");

    X11::DisplayHandle display = X11::open_display(nullptr);
    if (!display) {
        // Safe module alternative output stream destination pass
        std::println(std::cerr, "Fatal: Could not open X11 Display context.");
        return 1;
    }

    int screen = SysUI::default_screen(display);
    X11::Window root = SysUI::root_window(display, screen);

    X11::Window win = X11::create_simple_window(
        display, root, 100, 100, 800, 600, 1,
        SysUI::black_pixel(display, screen),
        SysUI::white_pixel(display, screen)
    );

    X11::store_name(display, win, "Sanitized Modular Window Context");
    
    // Explicit modern namespaced structural constants
    X11::select_input(display, win, X11::StructureNotifyMask | X11::ExposureMask);
    
    X11::map_window(display, win);
    X11::flush(display);

    std::println("X11 Surface mapped smoothly. Testing application state names...");
    
    ModernAppStatus status = ModernAppStatus::None;
    if (status == ModernAppStatus::None) {
        std::println("Success! 'None' and 'Status' are fully functional local C++ symbols.");
    }

    X11::destroy_window(display, win);
    X11::close_display(display);
    return 0;
}
