#ifndef WAYLAND_XDG_CLIENT_H
#define WAYLAND_XDG_CLIENT_H

#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <string.h>

struct rp_wayland_xdg_client {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

void init_wayland_xdg_client(struct rp_wayland_xdg_client *client);

#endif // WAYLAND_XDG_CLIENT_H