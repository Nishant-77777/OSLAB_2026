"""Generate the submission report PDF from the deterministic simulation outputs."""

from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import inch
from reportlab.platypus import Image, PageBreak, Paragraph, SimpleDocTemplate, Spacer, Table, TableStyle


ROOT = Path(__file__).parent
OUTPUT_DIR = ROOT / "outputs"
REPORT = ROOT / "routing_protocols_report.pdf"


def read_output(name: str) -> str:
    return (OUTPUT_DIR / name).read_text(encoding="utf-8")


def make_report() -> None:
    styles = getSampleStyleSheet()
    styles.add(ParagraphStyle(name="TitleCenter", parent=styles["Title"], alignment=TA_CENTER, spaceAfter=12))
    styles.add(ParagraphStyle(name="Small", parent=styles["BodyText"], fontSize=8.5, leading=11))
    document = SimpleDocTemplate(str(REPORT), pagesize=A4, rightMargin=42, leftMargin=42, topMargin=38, bottomMargin=38)
    story = [
        Paragraph("Routing Protocols Simulation in Python", styles["TitleCenter"]),
        Paragraph("Lab Assignment 7 | Computer Networks", styles["Heading2"]),
        Paragraph("This report documents four from-scratch simulations. The programs use graph data structures and implement the routing logic directly; NetworkX is used only to draw the topology diagrams.", styles["BodyText"]),
        Spacer(1, 10),
        Paragraph("1. Network topology and methodology", styles["Heading2"]),
        Paragraph("All experiments use a five-router topology. RIP counts every link as one hop, while OSPF and IS-IS use explicit link costs/metrics. BGP uses the same style of five-node graph, but each node represents an Autonomous System (AS). Every simulation is deterministic, so the output can be reproduced by running the supplied scripts.", styles["BodyText"]),
        Image(str(OUTPUT_DIR / "rip_topology.png"), width=3.6 * inch, height=2.35 * inch),
        Paragraph("Figure 1. Example topology used by the distance-vector experiment.", styles["Small"]),
        Spacer(1, 8),
        Paragraph("2. Protocol results", styles["Heading2"]),
        Paragraph("<b>RIP.</b> Each router begins with a distance of zero to itself, one to directly connected neighbours, and infinity elsewhere. Synchronous Bellman-Ford rounds exchange vectors until no value changes. This is simple but can require several rounds and sends an update over every adjacency in every round.", styles["BodyText"]),
        Paragraph("<b>OSPF.</b> Link-state advertisements synchronize the complete graph. Each router then independently runs Dijkstra from itself. OSPF converges through database synchronization plus one SPF calculation per router, and its weighted metric can represent latency or bandwidth.", styles["BodyText"]),
        Paragraph("<b>BGP.</b> Each UPDATE carries an AS path. A receiving AS rejects a route if its own AS number already appears in that path, preventing loops. The simulation chooses the shortest AS path and uses lexicographic order only as a deterministic tie-breaker.", styles["BodyText"]),
        Paragraph("<b>IS-IS.</b> Routers flood link-state information, synchronize a common database, and run Dijkstra using link metrics. Its control-plane behavior is therefore close to OSPF in this abstraction, although production IS-IS is carried directly over the data-link layer and supports hierarchical levels.", styles["BodyText"]),
        Spacer(1, 8),
        Paragraph("3. Routing-table snapshots", styles["Heading2"]),
    ]
    for title, filename in [("RIP output", "rip_output.txt"), ("OSPF output", "ospf_output.txt"), ("BGP output", "bgp_output.txt"), ("IS-IS output", "isis_output.txt")]:
        text = read_output(filename).replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\n", "<br/>")
        story.extend([Paragraph(title, styles["Heading3"]), Paragraph(text, styles["Small"]), Spacer(1, 5)])
    story.extend([
        PageBreak(),
        Paragraph("4. Convergence and overhead comparison", styles["Heading2"]),
        Table(
            [["Protocol", "Core algorithm", "Information exchanged", "Loop prevention / metric"],
             ["RIP", "Bellman-Ford", "Distance vectors", "Hop count; split-horizon is not modelled"],
             ["OSPF", "Dijkstra", "LSAs / full link-state DB", "Weighted link cost; SPF tree"],
             ["BGP", "Path-vector", "AS-path UPDATEs", "Reject path containing local AS"],
             ["IS-IS", "Dijkstra", "Flooded LSPs / full DB", "Weighted metric; SPF tree"]],
            colWidths=[0.8 * inch, 1.0 * inch, 1.65 * inch, 2.25 * inch],
            style=TableStyle([
                ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#1f4e79")),
                ("TEXTCOLOR", (0, 0), (-1, 0), colors.white),
                ("GRID", (0, 0), (-1, -1), 0.4, colors.grey),
                ("VALIGN", (0, 0), (-1, -1), "TOP"),
                ("FONTSIZE", (0, 0), (-1, -1), 8),
                ("LEADING", (0, 0), (-1, -1), 10),
                ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.white, colors.HexColor("#eef4f8")]),
            ]),
        ),
        Spacer(1, 12),
        Paragraph("RIP's overhead grows because every round sends complete vectors to neighbours. Link-state protocols exchange topology information and then compute locally, which usually gives faster, more predictable convergence but uses more memory and CPU. BGP intentionally prioritizes policy and loop-free AS paths over numeric shortest cost; this exercise uses shortest AS-path length as the requested simplified policy. IS-IS and OSPF have similar algorithmic behavior here, but differ in protocol encapsulation and deployment conventions.", styles["BodyText"]),
        Paragraph("5. Conclusion", styles["Heading2"]),
        Paragraph("The simulations demonstrate the central distinction between distance-vector, link-state, and path-vector routing. RIP learns indirectly from neighbours, OSPF and IS-IS build a synchronized map and calculate shortest paths, and BGP carries the complete AS path so a router can detect loops. The text outputs and PNG diagrams in the outputs folder are generated by the submitted source files, making the results auditable and repeatable.", styles["BodyText"]),
    ])
    document.build(story)


if __name__ == "__main__":
    make_report()
