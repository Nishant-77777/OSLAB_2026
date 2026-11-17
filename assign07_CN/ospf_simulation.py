"""OSPF link-state simulation using LSA flooding and Dijkstra's algorithm."""

from heapq import heappop, heappush
from pathlib import Path

import matplotlib.pyplot as plt
import networkx as nx


OUTPUT_DIR = Path(__file__).parent / "outputs"
TOPOLOGY = {
    "A": {"B": 2, "C": 5},
    "B": {"A": 2, "C": 1, "D": 2},
    "C": {"A": 5, "B": 1, "D": 3, "E": 1},
    "D": {"B": 2, "C": 3, "E": 2},
    "E": {"C": 1, "D": 2},
}


def dijkstra(topology: dict[str, dict[str, int]], source: str) -> tuple[dict[str, int], dict[str, str | None]]:
    """Return shortest distances and predecessor tree rooted at source."""
    distances = {router: float("inf") for router in topology}
    parents: dict[str, str | None] = {router: None for router in topology}
    distances[source] = 0
    queue = [(0, source)]
    while queue:
        distance, router = heappop(queue)
        if distance != distances[router]:
            continue
        for neighbour, cost in topology[router].items():
            candidate = distance + cost
            if candidate < distances[neighbour]:
                distances[neighbour] = candidate
                parents[neighbour] = router
                heappush(queue, (candidate, neighbour))
    return distances, parents


def save_topology_image(topology: dict[str, dict[str, int]]) -> None:
    graph = nx.Graph()
    for router, neighbours in topology.items():
        for neighbour, cost in neighbours.items():
            graph.add_edge(router, neighbour, weight=cost)
    positions = nx.spring_layout(graph, seed=7)
    nx.draw_networkx(graph, positions, node_color="#c8e6c9", node_size=1500, font_weight="bold")
    nx.draw_networkx_edge_labels(graph, positions, edge_labels=nx.get_edge_attributes(graph, "weight"))
    plt.title("OSPF topology (link costs)")
    plt.axis("off")
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "ospf_topology.png", dpi=160)
    plt.close()


def main() -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)
    routers = sorted(TOPOLOGY)
    lsa_messages = sum(len(neighbours) for neighbours in TOPOLOGY.values())
    lines = [
        "OSPF SIMULATION - LINK STATE FLOODING + DIJKSTRA",
        f"LSA transmissions modelled: {lsa_messages}",
        "All routers have a synchronized link-state database before SPF runs.",
        "",
    ]
    for source in routers:
        distances, parents = dijkstra(TOPOLOGY, source)
        tree = ", ".join(f"{node}<-{parent}" for node, parent in parents.items() if parent)
        table = ", ".join(f"{node}: cost {distances[node]}" for node in routers)
        lines.extend([f"Router {source} shortest-path tree: {tree}", f"  Routing table: {table}", ""])
    output = "\n".join(lines).rstrip()
    print(output)
    (OUTPUT_DIR / "ospf_output.txt").write_text(output + "\n", encoding="utf-8")
    save_topology_image(TOPOLOGY)


if __name__ == "__main__":
    main()
