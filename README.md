# modernx11

a hyper-optimized, macro-purged c++26 named module gateway for raw linux x11 and webkit systems. replaces the legacy preprocessor text-pasting model with bleeding-edge compiled module interfaces.

## the problem
the second you `#include <x11/xlib.h>`, it pollutes your global namespace with toxic 1980s macros like `#define none 0l`, `#define status int`, and `#define success 0`. this violently smashes modern c++ abstractions like `std::optional`, `enum class`, and custom error handlers, forcing engineers into writing ugly `#undef` boilerplate. it is a crime against hardware and logic.

## the solution
`modernx11` puts up a linguistic magic wall using c++26 modules. it performs a complete macroexorcism. it ingests legacy headers inside a global module fragment, captures raw values, aggressively executes `#undef` on the toxic macros, and exports type-safe, namespaced c++ constants and symbols. 

```cpp
import std;
import modernx11; // 100% macro-free. zero leaks.

enum class appstatus { none, success, pending }; // fully legal local tokens!

int main() {
    x11::displayhandle display = x11::open_display(nullptr);
    int screen = sysui::default_screen(display);
    // bare-metal x11 window handling with absolute namespace sanctity...
}
```

## features
- **macroexorcism**: traps `none`, `status`, and `success` at the module boundary and destroys them.
- **uncapped performance**: pure x11 window mapping with absolutely zero forced vsync compositing latency or input lag. you get the raw speed of the silicon.
- **ultra-fast sub-millisecond compiles**: compiles via pure `import std;` using gcc's native module caching (`-fmodules-ts`). no text copy-pasting.
- **no cmake bloat**: built via a raw, lean, deterministic shell script because cmake is an over-engineered tragedy.

## building 
ensure you have up-to-date system development packages and `g++`:
```bash
chmod +x build.sh
./build.sh
```
(hopefully)

