#include <client.h>

int main() {
    Client* client = createClient();
    xdg_toplevel_set_title(client->xdgToplevel, "Hello Radium");
    xdg_toplevel_set_app_id(client->xdgToplevel, "");
    execClient(client);
    nukeClient(client);
    return 0;
}
