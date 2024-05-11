#pragma once

#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <xdg-decoration-client-protocol.h>

typedef struct Client Client;

struct Client {
    struct wl_display* Display;
    struct wl_registry* Registry;
    struct wl_compositor* Compositor;
    struct wl_surface* wlSurface;
    struct wl_buffer* Buffer;
    struct wl_shm* sharedMemory;
    uint8_t* pixel;
    uint16_t width;
    uint16_t height;
    struct xdg_wm_base* xdgShell;
    struct xdg_toplevel* xdgToplevel;
    struct xdg_surface* xdgSurface;
    struct wl_callback* callback;
    int close;

    struct zxdg_decoration_manager_v1* decorationManager;
    struct zxdg_toplevel_decoration_v1* topLevelDecoration;
};

Client* createClient();
void nukeClient(Client* client);
void execClient(Client* client);
