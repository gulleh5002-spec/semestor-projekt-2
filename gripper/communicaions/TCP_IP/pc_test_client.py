#!/usr/bin/env python3
import socket
import sys


def main() -> int:
    if len(sys.argv) < 3:
        print("Usage: python3 pc_test_client.py <pico_ip> <COMMAND> [<COMMAND> ...]")
        print("Example: python3 pc_test_client.py 192.168.137.42 STATUS OPEN STATUS CLOSE STATUS")
        return 1

    host = sys.argv[1]
    commands = sys.argv[2:]
    port = 4242

    try:
        with socket.create_connection((host, port), timeout=10) as sock:
            for command in commands:
                sock.sendall((command.strip().upper() + "\n").encode("utf-8"))
                response = sock.recv(128).decode("utf-8", errors="replace").strip()
                print(f"> {command.strip().upper()}")
                print(f"< {response}")
    except ConnectionResetError:
        print("Connection was reset by the Pico.")
        print("That usually means the Pico accepted the TCP connection, then closed it or rebooted.")
        print("Check the Pico serial monitor right after you run this command.")
        return 2
    except TimeoutError:
        print("Timed out waiting for the Pico.")
        return 3
    except OSError as exc:
        print(f"Socket error: {exc}")
        return 4

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
