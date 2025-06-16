from scapy.all import sniff, conf
import argparse
import signal
import sys
import time
from collections import Counter
from datetime import datetime
from scapy.all import conf
conf.logLevel = "ERROR"
# Global variables to track statistics
packet_counts = Counter()
start_time = None
packet_total = 0
running = True


def packet_callback(packet):
    """Process each packet and update statistics"""
    global packet_total

    packet_total += 1

    # Try to determine the highest layer protocol
    if packet.haslayer("TCP"):
        proto = "TCP"
        src_port = packet["TCP"].sport if packet["TCP"].sport is not None else 0
        dst_port = packet["TCP"].dport if packet["TCP"].dport is not None else 0
        # Track common services
        if src_port == 80 or dst_port == 80:
            proto = "HTTP"
        elif src_port == 443 or dst_port == 443:
            proto = "HTTPS"
        elif src_port == 22 or dst_port == 22:
            proto = "SSH"
        elif src_port == 53 or dst_port == 53:
            proto = "DNS/TCP"
    elif packet.haslayer("UDP"):
        proto = "UDP"
        src_port = packet["UDP"].sport if packet["UDP"].sport is not None else 0
        dst_port = packet["UDP"].dport if packet["UDP"].dport is not None else 0
        if src_port == 53 or dst_port == 53:
            proto = "DNS"
        elif src_port in [67, 68] or dst_port in [67, 68]:
            proto = "DHCP"
    elif packet.haslayer("ICMP"):
        proto = "ICMP"
    elif packet.haslayer("ARP"):
        proto = "ARP"
    else:
        proto = "Other"

    packet_counts[proto] += 1

    # Get source and destination if available
    src = packet.src if hasattr(packet, 'src') else "Unknown"
    dst = packet.dst if hasattr(packet, 'dst') else "Unknown"

    # Get packet length
    length = len(packet) if packet else 0

    # Print packet summary
    current_time = datetime.now().strftime("%H:%M:%S")
    print(f"[{current_time}] {proto} {src} → {dst} ({length} bytes)")


def print_summary():
    """Print summary of captured packets"""
    global start_time, packet_total, packet_counts

    if start_time is None:
        return

    duration = time.time() - start_time
    packets_per_second = packet_total / duration if duration > 0 else 0

    print("\n" + "=" * 50)
    print("PACKET CAPTURE SUMMARY")
    print("=" * 50)
    print(f"Duration: {duration:.2f} seconds")
    print(f"Total packets: {packet_total}")
    print(f"Packets per second: {packets_per_second:.2f}")

    print("\nProtocol distribution:")
    for proto, count in packet_counts.most_common():
        percentage = (count / packet_total) * 100 if packet_total > 0 else 0
        print(f"  {proto}: {count} packets ({percentage:.2f}%)")

    print("=" * 50)


def signal_handler(sig, frame):
    """Handle Ctrl+C to exit gracefully"""
    global running
    print("\nStopping packet capture...")
    running = False
    # Note: This doesn't immediately stop sniffing
    # The summary will be printed in the main function

def main():
    global start_time, running

    parser = argparse.ArgumentParser(description="Simple Wireshark-like CLI packet sniffer")
    parser.add_argument("-i", "--interface", help="Network interface to sniff on")
    parser.add_argument("-c", "--count", type=int, help="Number of packets to capture")
    parser.add_argument("-f", "--filter", default="", help="BPF filter string")
    parser.add_argument("-l", "--list", action="store_true", help="List available interfaces and exit")

    args = parser.parse_args()

    if args.list:
        print("Available interfaces:")
        for iface in conf.ifaces:
            print(f"  {iface}")
        sys.exit(0)

    # Set up signal handler for Ctrl+C
    signal.signal(signal.SIGINT, signal_handler)

    # Print startup message
    print("Simple Wireshark-like CLI Packet Sniffer")
    print("Press Ctrl+C to stop and view summary")

    if args.interface:
        print(f"Interface: {args.interface}")
    if args.filter:
        print(f"Filter: {args.filter}")
    if args.count:
        print(f"Capture limited to {args.count} packets")

    print("\nStarting packet capture...\n")

    # Record start time
    start_time = time.time()

    try:
        # Start sniffing packets
        sniff(
            iface=args.interface,
            prn=packet_callback,
            filter=args.filter,
            store=0,  # Don't store packets in memory
            stop_filter=lambda x: not running  # Stop sniffing when running is False
        )
    except PermissionError:
        print("ERROR: Permission denied. Please run with root/sudo privileges.")
        sys.exit(1)
    except Exception as e:
        print(f"ERROR: {e}")
        sys.exit(1)

    # Print summary when done
    print_summary()


if __name__ == "__main__":
    main()