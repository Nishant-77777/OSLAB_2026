"""IS-IS simulation: link-state flooding followed by Dijkstra SPF."""

from heapq import heappop, heappush
from pathlib import Path

import matplotlib.pyplot as plt
import networkx as nx


OUTPUT_DIR = Path(__file__).parent / "outputs"
TOPOLOGY = {
    "R1": {"R2": 10, "R3": 30},
    "R2": {"R1": 10, "R3": 5, "R4": 20},
    "R3": {"R1": 30, "R2": 5, "R4": 10, "R5": 8},
    "R4": {"R2": 20, "R3": 10, "R5": 6},
    "R5": {"R3": 8, "R4": 6},
}


def shortest_paths(topology: dict[str, dict[str, int]], source: str) -> dict[str, int]:
    distances = {router: float("inf") for router in topology}
    distances[source] = 0
    queue = [(0, source)]
    while queue:
        distance, router = heappop(queue)
        if distance != distances[router]:
            continue
        for neighbour, metric in topology[router].items():
            candidate = distance + metric
            if candidate < distances[neighbour]:
                distances[neighbour] = candidate
                heappush(queue, (candidate, neighbour))
    return distances


def flooding_messages(topology: dict[str, dict[str, int]]) -> int:
    """Count one reliable LSP forwarding event over each directed adjacency."""
    return sum(len(neighbours) for neighbours in topology.values())


def save_topology_image(topology: dict[str, dict[str, int]]) -> None:
    graph = nx.Graph()
    for router, neighbours in topology.items():
        for neighbour, metric in neighbours.items():
            graph.add_edge(router, neighbour, weight=metric)
    positions = nx.spring_layout(graph, seed=7)
    nx.draw_networkx(graph, positions, node_color="#d1c4e9", node_size=1500, font_weight="bold")
    nx.draw_networkx_edge_labels(graph, positions, edge_labels=nx.get_edge_attributes(graph, "weight"))
    plt.title("IS-IS topology (link metrics)")
    plt.axis("off")
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "isis_topology.png", dpi=160)
    plt.close()


def main() -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)
    lines = [
        "IS-IS SIMULATION - LSP FLOODING + DIJKSTRA",
        f"LSP forwarding events modelled: {flooding_messages(TOPOLOGY)}",
        "Each router synchronizes the complete link-state database before SPF.",
        "",
    ]
    for source in sorted(TOPOLOGY):
        distances = shortest_paths(TOPOLOGY, source)
        table = ", ".join(f"{destination}: metric {distances[destination]}"
                          for destination in sorted(distances))
        lines.append(f"Router {source} routing table: {table}")
    output = "\n".join(lines)
    print(output)
    (OUTPUT_DIR / "isis_output.txt").write_text(output + "\n", encoding="utf-8")
    save_topology_image(TOPOLOGY)


if __name__ == "__main__":
    main()
