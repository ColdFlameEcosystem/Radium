#include <private/backend/wayland/platformwindow.h>
#include <stdio.h>

static void registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    struct rp_wayland_xdg_client *client = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        client->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        client->xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
    }
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = NULL,
};

void init_wayland_xdg_client(struct rp_wayland_xdg_client *client) {
    client->display = wl_display_connect(NULL);
    if (!client->display) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return;
    }

    client->registry = wl_display_get_registry(client->display);
    wl_registry_add_listener(client->registry, &registry_listener, client);
    wl_display_roundtrip(client->display);

    if (!client->compositor || !client->xdg_wm_base) {
        fprintf(stderr, "Failed to bind Wayland compositor or xdg-shell\n");
        return;
    }

    // Create a surface and xdg-surface
    client->surface = wl_compositor_create_surface(client->compositor);
    client->xdg_surface = xdg_wm_base_get_xdg_surface(client->xdg_wm_base, client->surface);
    client->xdg_toplevel = xdg_surface_get_toplevel(client->xdg_surface);

    // Setup the xdg-toplevel properties
    xdg_toplevel_set_title(client->xdg_toplevel, "My Wayland Window");
    xdg_surface_set_window_geometry(client->xdg_surface, 0, 0, 800, 600);
    wl_surface_commit(client->surface);
}
