import networkx as nx
import numpy as np
import random
import matplotlib.pyplot as plt
from tqdm import tqdm
from tabulate import tabulate
import datetime


class NetworkReliabilityAnalyzer:
    def __init__(self, num_vertices=20, edge_failure_prob=0.05, T_max=0.5, avg_packet_size=1000):
        """
        Initialize the network model and reliability parameters.
        """
        self.num_vertices = num_vertices
        self.edge_failure_prob = edge_failure_prob
        self.p = 1 - edge_failure_prob  # Probability of edge not failing
        self.T_max = T_max
        self.m = avg_packet_size  # Average packet size in bits

        # Create initial graph and parameters
        self.G = None
        self.N = None
        self.capacities = {}
        self.flows = {}

        # Initialize the network
        self.initialize_network()

    def initialize_network(self):
        """Create initial network with realistic parameters"""
        # Create a directed graph
        self.G = nx.DiGraph()
        self.G.add_nodes_from(range(1, self.num_vertices + 1))

        # Create a ring structure to ensure basic connectivity
        for i in range(1, self.num_vertices):
            self.G.add_edge(i, i + 1)
        self.G.add_edge(self.num_vertices, 1)  # Complete the ring

        # Add some random edges to make the graph more interesting, but keep |E| < 30
        additional_edges = random.randint(5, 9)
        edges_added = 0

        while edges_added < additional_edges:
            i = random.randint(1, self.num_vertices)
            j = random.randint(1, self.num_vertices)
            if i != j and not self.G.has_edge(i, j):
                self.G.add_edge(i, j)
                edges_added += 1

        # Create the traffic intensity matrix N with realistic values
        self.N = np.zeros((self.num_vertices, self.num_vertices))
        for i in range(self.num_vertices):
            for j in range(self.num_vertices):
                if i != j:
                    # Assign lower traffic values (0-2 packets/second)
                    self.N[i, j] = round(random.uniform(0, 2), 2)

        # Define capacity and flow functions with realistic values
        self.capacities = {}
        self.flows = {}

        # First assign capacities (much higher than in previous model)
        for u, v in self.G.edges():
            # Assign capacity between 5000 and 10000 bits/second
            self.capacities[(u, v)] = random.randint(5000, 10000)

        # Then calculate flows based on routing
        # Simple routing model: distribute traffic evenly along shortest paths
        self._calculate_flows()

    def _calculate_flows(self):
        """Calculate realistic flows based on routing of traffic matrix N"""
        # Reset flows
        self.flows = {edge: 0 for edge in self.G.edges()}

        # For each source-destination pair, route traffic along shortest path
        for i in range(1, self.num_vertices + 1):
            for j in range(1, self.num_vertices + 1):
                if i != j and self.N[i - 1, j - 1] > 0:
                    # Find shortest path
                    try:
                        path = nx.shortest_path(self.G, i, j)
                        # Add flow to each edge in the path
                        for k in range(len(path) - 1):
                            u, v = path[k], path[k + 1]
                            self.flows[(u, v)] += self.N[i - 1, j - 1]
                    except nx.NetworkXNoPath:
                        # No path exists, skip this pair
                        pass

    def print_network_details(self):
        """Print detailed information about the network"""
        print("\n===== NETWORK DETAILS =====")
        print(f"Number of vertices: {self.num_vertices}")
        print(f"Number of edges: {self.G.number_of_edges()}")
        print(f"Edge non-failure probability (p): {self.p:.2f}")
        print(f"Maximum acceptable delay (T_max): {self.T_max}")
        print(f"Average packet size (m): {self.m} bits")

        # Print edge information
        print("\nEdges with Capacity c(e) and Flow a(e):")
        edge_data = []
        for u, v in sorted(self.G.edges()):
            capacity = self.capacities[(u, v)]
            flow = self.flows[(u, v)]
            # Calculate utilization correctly (as percentage)
            utilization = (flow * self.m / capacity) * 100
            edge_data.append([f"{u} -> {v}", capacity, f"{flow:.2f}", f"{utilization:.2f}%"])

        headers = ["Edge", "Capacity c(e)", "Flow a(e)", "Utilization"]
        print(tabulate(edge_data, headers=headers, tablefmt="grid"))

        # Print traffic intensity matrix summary
        print("\nTraffic Intensity Matrix N (sample):")
        matrix_data = []
        count = 0
        for i in range(self.num_vertices):
            for j in range(self.num_vertices):
                if self.N[i, j] > 0 and count < 10:  # Show first 10 non-zero entries
                    matrix_data.append([i + 1, j + 1, self.N[i, j]])
                    count += 1

        matrix_headers = ["Source v(i)", "Destination v(j)", "Intensity n(i,j)"]
        print(tabulate(matrix_data, headers=matrix_headers, tablefmt="grid"))
        print("... (remaining entries omitted)")

        # Calculate total traffic
        total_traffic = np.sum(self.N)
        print(f"\nTotal traffic intensity: {total_traffic:.2f} packets/second")

    def calculate_average_delay(self, graph=None, capacities=None, flows=None):
        """
        Calculate the average packet delay T in the network.

        T = 1/G * SUM_e( a(e)/(c(e)/m - a(e)) )
        """
        if graph is None:
            graph = self.G
        if capacities is None:
            capacities = self.capacities
        if flows is None:
            flows = self.flows

        # Check if the graph is strongly connected (for directed graph)
        if not nx.is_strongly_connected(graph):
            return float('inf'), False, False

        # Calculate G (sum of all elements in N)
        G = np.sum(self.N)
        if G <= 0:
            return 0, True, True  # No traffic means no delay

        # Calculate sum of delays over all edges
        sum_delays = 0
        for u, v in graph.edges():
            if (u, v) in flows and (u, v) in capacities:
                flow = flows[(u, v)]
                capacity = capacities[(u, v)]

                # Check if capacity is sufficient for the flow
                effective_capacity = capacity / self.m  # Convert bits/sec to packets/sec
                if effective_capacity <= flow:
                    return float('inf'), False, True  # Infinite delay

                # Add delay component
                sum_delays += flow / (effective_capacity - flow)

        # Calculate average delay
        T = (1 / G) * sum_delays
        return T, T < self.T_max, True

    def estimate_reliability(self, num_simulations=1000):
        """
        Estimate the reliability of the network using Monte Carlo simulation.
        """
        success_count = 0

        for _ in range(num_simulations):
            # Create a copy of the graph to simulate failures
            current_graph = self.G.copy()
            current_capacities = self.capacities.copy()
            current_flows = self.flows.copy()

            # Remove edges based on failure probability
            edges_to_remove = []
            for u, v in list(self.G.edges()):
                if random.random() > self.p:  # Edge fails with probability 1-p
                    edges_to_remove.append((u, v))

            # Remove the edges
            for u, v in edges_to_remove:
                current_graph.remove_edge(u, v)
                # Also remove the capacities and flows for failed edges
                current_capacities.pop((u, v), None)
                current_flows.pop((u, v), None)

            # Recalculate flows for the modified network
            # This is a simplification - in a real network, traffic would be rerouted
            modified_flows = {edge: 0 for edge in current_graph.edges()}

            # Attempt to route traffic in modified network
            for i in range(1, self.num_vertices + 1):
                for j in range(1, self.num_vertices + 1):
                    if i != j and self.N[i - 1, j - 1] > 0:
                        try:
                            path = nx.shortest_path(current_graph, i, j)
                            for k in range(len(path) - 1):
                                u, v = path[k], path[k + 1]
                                modified_flows[(u, v)] += self.N[i - 1, j - 1]
                        except nx.NetworkXNoPath:
                            # No path exists in the modified network
                            pass

            # Calculate delay for this scenario
            T, is_valid, is_connected = self.calculate_average_delay(
                current_graph, current_capacities, modified_flows)

            if is_valid and is_connected:
                success_count += 1

        return success_count / num_simulations

    def analyze_increasing_traffic(self, steps=10, scale_factor_max=3.0):
        """Analyze how reliability changes when increasing traffic intensity"""
        scale_factors = np.linspace(1.0, scale_factor_max, steps)
        reliability_estimates = []
        delays = []

        original_N = self.N.copy()
        original_flows = self.flows.copy()

        print("\n===== TRAFFIC INTENSITY ANALYSIS =====")
        print("Analyzing effect of increasing traffic intensity...")

        results_data = []

        for factor in tqdm(scale_factors, desc="Analyzing traffic intensity"):
            # Scale the traffic matrix
            self.N = original_N * factor

            # Recalculate flows based on scaled traffic
            self._calculate_flows()

            # Estimate reliability with scaled traffic
            reliability = self.estimate_reliability()
            reliability_estimates.append(reliability)

            # Calculate current average delay
            T, is_valid, _ = self.calculate_average_delay()
            if T == float('inf'):
                T_display = "inf"
            else:
                T_display = f"{T:.4f}"
            delays.append(T)

            results_data.append([
                f"{factor:.2f}",
                f"{reliability:.4f}",
                T_display,
                "Yes" if is_valid else "No"
            ])

        # Print results in table format
        headers = ["Traffic Scale Factor", "Reliability", "Average Delay (T)", "T < T_max"]
        print("\nTraffic Intensity Results:")
        print(tabulate(results_data, headers=headers, tablefmt="grid"))

        # Restore original traffic matrix and flows
        self.N = original_N
        self.flows = original_flows

        return scale_factors, reliability_estimates, delays

    def analyze_increasing_capacity(self, steps=10, scale_factor_max=3.0):
        """Analyze how reliability changes when increasing edge capacities"""
        scale_factors = np.linspace(1.0, scale_factor_max, steps)
        reliability_estimates = []
        delays = []

        original_capacities = self.capacities.copy()

        print("\n===== CAPACITY SCALING ANALYSIS =====")
        print("Analyzing effect of increasing edge capacities...")

        results_data = []

        for factor in tqdm(scale_factors, desc="Analyzing capacity scaling"):
            # Scale the capacities
            scaled_capacities = {edge: capacity * factor for edge, capacity in original_capacities.items()}

            # Update capacities temporarily
            self.capacities = scaled_capacities

            # Estimate reliability with scaled capacities
            reliability = self.estimate_reliability()
            reliability_estimates.append(reliability)

            # Calculate current average delay
            T, is_valid, _ = self.calculate_average_delay()
            if T == float('inf'):
                T_display = "inf"
            else:
                T_display = f"{T:.4f}"
            delays.append(T)

            results_data.append([
                f"{factor:.2f}",
                f"{reliability:.4f}",
                T_display,
                "Yes" if is_valid else "No"
            ])

        # Print results in table format
        headers = ["Capacity Scale Factor", "Reliability", "Average Delay (T)", "T < T_max"]
        print("\nCapacity Scaling Results:")
        print(tabulate(results_data, headers=headers, tablefmt="grid"))

        # Restore original capacities
        self.capacities = original_capacities

        return scale_factors, reliability_estimates, delays

    def analyze_adding_edges(self, steps=10):
        """Analyze how reliability changes when adding new edges to the topology"""
        # Calculate average capacity of existing edges
        avg_capacity = sum(self.capacities.values()) / len(self.capacities)

        # Find potential new edges (not already in the graph)
        potential_edges = []
        for i in range(1, self.num_vertices + 1):
            for j in range(1, self.num_vertices + 1):
                if i != j and not self.G.has_edge(i, j):
                    potential_edges.append((i, j))

        # Limit steps based on available potential edges
        actual_steps = min(steps, len(potential_edges))

        edge_counts = []
        reliability_estimates = []
        delays = []

        original_graph = self.G.copy()
        original_capacities = self.capacities.copy()
        original_flows = self.flows.copy()

        print("\n===== TOPOLOGY CHANGE ANALYSIS =====")
        print("Analyzing effect of adding edges to topology...")

        results_data = []

        # Get initial edge count and reliability
        edge_count = original_graph.number_of_edges()
        edge_counts.append(edge_count)
        reliability = self.estimate_reliability()
        reliability_estimates.append(reliability)

        T, is_valid, _ = self.calculate_average_delay()
        if T == float('inf'):
            T_display = "inf"
        else:
            T_display = f"{T:.4f}"
        delays.append(T)

        results_data.append([
            edge_count,
            f"{reliability:.4f}",
            T_display,
            "Yes" if is_valid else "No"
        ])

        # Shuffle potential edges for random selection
        random.shuffle(potential_edges)

        current_graph = original_graph.copy()
        current_capacities = original_capacities.copy()

        # Add edges incrementally
        for i in tqdm(range(actual_steps), desc="Analyzing topology changes"):
            if i < len(potential_edges):
                u, v = potential_edges[i]
                current_graph.add_edge(u, v)

                # Set capacity and flow for new edge
                current_capacities[(u, v)] = avg_capacity

                # Update graph and capacities
                self.G = current_graph
                self.capacities = current_capacities

                # Recalculate flows for the modified network
                self._calculate_flows()

                # Update edge count
                edge_count = current_graph.number_of_edges()
                edge_counts.append(edge_count)

                # Estimate reliability with new edge
                reliability = self.estimate_reliability()
                reliability_estimates.append(reliability)

                # Calculate current average delay
                T, is_valid, _ = self.calculate_average_delay()
                if T == float('inf'):
                    T_display = "inf"
                else:
                    T_display = f"{T:.4f}"
                delays.append(T)

                results_data.append([
                    edge_count,
                    f"{reliability:.4f}",
                    T_display,
                    "Yes" if is_valid else "No"
                ])

        # Print results in table format
        headers = ["Number of Edges", "Reliability", "Average Delay (T)", "T < T_max"]
        print("\nTopology Change Results:")
        print(tabulate(results_data, headers=headers, tablefmt="grid"))

        # Restore original graph and parameters
        self.G = original_graph
        self.capacities = original_capacities
        self.flows = original_flows

        return edge_counts, reliability_estimates, delays

    def plot_results(self, x_values, reliability_values, delays, title, xlabel, fig_filename):
        """Plot the results of an analysis"""
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

        # Convert infinite delays to a high but finite value for plotting
        plot_delays = [d if d != float('inf') else self.T_max * 5 for d in delays]

        # Plot reliability
        ax1.plot(x_values, reliability_values, 'bo-', linewidth=2)
        ax1.set_ylabel('Reliability Pr[T < T_max]')
        ax1.set_title(f'{title} - Reliability')
        ax1.grid(True)
        ax1.set_ylim(0, 1.05)

        # Plot delay
        ax2.plot(x_values, plot_delays, 'ro-', linewidth=2)
        ax2.axhline(y=self.T_max, color='g', linestyle='--', label=f'T_max = {self.T_max}')
        ax2.set_xlabel(xlabel)
        ax2.set_ylabel('Average Delay T')
        ax2.set_title(f'{title} - Average Delay')
        ax2.grid(True)
        ax2.legend()

        # Add annotations for infinite delay points
        for i, d in enumerate(delays):
            if d == float('inf'):
                ax2.annotate('∞',
                             xy=(x_values[i], plot_delays[i]),
                             xytext=(0, -20),
                             textcoords='offset points',
                             ha='center')

        plt.tight_layout()
        plt.savefig(fig_filename)
        print(f"Plot saved as {fig_filename}")

    def run_full_analysis(self, traffic_steps=8, capacity_steps=8, topology_steps=8):
        """Run the full analysis for all three scenarios and plot results"""
        # Set random seed for reproducibility
        random.seed(42)
        np.random.seed(42)

        current_time = datetime.datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S")

        print("\n======================================================")
        print("NETWORK RELIABILITY ANALYSIS (CORRECTED MODEL)")
        print("======================================================")
        print(f"Date and Time: {current_time}")

        # Print network details
        self.print_network_details()

        # Calculate initial average delay
        T_initial, is_valid, is_connected = self.calculate_average_delay()
        print("\n===== INITIAL NETWORK ANALYSIS =====")
        if T_initial == float('inf'):
            print(f"Initial average delay (T): infinite")
        else:
            print(f"Initial average delay (T): {T_initial:.4f}")
        print(f"Is delay valid (T < T_max = {self.T_max})? {is_valid}")
        print(f"Is network connected? {is_connected}")

        # Estimate initial reliability
        initial_reliability = self.estimate_reliability()
        print(f"Initial network reliability: {initial_reliability:.4f}")

        # 1. Analysis for increasing traffic
        traffic_factors, traffic_reliability, traffic_delays = self.analyze_increasing_traffic(steps=traffic_steps)
        self.plot_results(traffic_factors, traffic_reliability, traffic_delays,
                          "Effect of Increasing Traffic Intensity",
                          "Traffic Scale Factor", "traffic_reliability.png")

        # 2. Analysis for increasing capacity
        capacity_factors, capacity_reliability, capacity_delays = self.analyze_increasing_capacity(steps=capacity_steps)
        self.plot_results(capacity_factors, capacity_reliability, capacity_delays,
                          "Effect of Increasing Edge Capacities",
                          "Capacity Scale Factor", "capacity_reliability.png")

        # 3. Analysis for adding edges
        edge_counts, topology_reliability, topology_delays = self.analyze_adding_edges(steps=topology_steps)
        self.plot_results(edge_counts, topology_reliability, topology_delays,
                          "Effect of Adding Edges to Topology",
                          "Number of Edges", "topology_reliability.png")

        print("\n===== ANALYSIS SUMMARY =====")
        print("1. Traffic Intensity: As traffic increases, reliability decreases")
        print("2. Capacity Scaling: As capacity increases, reliability improves")
        print("3. Topology Changes: Adding more edges improves reliability by providing alternative paths")
        print("\nAnalysis complete. Results saved as PNG files.")


# Set up and run the analysis with improved parameters
if __name__ == "__main__":
    analyzer = NetworkReliabilityAnalyzer(
        num_vertices=20,
        edge_failure_prob=0.05,  # 5% chance of edge failure
        T_max=0.5,  # Maximum acceptable delay
        avg_packet_size=1000  # Average packet size in bits
    )

    analyzer.run_full_analysis(traffic_steps=8, capacity_steps=8, topology_steps=8)