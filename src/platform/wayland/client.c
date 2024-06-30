#include "client.h"
#include "sharedMemory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <png.h>

void draw(Client *client) {
    uint32_t *pixel = (uint32_t *)client->pixel;
    // Fixed loop condition to avoid out-of-bounds access
    for (int i = 0; i < client->width * client->height; i++) {
        pixel[i] = 0xff888888;
    }
    wl_surface_attach(client->wlSurface, client->Buffer, 0, 0);
    wl_surface_damage_buffer(client->wlSurface, 0, 0, client->width, client->height);
    wl_surface_commit(client->wlSurface);
}


struct wl_callback_listener wlCallbackListener;

void doneCallback(void *data,
        struct wl_callback *wl_callback,
        uint32_t callback_data) {
    Client *client = (Client *) data;
    wl_callback_destroy(wl_callback);
    wl_callback = wl_surface_frame(client->wlSurface);
    wl_callback_add_listener(wl_callback, &wlCallbackListener, client);
    draw(client);
}

struct wl_callback_listener wlCallbackListener = {
    .done = doneCallback
};

void resize(Client* client) {
    uint64_t size = client->width * client->height * 4;
    int32_t fd = allocateSharedMemory(size);
    printf("%d", fd);
    client->pixel = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct wl_shm_pool* pool = wl_shm_create_pool(client->sharedMemory, fd, size);
    client->Buffer = wl_shm_pool_create_buffer(pool, 0, client->width, client->height, client->width * 4, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
}


void xdgSurfaceConfigure(void *data,struct xdg_surface *xdg_surface,uint32_t serial) {
    Client *client = (Client *) data;
    xdg_surface_ack_configure(xdg_surface, serial);
    if (!client->pixel) {
        resize(client);
    }
    // Rewrite the below part to make use of good wayland stuff
    draw(client);

};

struct xdg_surface_listener xdgSurfaceListener = {
    .configure = xdgSurfaceConfigure
};


void xdgShellPing(void *data,
                  struct xdg_wm_base *xdg_wm_base,
                  uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

struct xdg_wm_base_listener xdgWmBaseListener = {
    .ping = xdgShellPing,
};

void registryGlobal (void *data,
                    struct wl_registry *registry,
                    uint32_t name,
                    const char *interface,
                    uint32_t version) {
    Client* client = (Client *) data;
    if (!strcmp(interface, wl_compositor_interface.name)) {
        client->Compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 6);
    }
    else if (!strcmp(interface, xdg_wm_base_interface.name)) {
        client->xdgShell = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(client->xdgShell, &xdgWmBaseListener, client);
    }
    else if(!strcmp(interface, wl_shm_interface.name)) {
        client->sharedMemory = wl_registry_bind(registry, name, &wl_shm_interface, 1);
    } else if(!strcmp(interface, zxdg_decoration_manager_v1_interface.name)) {
        client->decorationManager = wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, 1);
    }
}

void xdg_toplevel_configure(void *data,
                            struct xdg_toplevel *xdg_toplevel,
                            int32_t width,
                            int32_t height,
                            struct wl_array *states) {
    Client* client = (Client *) data;
    if (!width && !height) {
        return;
    }
    if (client->width != width || client->height != height) {
        client->width = width;
        client->height = height;
        if (client->pixel){
            munmap(client->pixel, client->width * client->height * 4);
        }
        resize(client);
    }
};

void xdg_toplevel_close(void *data,
                        struct xdg_toplevel *xdg_toplevel) {
    Client* client = (Client *) data;
    client->close = 1;
}
struct xdg_toplevel_listener xdgToplevelListener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close
};

void registryGlobalRemove (void *data,
                    struct wl_registry* registry,
                    uint32_t name) {
    Client* client = (Client *) data;
    // TODO: IMPLEMENT IT
}

struct wl_registry_listener registryListener = {
    .global = registryGlobal,
    .global_remove = registryGlobalRemove
};

void configureDecoration(void *data,
                        struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1,
                        uint32_t mode) {

}

struct zxdg_toplevel_decoration_v1_listener decorationListener = {
    .configure = configureDecoration
};
void initServerDecoration(Client *client) {
    client->topLevelDecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(client->decorationManager, client->xdgToplevel);
    zxdg_toplevel_decoration_v1_add_listener(client->topLevelDecoration, &decorationListener, client);
}

Client* createClient() {
    Client* client = (Client*) malloc(sizeof(Client));
    client->close = 0;
    client->width = 1024;
    client->height = 768;
    client->Display = wl_display_connect(NULL);
    if (client->Display == NULL) {
        printf("Failed to connect to the display\n"); //Fixed Typo
    }
    printf("Connected to the display\n"); //Fixed Typo

    client->Registry = wl_display_get_registry(client->Display);
    if (client->Registry == NULL) {
        printf("Failed to get registry\n"); //Fixed Typo
    }
    printf("Got the registry\n");  //Fixed Typo

    wl_registry_add_listener(client->Registry, &registryListener, client);

    wl_display_roundtrip(client->Display);

    client->wlSurface = wl_compositor_create_surface(client->Compositor);
    client->callback = wl_surface_frame(client->wlSurface);
    wl_callback_add_listener(client->callback, &wlCallbackListener, client);

    client->xdgSurface = xdg_wm_base_get_xdg_surface(client->xdgShell, client->wlSurface);
    xdg_surface_add_listener(client->xdgSurface, &xdgSurfaceListener, client);

    client->xdgToplevel = xdg_surface_get_toplevel(client->xdgSurface);
    xdg_toplevel_add_listener(client->xdgToplevel, &xdgToplevelListener, client);

    initServerDecoration(client);
    wl_surface_commit(client->wlSurface);
    return client;
};

void execClient(Client *client) {
    while (wl_display_dispatch(client->Display)) {
        if (client->close) break;
    }
}

void nukeClient(Client* client) {
    if (client->Buffer) wl_buffer_destroy(client->Buffer);
    if (client->topLevelDecoration) zxdg_toplevel_decoration_v1_destroy(client->topLevelDecoration);
    if (client->decorationManager) zxdg_decoration_manager_v1_destroy(client->decorationManager);
    xdg_toplevel_destroy(client->xdgToplevel);
    xdg_surface_destroy(client->xdgSurface);
    wl_surface_destroy(client->wlSurface);
    wl_registry_destroy(client->Registry);
    wl_display_disconnect(client->Display);
    printf("%u", *client->pixel); // ig this is unsafe
    free(client);
}
