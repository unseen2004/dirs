from scapy.layers.inet import IP, ICMP, UDP
from scapy.all import sr1
import sys
import time
import argparse
from socket import gethostbyname, gethostbyaddr


def traceroute(destination, max_hops=30, timeout=2, port=33434, verbose=True):
    """
    Perform a traceroute to a destination

    Args:
        destination (str): Target IP address or hostname
        max_hops (int): Maximum number of hops to probe
        timeout (int): Timeout for each probe in seconds
        port (int): UDP port to use (for UDP probes)
        verbose (bool): Whether to print detailed output
    """
    # Convert destination to IP if it's a hostname
    dst_ip = gethostbyname(destination)

    print(f"traceroute to {destination} ({dst_ip}), {max_hops} hops max")

    # Use UDP packets by default
    for ttl in range(1, max_hops + 1):
        # Send three probes per hop for more reliable results
        hop_info = {"responses": 0, "rtts": []}

        for probe in range(3):
            # Create a packet with the current TTL
            packet = IP(dst=dst_ip, ttl=ttl) / UDP(dport=port + probe)

            start_time = time.time()
            reply = sr1(packet, timeout=timeout, verbose=0)
            end_time = time.time()

            rtt = (end_time - start_time) * 1000  # RTT in milliseconds

            if reply:
                hop_info["responses"] += 1
                hop_info["rtts"].append(rtt)

                if probe == 0:  # Only store IP for the first valid response
                    hop_info["ip"] = reply.src
                    try:
                        # Try to get the hostname (reverse DNS lookup)
                        hop_info["host"] = gethostbyaddr(reply.src)[0]
                    except:
                        hop_info["host"] = ""

        # Print the hop information
        if verbose:
            if "ip" in hop_info:
                hostname = f" ({hop_info['host']})" if hop_info['host'] else ""
                rtts_str = " ".join([f"{rtt:.2f} ms" for rtt in hop_info["rtts"]])

                if not rtts_str:
                    rtts_str = "* * *"  # No responses

                print(f"{ttl:2d}  {hop_info['ip']}{hostname}  {rtts_str}")
            else:
                # No responses for this hop
                print(f"{ttl:2d}  * * *")

        # If we've reached our destination, stop
        if "ip" in hop_info and hop_info["ip"] == dst_ip:
            break

        # Small delay between hops
        time.sleep(0.1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A simple traceroute implementation using Scapy")
    parser.add_argument("destination", help="Target IP address or hostname")
    parser.add_argument("-m", "--max-hops", type=int, default=30, help="Maximum number of hops")
    parser.add_argument("-t", "--timeout", type=int, default=2, help="Timeout in seconds")
    parser.add_argument("-p", "--port", type=int, default=33434, help="Starting UDP port")
    parser.add_argument("-q", "--quiet", action="store_true", help="Quiet output")

    args = parser.parse_args()
    traceroute(args.destination, args.max_hops, args.timeout, args.port, not args.quiet)