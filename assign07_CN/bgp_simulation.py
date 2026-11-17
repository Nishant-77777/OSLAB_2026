"""Simplified BGP path-vector simulation with AS-path loop prevention."""

from pathlib import Path

import matplotlib.pyplot as plt
import networkx as nx


OUTPUT_DIR = Path(__file__).parent / "outputs"
TOPOLOGY = {
    65001: {65002, 65003},
    65002: {65001, 65003, 65004},
    65003: {65001, 65002, 65004, 65005},
    65004: {65002, 65003, 65005},
    65005: {65003, 65004},
}


def simulate_bgp(topology: dict[int, set[int]]) -> tuple[dict[int, dict[int, list[int]]], int, int]:
    """Propagate paths synchronously; reject UPDATEs containing the local AS."""
    ases = sorted(topology)
    routes = {asn: {asn: [asn]} for asn in ases}
    rounds = 0
    updates = 0
    while True:
        rounds += 1
        previous = {asn: {destination: path[:] for destination, path in table.items()}
                    for asn, table in routes.items()}
        updated = {asn: {destination: path[:] for destination, path in table.items()}
                   for asn, table in previous.items()}
        for local_as in ases:
            for neighbour in sorted(topology[local_as]):
                updates += 1
                for destination, path in previous[neighbour].items():
                    if local_as in path:  # BGP's fundamental loop-prevention rule.
                        continue
                    candidate = [local_as] + path
                    current = updated[local_as].get(destination)
                    if current is None or (len(candidate), candidate) < (len(current), current):
                        updated[local_as][destination] = candidate
        routes = updated
        if routes == previous:
            break
    return routes, rounds, updates


def save_topology_image(topology: dict[int, set[int]]) -> None:
    graph = nx.Graph()
    for asn, neighbours in topology.items():
        for neighbour in neighbours:
            graph.add_edge(asn, neighbour)
    positions = nx.spring_layout(graph, seed=7)
    nx.draw_networkx(graph, positions, node_color="#ffe0b2", node_size=1700, font_weight="bold")
    plt.title("BGP AS-level topology")
    plt.axis("off")
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "bgp_topology.png", dpi=160)
    plt.close()


def main() -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)
    routes, rounds, updates = simulate_bgp(TOPOLOGY)
    lines = [
        "BGP SIMULATION - SIMPLIFIED PATH VECTOR",
        f"Converged after {rounds} synchronous rounds.",
        f"BGP UPDATE exchanges modelled: {updates}",
        "Best route is selected by shortest AS-path; ties use lexicographic order.",
        "",
    ]
    for asn in sorted(routes):
        entries = ", ".join(f"AS{destination}: {' '.join(f'AS{x}' for x in path)}"
                            for destination, path in sorted(routes[asn].items()))
        lines.append(f"AS{asn}: {entries}")
    output = "\n".join(lines)
    print(output)
    (OUTPUT_DIR / "bgp_output.txt").write_text(output + "\n", encoding="utf-8")
    save_topology_image(TOPOLOGY)


if __name__ == "__main__":
    main()
