/**
 * @file platformwindow.h
 * @author Yegender Kumar
 * @date May 2024
 * @brief Wayland backend for the Radium
*/


#ifndef WAYLAND_XDG_CLIENT_H
#define WAYLAND_XDG_CLIENT_H

#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <string.h>


/**
 * @brief Wayland XDG client structure
 * @details This is a wrapper around the wayland xdg client
*/

#ifdef __cplusplus
extern "C" {
#endif
struct _rp_wayland_xdg_client {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

typedef struct _rp_wayland_xdg_client rp_wayland_xdg_client;

void __rp_init_wayland_xdg_client(rp_wayland_xdg_client *client);

#ifdef __cplusplus
extern "C" }
#endif

#endif // WAYLAND_XDG_CLIENT_H
