#!/usr/bin/env python3
"""Summarize decomp progress, append history, and write badges/chart for the README."""

from __future__ import annotations

import csv
import json
import os
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Optional, Tuple

SECTIONS = ("init", "game", "debugger")

# Paths are relative to the conker/ code build directory (cwd when invoked via make).
CSV_DIR = Path(".")
REPO_ROOT = Path("..")
PROGRESS_DIR = REPO_ROOT / "progress"
HISTORY_PATH = PROGRESS_DIR / "history.csv"
LATEST_PATH = PROGRESS_DIR / "latest.json"
CHART_PATH = PROGRESS_DIR / "chart.svg"

HISTORY_FIELDS = [
    "timestamp",
    "commit",
    "total_percent",
    "init_percent",
    "game_percent",
    "debugger_percent",
    "total_c_bytes",
    "total_bytes",
    "total_c_funcs",
    "total_funcs",
    "init_c_bytes",
    "init_bytes",
    "init_c_funcs",
    "init_funcs",
    "game_c_bytes",
    "game_bytes",
    "game_c_funcs",
    "game_funcs",
    "debugger_c_bytes",
    "debugger_bytes",
    "debugger_c_funcs",
    "debugger_funcs",
]

# Chart series: (history key, label, stroke color)
CHART_SERIES = [
    ("total_percent", "total", "#4c78a8"),
    ("init_percent", "init", "#f58518"),
    ("game_percent", "game", "#e45756"),
    ("debugger_percent", "debugger", "#54a24b"),
]


def percent(numerator: int, denominator: int) -> float:
    if denominator == 0:
        return 0.0
    return (numerator / denominator) * 100.0


def format_percent(value: float) -> str:
    return f"{value:.2f}"


def badge_color(pct: float) -> str:
    """Green-yellow-red gradient similar to other decomp badges."""
    green = min(255, round(min(1.0, (pct / 100.0) * 2) * 192))
    red = min(255, round(min(1.0, ((100.0 - pct) / 100.0) * 2) * 192))
    return f"#{red:02x}{green:02x}00"


def read_section_csv(path: Path) -> Dict[str, int]:
    stats = {"c_bytes": 0, "bytes": 0, "c_funcs": 0, "funcs": 0}
    with path.open(newline="") as handle:
        reader = csv.DictReader(handle)
        for row in reader:
            length = int(row["length"])
            stats["bytes"] += length
            stats["funcs"] += 1
            if row["language"] == "c":
                stats["c_bytes"] += length
                stats["c_funcs"] += 1
    return stats


def collect_stats() -> Dict[str, Dict[str, int]]:
    sections: Dict[str, Dict[str, int]] = {}
    for section in SECTIONS:
        path = CSV_DIR / f"progress.{section}.csv"
        if not path.is_file():
            print(f"Missing {path}; run `make progress` first", file=sys.stderr)
            sys.exit(1)
        sections[section] = read_section_csv(path)

    total = {"c_bytes": 0, "bytes": 0, "c_funcs": 0, "funcs": 0}
    for section_stats in sections.values():
        for key in total:
            total[key] += section_stats[key]
    sections["total"] = total
    return sections


def git_commit(repo_root: Path) -> str:
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--short=12", "HEAD"],
            cwd=repo_root,
            check=True,
            capture_output=True,
            text=True,
        )
        return result.stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return "unknown"


def snapshot_row(sections: Dict[str, Dict[str, int]], commit: str, timestamp: str) -> Dict[str, str]:
    row: Dict[str, str] = {
        "timestamp": timestamp,
        "commit": commit,
    }
    for name in ("total",) + SECTIONS:
        stats = sections[name]
        row[f"{name}_percent"] = format_percent(percent(stats["c_bytes"], stats["bytes"]))
        row[f"{name}_c_bytes"] = str(stats["c_bytes"])
        row[f"{name}_bytes"] = str(stats["bytes"])
        row[f"{name}_c_funcs"] = str(stats["c_funcs"])
        row[f"{name}_funcs"] = str(stats["funcs"])
    return row


def load_history() -> List[Dict[str, str]]:
    if not HISTORY_PATH.is_file():
        return []
    with HISTORY_PATH.open(newline="") as handle:
        return list(csv.DictReader(handle))


def metrics_equal(a: Dict[str, str], b: Dict[str, str]) -> bool:
    keys = [
        "total_percent",
        "init_percent",
        "game_percent",
        "debugger_percent",
        "total_c_bytes",
        "total_bytes",
        "total_c_funcs",
        "total_funcs",
    ]
    return all(a.get(key) == b.get(key) for key in keys)


