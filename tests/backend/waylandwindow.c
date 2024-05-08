#include <private/backend/wayland/platformwindow.h>

int main() {
    rp_wayland_client *client = rp_wayland_client_create(0);
    return rp_wayland_client_run(client);
}