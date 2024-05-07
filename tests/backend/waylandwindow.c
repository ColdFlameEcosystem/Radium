#include <private/backend/wayland/platformwindow.h>

int main() {
    rp_wayland_xdg_client client = {0};
    __rp_init_wayland_xdg_client(&client);

    // Your application logic here
    while (1) {
        wl_display_dispatch(client.display);
    }

    return 0;
}