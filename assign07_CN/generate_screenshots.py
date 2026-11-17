"""Render terminal-style screenshots of each program's saved output."""

from pathlib import Path

import matplotlib.pyplot as plt


ROOT = Path(__file__).parent
OUTPUT_DIR = ROOT / "outputs"


def render(name: str, title: str) -> None:
    text = (OUTPUT_DIR / f"{name}_output.txt").read_text(encoding="utf-8")
    lines = text.splitlines()
    height = max(3.5, 0.24 * len(lines) + 0.8)
    figure, axis = plt.subplots(figsize=(13, height))
    axis.set_facecolor("#101820")
    figure.patch.set_facecolor("#101820")
    axis.text(0.02, 0.96, title, color="#7ee787", fontsize=13, family="DejaVu Sans Mono",
              va="top", transform=axis.transAxes, weight="bold")
    axis.text(0.02, 0.90, "\n".join(lines), color="#f0f6fc", fontsize=10.5,
              family="DejaVu Sans Mono", va="top", linespacing=1.35, transform=axis.transAxes)
    axis.axis("off")
    figure.tight_layout(pad=0.4)
    figure.savefig(OUTPUT_DIR / f"{name}_output_screenshot.png", dpi=150, facecolor=figure.get_facecolor())
    plt.close(figure)


def main() -> None:
    for name, title in [
        ("rip", "Terminal output: RIP"),
        ("ospf", "Terminal output: OSPF"),
        ("bgp", "Terminal output: BGP"),
        ("isis", "Terminal output: IS-IS"),
    ]:
        render(name, title)


if __name__ == "__main__":
    main()
