#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "examples.c"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "yarl.h"

int main(void) {

    Yarl *yarl = yarl_init(500, 500, YARL_COLOR_FORMAT_BGRA);
    assert(yarl != NULL);
    triangles(yarl);

    Display *dpy = XOpenDisplay(NULL);
    assert(dpy != NULL);

    Window root = XDefaultRootWindow(dpy);
    int scr = XDefaultScreen(dpy);
    GC gc = XCreateGC(dpy, root, 0, NULL);
    Window win = XCreateSimpleWindow(dpy, root, 0, 0, 500, 500, 0, 0, 0);
    XSelectInput(dpy, win, KeyPressMask | ExposureMask);
    Visual *vis = XDefaultVisual(dpy, scr);

    int w = yarl_get_width(yarl);
    int h = yarl_get_height(yarl);
    XImage *img = XCreateImage(dpy, vis, 0, XYBitmap, 0, (char*) yarl_get_buffer(yarl), w, h, 8, w * 4);
    assert(img != NULL);

    XMapRaised(dpy, win);

    XEvent ev = { 0 };
    bool quit = false;
    while (!quit) {

        // TODO:
        XPutImage(dpy, win, gc, img, 0, 0, 0, 0, w, h);

        XNextEvent(dpy, &ev);
        XClearWindow(dpy, win);

        switch (ev.type) {
            case KeyPress: {
                uint32_t keycode = ev.xkey.keycode;
                if (keycode == 0x9 || keycode == 0x18)
                    quit = true;
            } break;

        }

    }

    XCloseDisplay(dpy);
    yarl_destroy(yarl);
}
