# Peer chat DES dua arah.
#   Sisi 1: python3 peer.py listen 5000
#   Sisi 2: python3 peer.py connect 192.168.85.1 5000           VMWare -> Local
# Yang dikirim lewat jaringan HANYA ciphertext (hex). Key sudah ada di kedua sisi, tidak dikirim.
import os
import socket
import subprocess
import sys

KEY = "133457799BBCDFF1" 
HERE = os.path.dirname(os.path.abspath(__file__))
CLI = os.path.join(HERE, "des_cli.exe" if os.name == "nt" else "des_cli")


def des(mode, hexdata):
    r = subprocess.run([CLI, mode, KEY], input=hexdata, capture_output=True, text=True)
    if r.returncode != 0:
        raise RuntimeError(r.stderr.strip())
    return r.stdout.strip()


def pad(b):  # PKCS#7, blok 8 byte
    n = 8 - len(b) % 8
    return b + bytes([n]) * n


def unpad(b):
    n = b[-1]
    if n < 1 or n > 8 or b[-n:] != bytes([n]) * n:
        raise ValueError("padding tidak valid (key salah?)")
    return b[:-n]


def send_msg(sock, text):
    cipher = des("enc", pad(text.encode("utf-8")).hex().upper())
    print(f"[KIRIM]  plaintext : {text}")
    print(f"         ciphertext: {cipher}", flush=True)
    sock.sendall((cipher + "\n").encode("ascii"))


def recv_loop(sock):
    f = sock.makefile("r", encoding="ascii")
    for line in f:
        cipher = line.strip()
        if not cipher:
            continue
        print(f"[TERIMA] ciphertext: {cipher}")
        try:
            text = unpad(bytes.fromhex(des("dec", cipher))).decode("utf-8")
            print(f"         plaintext : {text}", flush=True)
        except Exception as e:
            print(f"         gagal dekripsi: {e}", flush=True)
    print("[INFO] Koneksi ditutup oleh lawan.", flush=True)
    os._exit(0)


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == "listen":
        srv = socket.socket()
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        srv.bind(("0.0.0.0", int(sys.argv[2])))
        srv.listen(1)
        print(f"[INFO] Menunggu koneksi di port {sys.argv[2]} ...", flush=True)
        sock, addr = srv.accept()
        print(f"[INFO] Terhubung dengan {addr[0]}:{addr[1]}", flush=True)
    elif len(sys.argv) >= 4 and sys.argv[1] == "connect":
        sock = socket.create_connection((sys.argv[2], int(sys.argv[3])))
        print(f"[INFO] Terhubung ke {sys.argv[2]}:{sys.argv[3]}", flush=True)
    else:
        print("Pemakaian: peer.py listen PORT | peer.py connect IP PORT")
        sys.exit(1)

    import threading
    threading.Thread(target=recv_loop, args=(sock,), daemon=True).start()

    print("[INFO] Ketik pesan lalu Enter. Ketik 'quit' untuk keluar.", flush=True)
    try:
        while True:
            line = input()
            if line.strip().lower() == "quit":
                break
            if line:
                send_msg(sock, line)
    except (EOFError, KeyboardInterrupt):
        pass
    sock.close()


if __name__ == "__main__":
    main()