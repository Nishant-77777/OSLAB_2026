"""RIP distance-vector simulation using the Bellman-Ford update rule.

The program uses hop count as the metric.  Every router starts knowing only
itself and its directly connected neighbours, then exchanges a copy of its
distance vector with each neighbour until no table changes.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import networkx as nx


OUTPUT_DIR = Path(__file__).parent / "outputs"
TOPOLOGY = {
    "A": {"B", "C"},
    "B": {"A", "C", "D"},
    "C": {"A", "B", "D", "E"},
    "D": {"B", "C", "E"},
    "E": {"C", "D"},
}


def simulate_rip(topology: dict[str, set[str]]) -> tuple[dict[str, dict[str, int]], int, int]:
    """Run synchronous Bellman-Ford rounds and return tables and counters."""
    routers = sorted(topology)
    infinity = 999
    tables = {
        router: {destination: (0 if router == destination else 1 if destination in topology[router] else infinity)
                 for destination in routers}
        for router in routers
    }
    rounds = 0
    messages = 0

    while True:
        rounds += 1
        previous = {router: values.copy() for router, values in tables.items()}
        updated = {router: values.copy() for router, values in previous.items()}
        for router in routers:
            for neighbour in sorted(topology[router]):
                messages += 1
                for destination in routers:
                    candidate = previous[neighbour][destination] + 1
                    if candidate < updated[router][destination]:
                        updated[router][destination] = candidate
        tables = updated
        if tables == previous:
            break
    return tables, rounds, messages


def save_topology_image(topology: dict[str, set[str]]) -> None:
    graph = nx.Graph()
    for router, neighbours in topology.items():
        for neighbour in neighbours:
            graph.add_edge(router, neighbour)
    positions = nx.spring_layout(graph, seed=7)
    nx.draw_networkx(graph, positions, node_color="#b7d7f0", node_size=1500, font_weight="bold")
    plt.title("RIP topology (metric: hop count)")
    plt.axis("off")
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "rip_topology.png", dpi=160)
    plt.close()


def main() -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)
    tables, rounds, messages = simulate_rip(TOPOLOGY)
    lines = [
        "RIP SIMULATION - BELLMAN-FORD DISTANCE VECTOR",
        f"Converged after {rounds} synchronous rounds.",
        f"Routing-update messages sent: {messages}",
        "",
    ]
    for router in sorted(tables):
        formatted = ", ".join(f"{destination}: {cost} hop(s)" for destination, cost in tables[router].items())
        lines.append(f"Router {router}: {formatted}")
    output = "\n".join(lines)
    print(output)
    (OUTPUT_DIR / "rip_output.txt").write_text(output + "\n", encoding="utf-8")
    save_topology_image(TOPOLOGY)


if __name__ == "__main__":
    main()