def write_history(rows: List[Dict[str, str]]) -> None:
    PROGRESS_DIR.mkdir(parents=True, exist_ok=True)
    with HISTORY_PATH.open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=HISTORY_FIELDS)
        writer.writeheader()
        for row in rows:
            writer.writerow({field: row.get(field, "") for field in HISTORY_FIELDS})


def write_latest_json(sections: Dict[str, Dict[str, int]], commit: str, timestamp: str) -> None:
    payload = {
        "timestamp": timestamp,
        "commit": commit,
        "sections": {},
    }
    for name in ("total",) + SECTIONS:
        stats = sections[name]
        payload["sections"][name] = {
            "percent": round(percent(stats["c_bytes"], stats["bytes"]), 4),
            "c_bytes": stats["c_bytes"],
            "bytes": stats["bytes"],
            "c_funcs": stats["c_funcs"],
            "funcs": stats["funcs"],
        }
    PROGRESS_DIR.mkdir(parents=True, exist_ok=True)
    LATEST_PATH.write_text(json.dumps(payload, indent=2) + "\n")


def text_width(text: str, font_size: int = 11) -> float:
    # Approximate DejaVu Sans / Verdana average character width.
    return len(text) * font_size * 0.55 + 10


def write_badge(path: Path, label: str, message: str, color: str) -> None:
    label_width = max(40.0, text_width(label))
    message_width = max(40.0, text_width(message))
    total_width = label_width + message_width
    label_x = label_width / 2
    message_x = label_width + message_width / 2

    svg = f"""<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" width="{total_width:.0f}" height="20" role="img" aria-label="{label}: {message}">
  <title>{label}: {message}</title>
  <linearGradient id="s" x2="0" y2="100%">
    <stop offset="0" stop-color="#bbb" stop-opacity=".1"/>
    <stop offset="1" stop-opacity=".1"/>
  </linearGradient>
  <clipPath id="r">
    <rect width="{total_width:.0f}" height="20" rx="3" fill="#fff"/>
  </clipPath>
  <g clip-path="url(#r)">
    <rect width="{label_width:.0f}" height="20" fill="#555"/>
    <rect x="{label_width:.0f}" width="{message_width:.0f}" height="20" fill="{color}"/>
    <rect width="{total_width:.0f}" height="20" fill="url(#s)"/>
  </g>
  <g fill="#fff" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11">
    <text x="{label_x:.1f}" y="15" fill="#010101" fill-opacity=".3">{label}</text>
    <text x="{label_x:.1f}" y="14">{label}</text>
    <text x="{message_x:.1f}" y="15" fill="#010101" fill-opacity=".3">{message}</text>
    <text x="{message_x:.1f}" y="14">{message}</text>
  </g>
</svg>
"""
    path.write_text(svg)


def write_badges(sections: Dict[str, Dict[str, int]]) -> None:
    PROGRESS_DIR.mkdir(parents=True, exist_ok=True)
    labels = {
        "total": "Conker's Bad Fur Day (US)",
        "init": "init",
        "game": "game",
        "debugger": "debugger",
    }
    for name, label in labels.items():
        stats = sections[name]
        pct = percent(stats["c_bytes"], stats["bytes"])
        message = f"{format_percent(pct)}% | {stats['c_funcs']}/{stats['funcs']} funcs"
        write_badge(PROGRESS_DIR / f"badge_{name}.svg", label, message, badge_color(pct))


def parse_timestamp(value: str) -> datetime:
    if value.endswith("Z"):
        value = value[:-1] + "+00:00"
    return datetime.fromisoformat(value)


def chart_points(
    rows: List[Dict[str, str]],
    key: str,
    left: float,
    top: float,
    width: float,
    height: float,
    y_max: float,
) -> List[Tuple[float, float]]:
    if not rows:
        return []
    if len(rows) == 1:
        x = left + width / 2
        y = top + height - (float(rows[0][key]) / y_max) * height
        return [(x, y)]

    times = [parse_timestamp(row["timestamp"]).timestamp() for row in rows]
    t_min = min(times)
    t_max = max(times)
    span = t_max - t_min if t_max > t_min else 1.0

    points: List[Tuple[float, float]] = []
    for row, t in zip(rows, times):
        x = left + ((t - t_min) / span) * width
        y = top + height - (float(row[key]) / y_max) * height
        points.append((x, y))
    return points


def polyline(points: List[Tuple[float, float]]) -> str:
    return " ".join(f"{x:.2f},{y:.2f}" for x, y in points)


