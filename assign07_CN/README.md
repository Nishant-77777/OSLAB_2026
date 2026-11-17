# Lab Assignment 7: Routing Protocols Simulation

This folder contains a complete, reproducible submission for the routing-protocol
simulation assignment.

## Run the submission

From this folder, run:

```text
python rip_simulation.py
python ospf_simulation.py
python bgp_simulation.py
python isis_simulation.py
python generate_report.py
```

The four simulations write text results and topology diagrams into `outputs/`.
`generate_report.py` combines those results into `routing_protocols_report.pdf`.

The only third-party packages used are `networkx` for drawing graphs,
`matplotlib` for PNG figures, and `reportlab` for the PDF. No routing algorithm
is imported from a library.

## How to understand the assignment

### 1. RIP: distance vector

RIP stores a distance (number of hops) to each destination. A router does not
know the whole network; it learns from neighbours. The Bellman-Ford update is:

`distance(router, destination) = min over neighbours (1 + neighbour's distance)`

The program performs synchronous rounds. Using the previous round's tables makes
the convergence process visible and avoids accidental dependence on iteration
order.

### 2. OSPF: link state

OSPF distributes link-state advertisements (LSAs), so every router eventually
has the same topology database. Each router runs Dijkstra with the source router
as the root. Unlike RIP, the metric is a link cost rather than necessarily one
hop. The predecessor map printed by the program is the shortest-path tree.

### 3. BGP: path vector

BGP advertises an ordered AS path. When AS 65002 advertises a route to AS 65001,
the receiver prepends its own AS number. If a received path already contains the
receiver's AS number, the update is discarded. That single rule prevents routing
loops in this simplified model. The selected route is the shortest AS path.

### 4. IS-IS: link state

IS-IS floods link-state information and then uses Dijkstra, just as the exercise
requests. The simulation intentionally models the algorithmic idea rather than
every production feature. In real networks, IS-IS is carried directly over the
data-link layer and supports Level-1/Level-2 hierarchy.

## Files

* `rip_simulation.py`: Bellman-Ford distance-vector simulation.
* `ospf_simulation.py`: LSA synchronization and Dijkstra SPF.
* `bgp_simulation.py`: AS-path UPDATE propagation and loop prevention.
* `isis_simulation.py`: LSP flooding and Dijkstra SPF.
* `generate_report.py`: creates the PDF report.
* `outputs/`: generated routing tables, topology diagrams, and screenshot-style
  output images.
