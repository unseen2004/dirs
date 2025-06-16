import networkx as nx
import numpy as np
import random

# Set random seed for reproducibility
random.seed(42)
np.random.seed(42)

# Create a directed graph with 20 vertices
G = nx.DiGraph()
G.add_nodes_from(range(1, 21))

# Create a ring structure to ensure basic connectivity with no isolated vertices
for i in range(1, 20):
    G.add_edge(i, i + 1)
G.add_edge(20, 1)  # Complete the ring

# Add some random edges to make the graph more interesting, but keep |E| < 30
additional_edges = random.randint(5, 9)  # We already have 20 edges, so add up to 9 more
edges_added = 0

while edges_added < additional_edges:
    i = random.randint(1, 20)
    j = random.randint(1, 20)
    if i != j and not G.has_edge(i, j):
        G.add_edge(i, j)
        edges_added += 1

# Create the traffic intensity matrix N
N = np.zeros((20, 20))
for i in range(20):
    for j in range(20):
        if i != j:
            # Assign random traffic values between 0 and 10 packets/second
            N[i, j] = round(random.uniform(0, 10), 2)

# Define capacity and flow functions
capacities = {}  # Function c: maximum bits/second
flows = {}  # Function a: actual packets/second

for u, v in G.edges():
    # Assign random capacity between 100 and 500 bits/second
    capacity = random.randint(100, 500)
    capacities[(u, v)] = capacity

    # Make sure the flow is less than capacity (c(e) > a(e))
    flows[(u, v)] = random.randint(10, capacity - 10)


# Function to print the graph in ASCII format for terminal display
def print_graph():
    num_vertices = G.number_of_nodes()
    num_edges = G.number_of_edges()

    print(f"Graph G with |V|={num_vertices}, |E|={num_edges}:")
    print("\nAdjacency Matrix:")

    # Create adjacency matrix for visualization
    adj_matrix = np.zeros((20, 20), dtype=int)
    for u, v in G.edges():
        adj_matrix[u - 1][v - 1] = 1

    # Print column headers
    print("   ", end="")
    for j in range(1, 21):
        print(f"{j:2d} ", end="")
    print()

    # Print matrix with row headers
    for i in range(20):
        print(f"{i + 1:2d} ", end="")
        for j in range(20):
            print(f"{adj_matrix[i][j]:2d} ", end="")
        print()

    print("\nEdges with Capacity c(e) and Flow a(e):")
    print("Edge\t\tCapacity c(e)\tFlow a(e)")
    print("-" * 40)

    for u, v in sorted(G.edges()):
        print(f"{u} -> {v}\t\t{capacities[(u, v)]}\t\t{flows[(u, v)]}")


# Function to print the traffic intensity matrix N
def print_matrix_N():
    print("\nTraffic Intensity Matrix N (sample of entries):")
    print("Source v(i)\tDestination v(j)\tIntensity n(i,j)")
    print("-" * 50)

    # Print first 15 non-zero entries as a sample
    count = 0
    for i in range(20):
        for j in range(20):
            if N[i, j] > 0 and count < 15:
                print(f"{i + 1}\t\t{j + 1}\t\t\t{N[i, j]:.2f}")
                count += 1
    print("... (remaining entries omitted for brevity)")


# Execute the visualization
print_graph()
print_matrix_N()

# Verify that the requirements are met
isolated_vertices = [node for node in G.nodes() if G.degree(node) == 0]
print(f"\nIsolated vertices: {isolated_vertices}")  # Should be empty list

all_valid = all(capacities[(u, v)] > flows[(u, v)] for u, v in G.edges())
print(f"All edges satisfy c(e) > a(e): {all_valid}")  # Should be True

# Optional: summary statistics
total_flow = sum(flows.values())
total_capacity = sum(capacities.values())
print(f"\nTotal network flow: {total_flow}")
print(f"Total network capacity: {total_capacity}")
print(f"Network utilization: {total_flow / total_capacity:.2%}")