#!/usr/bin/env python3
"""Validate an audioBlade Profile v0 JSON file.

This is a Phase 0 diagnostics helper. It validates profile files only; it does
not open devices, execute routes, or enter the real-time audio path.
"""

from __future__ import annotations

import json
import sys
from pathlib import Path
from typing import Any


REPO_ROOT = Path(__file__).resolve().parents[2]
SCHEMA_PATH = REPO_ROOT / "control" / "profiles" / "profile.schema.json"


def load_json(path: Path) -> Any:
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def validate_basic_profile(profile: dict[str, Any]) -> None:
    require(profile.get("schemaVersion") == "0.1.0", "schemaVersion must be 0.1.0")
    require(isinstance(profile.get("profileId"), str) and profile["profileId"], "profileId is required")
    require(isinstance(profile.get("name"), str) and profile["name"], "name is required")

    nodes = profile.get("nodes")
    routes = profile.get("routes")
    require(isinstance(nodes, list) and len(nodes) >= 2, "nodes must contain at least two entries")
    require(isinstance(routes, list) and len(routes) >= 1, "routes must contain at least one entry")

    node_ids: set[str] = set()
    port_ids_by_node: dict[str, set[str]] = {}

    for node in nodes:
        require(isinstance(node, dict), "each node must be an object")
        node_id = node.get("nodeId")
        require(isinstance(node_id, str) and node_id, "node.nodeId is required")
        require(node_id not in node_ids, f"duplicate nodeId: {node_id}")
        node_ids.add(node_id)

        ports = node.get("ports")
        require(isinstance(ports, list) and ports, f"node {node_id} must contain ports")
        port_ids: set[str] = set()
        for port in ports:
            require(isinstance(port, dict), f"ports on {node_id} must be objects")
            port_id = port.get("portId")
            require(isinstance(port_id, str) and port_id, f"portId is required on {node_id}")
            require(port_id not in port_ids, f"duplicate portId on {node_id}: {port_id}")
            port_ids.add(port_id)
        port_ids_by_node[node_id] = port_ids

    for route in routes:
        require(isinstance(route, dict), "each route must be an object")
        for endpoint_name in ("source", "destination"):
            endpoint = route.get(endpoint_name)
            require(isinstance(endpoint, dict), f"route.{endpoint_name} must be an object")
            node_id = endpoint.get("nodeId")
            require(node_id in node_ids, f"route.{endpoint_name}.nodeId is unknown: {node_id}")
            port_ids = endpoint.get("portIds")
            require(isinstance(port_ids, list) and len(port_ids) == 2, f"route.{endpoint_name}.portIds must contain stereo ports")
            for port_id in port_ids:
                require(port_id in port_ids_by_node[node_id], f"route.{endpoint_name}.portId is unknown on {node_id}: {port_id}")


def validate_with_jsonschema(profile: dict[str, Any], schema: dict[str, Any]) -> bool:
    try:
        import jsonschema  # type: ignore[import-not-found]
    except ImportError:
        return False

    jsonschema.validate(instance=profile, schema=schema)
    return True


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: validate_profile.py <profile.json>", file=sys.stderr)
        return 2

    profile_path = Path(argv[1])
    profile = load_json(profile_path)
    schema = load_json(SCHEMA_PATH)

    require(isinstance(profile, dict), "profile root must be an object")
    require(isinstance(schema, dict), "schema root must be an object")

    used_jsonschema = validate_with_jsonschema(profile, schema)
    validate_basic_profile(profile)

    mode = "jsonschema+basic" if used_jsonschema else "basic"
    print(f"profile validation passed: {profile_path} ({mode})")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main(sys.argv))
    except (OSError, json.JSONDecodeError, ValueError) as exc:
        print(f"profile validation failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
