// Copyright (C) 2026 mxreal64
// Licensed under the GPL-3.0 License

import std;
import modernX11; 

int main() {
    std::println("Launching test");

    try {
        CoreUI::ContextWindow app("test", 1280, 720);

        // Bind high-performance callbacks directly to bare-metal window vectors
        app.set_paint_callback([]() {
            std::println("X11 Expose Signal Caught: Triggering raw viewport redraw pass.");
        });

        app.set_resize_callback([](X11::ResizeEvent ev) {
            std::println("Window Resized Bounds Updated: {}x{}", ev.width, ev.height);
        });

        std::println("Message pump activated. Uncapped frame processing running...");

        // Pure event loop running at maximum frequency without VSync blockages
        while (app.running()) {
            app.poll_events();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

    } catch (const std::exception& e) {
        std::println(std::cerr, "Fatal execution termination exception: {}", e.what());
        return 1;
    }

    std::println("Pure X11 environment dismantled cleanly.");
    return 0;
}