def write_chart(rows: List[Dict[str, str]]) -> None:
    width = 760
    height = 320
    margin_left = 48
    margin_right = 24
    margin_top = 24
    margin_bottom = 56
    plot_left = margin_left
    plot_top = margin_top
    plot_width = width - margin_left - margin_right
    plot_height = height - margin_top - margin_bottom

    y_max = 100.0
    if rows:
        observed = max(float(row[key]) for row in rows for key, _, _ in CHART_SERIES)
        y_max = max(10.0, min(100.0, (int(observed / 5) + 2) * 5))

    lines: List[str] = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img" aria-label="Decomp progress over time">',
        "  <title>Decomp progress over time</title>",
        '  <rect width="100%" height="100%" fill="#ffffff"/>',
        f'  <rect x="{plot_left}" y="{plot_top}" width="{plot_width}" height="{plot_height}" fill="#fafafa" stroke="#dddddd"/>',
    ]

    # Horizontal grid lines and y-axis labels.
    for i in range(5):
        frac = i / 4
        y = plot_top + plot_height * (1 - frac)
        value = y_max * frac
        lines.append(
            f'  <line x1="{plot_left}" y1="{y:.2f}" x2="{plot_left + plot_width}" y2="{y:.2f}" stroke="#eeeeee"/>'
        )
        lines.append(
            f'  <text x="{plot_left - 8}" y="{y + 4:.2f}" text-anchor="end" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11" fill="#666666">{value:.0f}%</text>'
        )

    # X-axis labels (first / middle / last).
    if rows:
        label_indexes = sorted({0, len(rows) // 2, len(rows) - 1})
        times = [parse_timestamp(row["timestamp"]).timestamp() for row in rows]
        t_min = min(times)
        t_max = max(times)
        span = t_max - t_min if t_max > t_min else 1.0
        for index in label_indexes:
            if len(rows) == 1:
                x = plot_left + plot_width / 2
            else:
                x = plot_left + ((times[index] - t_min) / span) * plot_width
            label = parse_timestamp(rows[index]["timestamp"]).strftime("%Y-%m-%d")
            lines.append(
                f'  <text x="{x:.2f}" y="{plot_top + plot_height + 20}" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11" fill="#666666">{label}</text>'
            )

    for key, label, color in CHART_SERIES:
        points = chart_points(rows, key, plot_left, plot_top, plot_width, plot_height, y_max)
        if not points:
            continue
        if len(points) == 1:
            x, y = points[0]
            lines.append(f'  <circle cx="{x:.2f}" cy="{y:.2f}" r="4" fill="{color}"/>')
        else:
            lines.append(
                f'  <polyline fill="none" stroke="{color}" stroke-width="2" points="{polyline(points)}"/>'
            )
            x, y = points[-1]
            lines.append(f'  <circle cx="{x:.2f}" cy="{y:.2f}" r="3" fill="{color}"/>')

    # Legend
    legend_x = plot_left
    legend_y = height - 18
    for key, label, color in CHART_SERIES:
        lines.append(f'  <rect x="{legend_x}" y="{legend_y - 9}" width="12" height="12" fill="{color}"/>')
        lines.append(
            f'  <text x="{legend_x + 16}" y="{legend_y}" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="12" fill="#333333">{label}</text>'
        )
        legend_x += 90

    lines.append("</svg>")
    lines.append("")
    CHART_PATH.write_text("\n".join(lines))


def print_summary(sections: Dict[str, Dict[str, int]]) -> None:
    for name in ("total",) + SECTIONS:
        stats = sections[name]
        pct = percent(stats["c_bytes"], stats["bytes"])
        print(
            f"{name}: {format_percent(pct)}% bytes "
            f"({stats['c_bytes']}/{stats['bytes']}), "
            f"funcs {stats['c_funcs']}/{stats['funcs']}"
        )


def main() -> None:
    sections = collect_stats()
    commit = git_commit(REPO_ROOT)
    timestamp = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    row = snapshot_row(sections, commit, timestamp)

    history = load_history()
    if history and history[-1].get("commit") == commit and metrics_equal(history[-1], row):
        # Same commit and same metrics: refresh generated artifacts only.
        pass
    elif history and metrics_equal(history[-1], row):
        # Metrics unchanged: update commit/timestamp on the latest row.
        history[-1] = row
    else:
        history.append(row)

    write_history(history)
    write_latest_json(sections, commit, timestamp)
    write_badges(sections)
    write_chart(history)
    print_summary(sections)
    print(f"Wrote {HISTORY_PATH}, {LATEST_PATH}, badges, and {CHART_PATH}")


if __name__ == "__main__":
    main()
