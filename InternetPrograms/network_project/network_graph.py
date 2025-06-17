import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from typing import Dict, List, Tuple
import warnings

warnings.filterwarnings('ignore')

NUM_VERTICES = 20  # |V| = 20
MAX_EDGES = 29  # |E| < 30
MIN_PACKET_FLOW = 0
MAX_PACKET_FLOW = 100
CAPACITY_OPTIONS = [10, 100, 1000, 2500, 10000]  # Available capacity levels in bits/second
AVG_PACKET_SIZE = 1


class NetworkSimulator:

    def __init__(self):
        self.vertices = NUM_VERTICES
        self.edges = MAX_EDGES
        self.topology = None
        self.packet_flow_matrix = None  # N matrix
        self.path_dictionary = None
        self.edge_flow_matrix = None  # a(e) function
        self.edge_capacity_matrix = None  # c(e) function

    def create_packet_flow_matrix(self, scale_factor=1.0) -> np.ndarray:
        pfi_matrix = np.random.randint(
            MIN_PACKET_FLOW,
            int(MAX_PACKET_FLOW * scale_factor) + 1,
            size=(self.vertices, self.vertices)
        )
        # No self-loops allowed: n(i,i) = 0
        np.fill_diagonal(pfi_matrix, 0)
        return pfi_matrix

    def create_network_topology(self, additional_edges=0) -> np.ndarray:
        adjacency_matrix = np.zeros((self.vertices, self.vertices), dtype=int)

        for i in range(self.vertices - 1):
            adjacency_matrix[i][i + 1] = 1
            adjacency_matrix[i + 1][i] = 1

        current_edge_count = self.vertices - 1
        target_edges = min(self.edges + additional_edges,
                           self.vertices * (self.vertices - 1) // 2)

        while current_edge_count < target_edges:
            v1, v2 = np.random.randint(0, self.vertices, size=2)
            if v1 != v2 and adjacency_matrix[v1][v2] == 0:
                adjacency_matrix[v1][v2] = 1
                adjacency_matrix[v2][v1] = 1
                current_edge_count += 1

        return adjacency_matrix

    def extract_edge_list(self, adjacency_matrix: np.ndarray) -> List[Tuple[int, int]]:
        edges = []
        for i in range(len(adjacency_matrix)):
            for j in range(i + 1, len(adjacency_matrix)):
                if adjacency_matrix[i][j] == 1:
                    edges.append((i, j))
        return edges

    def find_shortest_path(self, source: int, destination: int) -> List[Tuple[int, int]]:
        graph = nx.from_numpy_array(self.topology, create_using=nx.DiGraph)
        try:
            path_vertices = nx.shortest_path(graph, source=source, target=destination)
            path_edges = [(path_vertices[i], path_vertices[i + 1])
                          for i in range(len(path_vertices) - 1)]
            return path_edges
        except nx.NetworkXNoPath:
            return []

    def compute_all_paths(self) -> Dict[Tuple[int, int], List[Tuple[int, int]]]:
        paths = {}
        for src in range(self.vertices):
            for dst in range(self.vertices):
                if src != dst:
                    path = self.find_shortest_path(src, dst)
                    if path:
                        paths[(src, dst)] = path
        return paths

    def calculate_edge_flows(self) -> np.ndarray:
        flow_matrix = np.zeros((self.vertices, self.vertices), dtype=int)

        for src in range(self.vertices):
            for dst in range(self.vertices):
                if src == dst:
                    continue

                path_edges = self.path_dictionary.get((src, dst), [])
                packet_flow = self.packet_flow_matrix[src][dst]

                # Add this flow to all edges in the path
                for (u, v) in path_edges:
                    flow_matrix[u][v] += packet_flow

        return flow_matrix

    def assign_edge_capacities(self, capacity_multiplier=1.0) -> np.ndarray:
        capacity_matrix = np.zeros((self.vertices, self.vertices), dtype=int)

        for i in range(self.vertices):
            for j in range(self.vertices):
                if i == j or self.topology[i][j] == 0:
                    continue

                required_capacity = self.edge_flow_matrix[i][j]
                # Find minimum capacity level that exceeds the flow
                suitable_capacities = [int(c * capacity_multiplier)
                                       for c in CAPACITY_OPTIONS
                                       if c * capacity_multiplier > required_capacity]

                if suitable_capacities:
                    capacity_matrix[i][j] = min(suitable_capacities)
                else:
                    # If no suitable capacity, use maximum available
                    capacity_matrix[i][j] = int(max(CAPACITY_OPTIONS) * capacity_multiplier)

        return capacity_matrix

    def compute_average_delay(self, topology: np.ndarray,
                              pfi_matrix: np.ndarray,
                              flow_matrix: np.ndarray,
                              capacity_matrix: np.ndarray) -> float:
        total_flow = pfi_matrix.sum()
        if total_flow == 0:
            return 0

        delay_sum = 0

        for i in range(self.vertices):
            for j in range(self.vertices):
                if i != j and topology[i][j] == 1:
                    flow = flow_matrix[i][j]
                    capacity = capacity_matrix[i][j]

                    denominator = capacity / AVG_PACKET_SIZE - flow
                    if denominator > 0:
                        delay_sum += flow / denominator
                    else:
                        return float('inf')

        return delay_sum / total_flow

    def simulate_link_failures(self, failure_probability: float) -> np.ndarray:
        failed_topology = np.copy(self.topology)

        for i in range(self.vertices):
            for j in range(i + 1, self.vertices):
                if self.topology[i][j] == 1 and np.random.random() > failure_probability:
                    failed_topology[i][j] = 0
                    failed_topology[j][i] = 0

        return failed_topology

    def estimate_reliability(self, link_reliability: float,
                             max_delay: float,
                             iterations: int = 1000) -> float:
        successful_scenarios = 0

        for _ in range(iterations):
            # Simulate link failures
            failed_topology = self.simulate_link_failures(link_reliability)

            # Recompute paths and flows for failed topology
            temp_sim = NetworkSimulator()
            temp_sim.topology = failed_topology
            temp_sim.packet_flow_matrix = self.packet_flow_matrix
            temp_sim.vertices = self.vertices

            temp_sim.path_dictionary = temp_sim.compute_all_paths()
            temp_sim.edge_flow_matrix = temp_sim.calculate_edge_flows()

            # Calculate delay for failed network
            delay = self.compute_average_delay(
                failed_topology,
                self.packet_flow_matrix,
                temp_sim.edge_flow_matrix,
                self.edge_capacity_matrix
            )

            if delay < max_delay:
                successful_scenarios += 1

        return successful_scenarios / iterations

    def visualize_network(self, title="Network Topology"):
        plt.figure(figsize=(12, 8))

        G = nx.from_numpy_array(self.topology)

        pos = nx.spring_layout(G, k=2, iterations=50)

        nx.draw_networkx_nodes(G, pos, node_color='lightblue',
                               node_size=500, alpha=0.9)

        edge_labels = {}
        for i in range(self.vertices):
            for j in range(i + 1, self.vertices):
                if self.topology[i][j] == 1:
                    flow = self.edge_flow_matrix[i][j] + self.edge_flow_matrix[j][i]
                    capacity = max(self.edge_capacity_matrix[i][j],
                                   self.edge_capacity_matrix[j][i])
                    edge_labels[(i, j)] = f"{flow}/{capacity}"

        nx.draw_networkx_edges(G, pos, alpha=0.5)
        nx.draw_networkx_labels(G, pos)
        nx.draw_networkx_edge_labels(G, pos, edge_labels, font_size=8)

        plt.title(title)
        plt.axis('off')
        plt.tight_layout()
        plt.show()

    def print_network_info(self):
        print("\n" + "=" * 60)
        print("NETWORK TOPOLOGY INFORMATION")
        print("=" * 60)
        print(f"Number of vertices: {self.vertices}")
        print(f"Number of edges: {len(self.extract_edge_list(self.topology))}")
        print(f"Total packet flow (G): {self.packet_flow_matrix.sum()}")

        G = nx.from_numpy_array(self.topology)
        print(f"Network is connected: {nx.is_connected(G)}")

        print("\nAdjacency Matrix:")
        print(self.topology)

        print("\nPacket Flow Statistics:")
        print(f"  Min flow: {self.packet_flow_matrix[self.packet_flow_matrix > 0].min()}")
        print(f"  Max flow: {self.packet_flow_matrix.max()}")
        print(f"  Average flow: {self.packet_flow_matrix[self.packet_flow_matrix > 0].mean():.2f}")

        print("\nEdge Capacity Statistics:")
        capacities = self.edge_capacity_matrix[self.edge_capacity_matrix > 0]
        print(f"  Min capacity: {capacities.min()}")
        print(f"  Max capacity: {capacities.max()}")
        print(f"  Average capacity: {capacities.mean():.2f}")

        base_delay = self.compute_average_delay(
            self.topology,
            self.packet_flow_matrix,
            self.edge_flow_matrix,
            self.edge_capacity_matrix
        )
        print(f"\nBase average packet delay (T): {base_delay:.4f}")
        print("=" * 60)


import matplotlib.pyplot as plt

def plot_scenario_1(scale_factors, avg_delays, reliabilities):
    fig, ax1 = plt.subplots()

    ax1.set_xlabel('Flow Intensity Scale Factor')
    ax1.set_ylabel('Average Packet Delay (T)', color='tab:blue')
    ax1.plot(scale_factors, avg_delays, 'b-o', label='Avg Delay')
    ax1.tick_params(axis='y', labelcolor='tab:blue')
    ax1.grid(True)

    ax2 = ax1.twinx()
    ax2.set_ylabel('Reliability (P(T < T_max))', color='tab:red')
    ax2.plot(scale_factors, reliabilities, 'r-o', label='Reliability')
    ax2.tick_params(axis='y', labelcolor='tab:red')

    plt.title('Scenario 1: Impact of Increasing Flow Intensity')
    fig.tight_layout()
    plt.show()


def plot_scenario_2(capacity_multipliers, avg_delays, reliabilities):
    fig, ax1 = plt.subplots()

    ax1.set_xlabel('Capacity Multiplier')
    ax1.set_ylabel('Average Packet Delay (T)', color='tab:blue')
    ax1.plot(capacity_multipliers, avg_delays, 'b-o', label='Avg Delay')
    ax1.tick_params(axis='y', labelcolor='tab:blue')
    ax1.grid(True)

    ax2 = ax1.twinx()
    ax2.set_ylabel('Reliability (P(T < T_max))', color='tab:red')
    ax2.plot(capacity_multipliers, reliabilities, 'r-o', label='Reliability')
    ax2.tick_params(axis='y', labelcolor='tab:red')

    plt.title('Scenario 2: Impact of Increasing Link Capacities')
    fig.tight_layout()
    plt.show()


def plot_scenario_3(edge_counts, avg_delays, reliabilities):
    fig, ax1 = plt.subplots()

    ax1.set_xlabel('Number of Edges')
    ax1.set_ylabel('Average Packet Delay (T)', color='tab:blue')
    ax1.plot(edge_counts, avg_delays, 'b-o', label='Avg Delay')
    ax1.tick_params(axis='y', labelcolor='tab:blue')
    ax1.grid(True)

    ax2 = ax1.twinx()
    ax2.set_ylabel('Reliability (P(T < T_max))', color='tab:red')
    ax2.plot(edge_counts, reliabilities, 'r-o', label='Reliability')
    ax2.tick_params(axis='y', labelcolor='tab:red')

    plt.title('Scenario 3: Impact of Adding New Edges')
    fig.tight_layout()
    plt.show()

def scenario_1_varying_flow_intensity(sim: NetworkSimulator):
    print("\n" + "=" * 60)
    print("SCENARIO 1: IMPACT OF INCREASING FLOW INTENSITIES")
    print("=" * 60)

    scale_factors = [0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0]
    reliabilities = []
    avg_delays = []

    # Save original packet flow matrix
    original_pfi = sim.packet_flow_matrix.copy()

    for scale in scale_factors:
        # Scale packet flow matrix
        sim.packet_flow_matrix = (original_pfi * scale).astype(int)

        # Recalculate flows
        sim.path_dictionary = sim.compute_all_paths()
        sim.edge_flow_matrix = sim.calculate_edge_flows()

        # Calculate base delay
        base_delay = sim.compute_average_delay(
            sim.topology,
            sim.packet_flow_matrix,
            sim.edge_flow_matrix,
            sim.edge_capacity_matrix
        )
        avg_delays.append(base_delay)

        # Estimate reliability
        reliability = sim.estimate_reliability(
            link_reliability=0.9,
            max_delay=10.0,
            iterations=100
        )
        reliabilities.append(reliability)

        print(f"Scale factor: {scale:4.2f} | "
              f"Total flow: {sim.packet_flow_matrix.sum():6d} | "
              f"Avg delay: {base_delay:6.3f} | "
              f"Reliability: {reliability:6.3f}")

    # Plot results
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

    ax1.plot(scale_factors, avg_delays, 'b-o')
    ax1.set_xlabel('Flow Intensity Scale Factor')
    ax1.set_ylabel('Average Packet Delay (T)')
    ax1.set_title('Delay vs Flow Intensity')
    ax1.grid(True)

    ax2.plot(scale_factors, reliabilities, 'r-o')
    ax2.set_xlabel('Flow Intensity Scale Factor')
    ax2.set_ylabel('Reliability P(T < T_max)')
    ax2.set_title('Reliability vs Flow Intensity')
    ax2.grid(True)

    plt.tight_layout()
    plt.savefig('plt_1.png')
    plt.show()

    # Restore original packet flow matrix
    sim.packet_flow_matrix = original_pfi
    sim.path_dictionary = sim.compute_all_paths()
    sim.edge_flow_matrix = sim.calculate_edge_flows()


def scenario_2_varying_capacities(sim: NetworkSimulator):
    """Scenario 2: Gradually increase link capacities"""
    print("\n" + "=" * 60)
    print("SCENARIO 2: IMPACT OF INCREASING LINK CAPACITIES")
    print("=" * 60)

    capacity_multipliers = [0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0]
    reliabilities = []
    avg_delays = []

    for multiplier in capacity_multipliers:
        # Scale capacities
        sim.edge_capacity_matrix = sim.assign_edge_capacities(multiplier)

        # Calculate base delay
        base_delay = sim.compute_average_delay(
            sim.topology,
            sim.packet_flow_matrix,
            sim.edge_flow_matrix,
            sim.edge_capacity_matrix
        )
        avg_delays.append(base_delay)

        # Estimate reliability
        reliability = sim.estimate_reliability(
            link_reliability=0.9,
            max_delay=10.0,
            iterations=100
        )
        reliabilities.append(reliability)

        avg_capacity = sim.edge_capacity_matrix[sim.edge_capacity_matrix > 0].mean()
        print(f"Capacity mult: {multiplier:4.2f} | "
              f"Avg capacity: {avg_capacity:7.1f} | "
              f"Avg delay: {base_delay:6.3f} | "
              f"Reliability: {reliability:6.3f}")

    # Plot results
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

    ax1.plot(capacity_multipliers, avg_delays, 'b-o')
    ax1.set_xlabel('Capacity Multiplier')
    ax1.set_ylabel('Average Packet Delay (T)')
    ax1.set_title('Delay vs Link Capacity')
    ax1.grid(True)

    ax2.plot(capacity_multipliers, reliabilities, 'r-o')
    ax2.set_xlabel('Capacity Multiplier')
    ax2.set_ylabel('Reliability P(T < T_max)')
    ax2.set_title('Reliability vs Link Capacity')
    ax2.grid(True)

    plt.tight_layout()
    plt.savefig('plt_2.png')
    plt.show()

    # Restore original capacities
    sim.edge_capacity_matrix = sim.assign_edge_capacities(1.0)


def scenario_3_adding_edges(sim: NetworkSimulator):
    """Scenario 3: Gradually add new edges to the topology"""
    print("\n" + "=" * 60)
    print("SCENARIO 3: IMPACT OF ADDING NEW EDGES")
    print("=" * 60)

    # Calculate average capacity for new edges
    existing_capacities = sim.edge_capacity_matrix[sim.edge_capacity_matrix > 0]
    avg_capacity = int(existing_capacities.mean())

    additional_edges = [0, 2, 4, 6, 8, 10]
    reliabilities = []
    avg_delays = []
    edge_counts = []

    # Save original topology
    original_topology = sim.topology.copy()

    for extra_edges in additional_edges:
        # Create new topology with additional edges
        sim.topology = sim.create_network_topology(extra_edges)

        # Recompute everything
        sim.path_dictionary = sim.compute_all_paths()
        sim.edge_flow_matrix = sim.calculate_edge_flows()

        # Assign capacities (use average for new edges)
        sim.edge_capacity_matrix = sim.assign_edge_capacities(1.0)

        # Set average capacity for newly added edges
        for i in range(sim.vertices):
            for j in range(sim.vertices):
                if sim.topology[i][j] == 1 and original_topology[i][j] == 0:
                    sim.edge_capacity_matrix[i][j] = avg_capacity

        # Count actual edges
        actual_edges = len(sim.extract_edge_list(sim.topology))
        edge_counts.append(actual_edges)

        # Calculate base delay
        base_delay = sim.compute_average_delay(
            sim.topology,
            sim.packet_flow_matrix,
            sim.edge_flow_matrix,
            sim.edge_capacity_matrix
        )
        avg_delays.append(base_delay)

        # Estimate reliability
        reliability = sim.estimate_reliability(
            link_reliability=0.9,
            max_delay=10.0,
            iterations=100
        )
        reliabilities.append(reliability)

        print(f"Extra edges: {extra_edges:3d} | "
              f"Total edges: {actual_edges:3d} | "
              f"Avg delay: {base_delay:6.3f} | "
              f"Reliability: {reliability:6.3f}")

    # Plot results
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

    ax1.plot(edge_counts, avg_delays, 'b-o')
    ax1.set_xlabel('Number of Edges')
    ax1.set_ylabel('Average Packet Delay (T)')
    ax1.set_title('Delay vs Network Connectivity')
    ax1.grid(True)

    ax2.plot(edge_counts, reliabilities, 'r-o')
    ax2.set_xlabel('Number of Edges')
    ax2.set_ylabel('Reliability P(T < T_max)')
    ax2.set_title('Reliability vs Network Connectivity')
    ax2.grid(True)

    plt.tight_layout()
    plt.savefig('plt_3.png')
    plt.show()

    # Restore original topology
    sim.topology = original_topology
    sim.path_dictionary = sim.compute_all_paths()
    sim.edge_flow_matrix = sim.calculate_edge_flows()
    sim.edge_capacity_matrix = sim.assign_edge_capacities(1.0)


def main():
    """Main program execution"""
    print("NETWORK RELIABILITY ANALYSIS TOOL")
    print("=================================")
    print(f"Configuration: {NUM_VERTICES} vertices, max {MAX_EDGES} edges")
    print(f"Packet flow range: [{MIN_PACKET_FLOW}, {MAX_PACKET_FLOW}]")
    print(f"Available capacities: {CAPACITY_OPTIONS}")

    # Initialize network simulator
    sim = NetworkSimulator()

    # Generate initial network configuration
    print("\nGenerating network topology...")
    sim.topology = sim.create_network_topology()

    print("Generating packet flow intensity matrix...")
    sim.packet_flow_matrix = sim.create_packet_flow_matrix()

    print("Computing routing paths...")
    sim.path_dictionary = sim.compute_all_paths()

    print("Calculating edge flows...")
    sim.edge_flow_matrix = sim.calculate_edge_flows()

    print("Assigning edge capacities...")
    sim.edge_capacity_matrix = sim.assign_edge_capacities()

    # Display network information
    sim.print_network_info()

    # Visualize network
    sim.visualize_network("Initial Network Topology (flow/capacity)")

    # Run analysis scenarios
    scenario_1_varying_flow_intensity(sim)
    scenario_2_varying_capacities(sim)
    scenario_3_adding_edges(sim)

    print("\nAnalysis complete!")


if __name__ == "__main__":
    main()