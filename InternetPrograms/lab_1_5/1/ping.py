from scapy.layers.inet import IP, TCP, ICMP
from scapy.all import sr1
import time
import sys
import argparse


def ping(destination, count=4, timeout=2, verbose=True):
    """
    Send ICMP echo request packets to a destination

    Args:
        destination (str): Target IP address or hostname
        count (int): Number of packets to send
        timeout (int): Timeout in seconds
        verbose (bool): Whether to print detailed output
    """
    print(f"PING {destination}")

    # Statistics
    packets_sent = 0
    packets_received = 0
    rtt_values = []

    for i in range(count):
        # Create the packet - an ICMP echo request
        packet = IP(dst=destination) / ICMP()

        # Send the packet and start timing
        start_time = time.time()
        try:
            # sr1 sends a packet and returns the first response
            reply = sr1(packet, timeout=timeout, verbose=0)
            end_time = time.time()

            # Calculate round-trip time in milliseconds
            rtt = (end_time - start_time) * 1000

            packets_sent += 1

            if reply:
                packets_received += 1
                rtt_values.append(rtt)
                if verbose:
                    print(f"{len(reply)} bytes from {reply.src}: icmp_seq={i + 1} time={rtt:.2f} ms")
            else:
                if verbose:
                    print(f"Request timeout for icmp_seq {i + 1}")

        except Exception as e:
            print(f"Error: {e}")

        # Wait a second before sending the next packet
        if i < count - 1:
            time.sleep(1)

    # Print statistics
    print(f"\n--- {destination} ping statistics ---")
    print(f"{packets_sent} packets transmitted, {packets_received} packets received, " +
          f"{((packets_sent - packets_received) / packets_sent * 100):.1f}% packet loss")

    if rtt_values:
        print(
            f"round-trip min/avg/max = {min(rtt_values):.2f}/{sum(rtt_values) / len(rtt_values):.2f}/{max(rtt_values):.2f} ms")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A simple ping implementation using Scapy")
    parser.add_argument("destination", help="Target IP address or hostname")
    parser.add_argument("-c", "--count", type=int, default=4, help="Number of packets to send")
    parser.add_argument("-t", "--timeout", type=int, default=2, help="Timeout in seconds")
    parser.add_argument("-q", "--quiet", action="store_true", help="Quiet output")

    args = parser.parse_args()
    ping(args.destination, args.count, args.timeout, not args.quiet)