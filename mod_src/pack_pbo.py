"""
Minimal PBO packer for DayZ server mods.
Packs a folder into a .pbo file with prefix support.

Usage: python pack_pbo.py <source_folder> <output.pbo>
"""

import struct
import hashlib
import os
import sys


def pack_pbo(source_dir, output_path):
    """Pack a directory into a DayZ PBO file."""

    # Read prefix from $PBOPREFIX$ file if it exists
    prefix = ""
    prefix_file = os.path.join(source_dir, "$PBOPREFIX$")
    if os.path.exists(prefix_file):
        with open(prefix_file, "r") as f:
            prefix = f.read().strip()

    # Collect all files (excluding $PBOPREFIX$)
    files = []
    for root, dirs, filenames in os.walk(source_dir):
        for fname in filenames:
            if fname == "$PBOPREFIX$":
                continue
            # Skip config.cpp source — only config.bin goes into PBO
            if fname == "config.cpp":
                continue
            full_path = os.path.join(root, fname)
            # Archive name: relative path with backslashes (DayZ convention)
            rel_path = os.path.relpath(full_path, source_dir)
            arc_name = rel_path.replace("/", "\\")
            with open(full_path, "rb") as f:
                data = f.read()
            files.append((arc_name, data))

    # Sort files for deterministic output
    files.sort(key=lambda x: x[0].lower())

    with open(output_path, "wb+") as pbo:
        # === HEADER SECTION ===

        # 1. Product entry (special header with "Vers" magic)
        pbo.write(b"\x00")  # empty filename
        pbo.write(struct.pack("<I", 0x56657273))  # "Vers" magic = properties entry
        pbo.write(struct.pack("<I", 0))  # original_size
        pbo.write(struct.pack("<I", 0))  # reserved
        pbo.write(struct.pack("<I", 0))  # timestamp
        pbo.write(struct.pack("<I", 0))  # data_size

        # 2. Properties (key\0value\0 pairs, terminated by empty key \0)
        if prefix:
            pbo.write(b"prefix\x00")
            pbo.write(prefix.encode("ascii") + b"\x00")
        pbo.write(b"\x00")  # empty key = end of properties

        # 3. File entries
        for arc_name, data in files:
            pbo.write(arc_name.encode("ascii") + b"\x00")
            pbo.write(struct.pack("<I", 0))  # packing = uncompressed
            pbo.write(struct.pack("<I", len(data)))  # original_size
            pbo.write(struct.pack("<I", 0))  # reserved
            pbo.write(struct.pack("<I", 0))  # timestamp
            pbo.write(struct.pack("<I", len(data)))  # data_size

        # 4. Terminator entry (empty filename + all zeros)
        pbo.write(b"\x00")  # empty filename
        pbo.write(struct.pack("<I", 0))
        pbo.write(struct.pack("<I", 0))
        pbo.write(struct.pack("<I", 0))
        pbo.write(struct.pack("<I", 0))
        pbo.write(struct.pack("<I", 0))

        # === DATA SECTION ===

        # 5. File data (concatenated in same order as entries)
        for arc_name, data in files:
            pbo.write(data)

        # === CHECKSUM ===

        # 6. SHA-1 checksum: hash everything written so far, then append
        pbo.seek(0)
        sha1 = hashlib.sha1(pbo.read()).digest()
        pbo.write(b"\x00")   # null separator
        pbo.write(sha1)      # 20-byte SHA-1 digest

    print(f"Packed {len(files)} files into {output_path}")
    print(f"  Prefix: {prefix}")
    for arc_name, data in files:
        print(f"  {arc_name} ({len(data)} bytes)")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <source_folder> <output.pbo>")
        sys.exit(1)

    pack_pbo(sys.argv[1], sys.argv[2])
