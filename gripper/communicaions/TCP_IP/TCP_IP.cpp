#include <cstdio>
#include <cstring>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

namespace
{
    // constexpr er compile-time constants, så det kører ved compile tid og ikke runtime. Er altså const variabler
    constexpr char WIFI_SSID[] = "Jeppe - iPhone";
    constexpr char WIFI_PASSWORD[] = "banankage3021";
    constexpr uint16_t SERVER_PORT = 4242;
    constexpr size_t BUFFER_SIZE = 128;
    constexpr uint32_t USB_SERIAL_WAIT_MS = 10000;

    enum class GripperState // state machine
    {
        OPEN,
        CLOSED,
    };

    struct TcpServerState
    {
        tcp_pcb *server_pcb = nullptr;
        tcp_pcb *client_pcb = nullptr;
    };

    TcpServerState g_tcp_server;
    GripperState g_gripper_state = GripperState::OPEN; // Åben gripper

    const char *state_to_text(GripperState state)
    {
        return state == GripperState::OPEN ? "OPEN" : "CLOSED";
    }

    void apply_gripper_state(GripperState state)
    {
        g_gripper_state = state;

        // Placeholder feedback: onboard LED is on when the test state is OPEN.
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state == GripperState::OPEN);
    }

    err_t send_response(tcp_pcb *pcb, const char *text) // tcp_pcb is a data structure like IP, portnumber and connection state
    {
        if (pcb == nullptr)
        {
            return ERR_CONN;
        }

        const uint16_t length = static_cast<uint16_t>(std::strlen(text));
        err_t result = tcp_write(pcb, text, length, TCP_WRITE_FLAG_COPY);
        if (result != ERR_OK)
        {
            return result;
        }

        return tcp_output(pcb);
    }

    err_t handle_command(tcp_pcb *pcb, const char *command)
    {
        if (std::strcmp(command, "OPEN") == 0) // strcmp sammenligner om command og "OPEN" er ens
        {
            apply_gripper_state(GripperState::OPEN);
            return send_response(pcb, "OK OPEN\n");
        }

        if (std::strcmp(command, "CLOSE") == 0)
        {
            apply_gripper_state(GripperState::CLOSED);
            return send_response(pcb, "OK CLOSED\n");
        }

        if (std::strcmp(command, "STATUS") == 0)
        {
            char response[32];
            std::snprintf(response, sizeof(response), "STATE %s\n", state_to_text(g_gripper_state));
            return send_response(pcb, response);
        }

        if (std::strcmp(command, "PING") == 0)
        {
            return send_response(pcb, "PONG\n");
        }

        return send_response(pcb, "ERR UNKNOWN_COMMAND\n");
    }

    // fjerner \n, \r (Carriage Return ) og mellemrum
    void trim_command(char *buffer)
    {
        size_t length = std::strlen(buffer);
        while (length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == '\r' || buffer[length - 1] == ' '))
        {
            buffer[length - 1] = '\0';
            --length;
        }
    }

    // Lukker socket og sætter pointers til nulpointers
    void close_client_connection()
    {
        if (g_tcp_server.client_pcb == nullptr)
        {
            return;
        }

        tcp_arg(g_tcp_server.client_pcb, nullptr);
        tcp_recv(g_tcp_server.client_pcb, nullptr);
        tcp_err(g_tcp_server.client_pcb, nullptr);
        tcp_close(g_tcp_server.client_pcb);
        g_tcp_server.client_pcb = nullptr;
        printf("Client disconnected\n");
    }

    void on_tcp_error(void *, err_t error)
    {
        g_tcp_server.client_pcb = nullptr;
        printf("TCP error: %d\n", error);
    }

    err_t on_tcp_receive(void *, tcp_pcb *pcb, pbuf *packet, err_t error)
    {
        if (packet == nullptr)
        {
            close_client_connection();
            return ERR_OK;
        }

        if (error != ERR_OK)
        {
            pbuf_free(packet);
            return error;
        }

        char buffer[BUFFER_SIZE];
        const uint16_t copy_length = static_cast<uint16_t>(packet->tot_len < BUFFER_SIZE - 1 ? packet->tot_len : BUFFER_SIZE - 1);
        std::memset(buffer, 0, sizeof(buffer));
        pbuf_copy_partial(packet, buffer, copy_length, 0); // kopier data fra pakke
        buffer[copy_length] = '\0';                        // endline
        tcp_recved(pcb, packet->tot_len);
        pbuf_free(packet); // frigør pakke

        trim_command(buffer);
        if (buffer[0] == '\0')
        {
            return ERR_OK;
        }

        printf("Received command: %s\n", buffer);
        return handle_command(pcb, buffer);
    }

    err_t on_client_connected(void *, tcp_pcb *client_pcb, err_t error)
    {
        if (error != ERR_OK)
        {
            return error;
        }

        if (g_tcp_server.client_pcb != nullptr) // der kan kun være en client
        {
            send_response(client_pcb, "ERR BUSY\n");
            tcp_close(client_pcb);
            return ERR_OK;
        }

        g_tcp_server.client_pcb = client_pcb; // gem client info
        tcp_arg(client_pcb, nullptr);
        tcp_recv(client_pcb, on_tcp_receive);
        tcp_err(client_pcb, on_tcp_error);

        printf("Client connected\n");
        return ERR_OK;
    }

    // forbind til WiFi og print IP
    bool connect_to_wifi()
    {
        printf("Connecting to WiFi SSID '%s'...\n", WIFI_SSID);
        const int result = cyw43_arch_wifi_connect_timeout_ms(
            WIFI_SSID,
            WIFI_PASSWORD,
            CYW43_AUTH_WPA2_AES_PSK,
            30000);

        if (result != 0)
        {
            printf("WiFi connection failed: %d\n", result);
            return false;
        }

        const ip4_addr_t *ip = netif_ip4_addr(netif_default);
        printf("WiFi connected. Pico IP: %s\n", ip4addr_ntoa(ip));
        return true;
    }

    bool start_tcp_server()
    {
        cyw43_arch_lwip_begin();

        // lav socket
        tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
        if (pcb == nullptr)
        {
            cyw43_arch_lwip_end();
            printf("Failed to create TCP PCB\n");
            return false;
        }

        // bind til port
        err_t result = tcp_bind(pcb, IP_ANY_TYPE, SERVER_PORT);
        if (result != ERR_OK)
        {
            tcp_close(pcb);
            cyw43_arch_lwip_end();
            printf("TCP bind failed: %d\n", result);
            return false;
        }

        // lyt
        g_tcp_server.server_pcb = tcp_listen_with_backlog(pcb, 1);
        if (g_tcp_server.server_pcb == nullptr)
        {
            cyw43_arch_lwip_end();
            printf("TCP listen failed\n");
            return false;
        }

        tcp_arg(g_tcp_server.server_pcb, nullptr);
        tcp_accept(g_tcp_server.server_pcb, on_client_connected); // sæt callbacks
        cyw43_arch_lwip_end();

        printf("TCP server listening on port %u\n", SERVER_PORT);
        return true;
    }

} // namespace

int main()
{
    // USB interface til debug
    stdio_init_all();

    absolute_time_t usb_wait_deadline = make_timeout_time_ms(USB_SERIAL_WAIT_MS);
    while (!stdio_usb_connected() && absolute_time_diff_us(get_absolute_time(), usb_wait_deadline) > 0)
    {
        sleep_ms(100);
    }

    sleep_ms(500);
    printf("BOOT OK\n");

    if (cyw43_arch_init())
    {
        printf("Failed to initialize WiFi chip\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    apply_gripper_state(GripperState::OPEN);

    if (!connect_to_wifi())
    {
        cyw43_arch_deinit();
        return 1;
    }

    if (!start_tcp_server())
    {
        cyw43_arch_deinit();
        return 1;
    }

    while (true)
    {
        sleep_ms(1000);
    }

    cyw43_arch_deinit();
    return 0;
}
