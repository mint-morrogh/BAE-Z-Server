"""
Minimal config.cpp to config.bin (rapified) converter for DayZ mods.
Converts a parsed config tree into the binary raP format.

Usage: python rapify.py <input.cpp> <output.bin>
"""

import struct
import sys
import re
import os


def write_compressed_int(buf, value):
    """Write a compressed integer to buffer."""
    if value < 0x80:
        buf += struct.pack("B", value)
    elif value < 0x4000:
        buf += struct.pack("B", (value & 0x7F) | 0x80)
        buf += struct.pack("B", (value >> 7) & 0x7F)
    elif value < 0x200000:
        buf += struct.pack("B", (value & 0x7F) | 0x80)
        buf += struct.pack("B", ((value >> 7) & 0x7F) | 0x80)
        buf += struct.pack("B", (value >> 14) & 0x7F)
    else:
        buf += struct.pack("B", (value & 0x7F) | 0x80)
        buf += struct.pack("B", ((value >> 7) & 0x7F) | 0x80)
        buf += struct.pack("B", ((value >> 14) & 0x7F) | 0x80)
        buf += struct.pack("B", (value >> 21) & 0x7F)
    return buf


def write_asciiz(buf, s):
    """Write null-terminated ASCII string."""
    buf += s.encode("ascii") + b"\x00"
    return buf


# ---- Simple config.cpp parser ----

class ConfigClass:
    def __init__(self, name, inherits=""):
        self.name = name
        self.inherits = inherits
        self.entries = []  # (type, ...)

    def add_class(self, cls):
        self.entries.append(("class", cls))

    def add_string(self, name, value):
        self.entries.append(("string", name, value))

    def add_int(self, name, value):
        self.entries.append(("int", name, int(value)))

    def add_float(self, name, value):
        self.entries.append(("float", name, float(value)))

    def add_array(self, name, elements):
        self.entries.append(("array", name, elements))


def tokenize(text):
    """Simple tokenizer for config.cpp."""
    tokens = []
    i = 0
    while i < len(text):
        # Skip whitespace
        if text[i] in " \t\r\n":
            i += 1
            continue
        # Skip // comments
        if text[i:i+2] == "//":
            while i < len(text) and text[i] != "\n":
                i += 1
            continue
        # Skip /* */ comments
        if text[i:i+2] == "/*":
            end = text.find("*/", i + 2)
            i = end + 2 if end != -1 else len(text)
            continue
        # String literal
        if text[i] == '"':
            j = i + 1
            while j < len(text) and text[j] != '"':
                if text[j] == "\\":
                    j += 1
                j += 1
            tokens.append(text[i:j+1])
            i = j + 1
            continue
        # Symbols
        if text[i] in "{}[];=,":
            tokens.append(text[i])
            i += 1
            continue
        # Word/number
        j = i
        while j < len(text) and text[j] not in " \t\r\n{}[];=,\"":
            j += 1
        if j > i:
            tokens.append(text[i:j])
        i = j
    return tokens


def parse_class_body(tokens, pos, cls):
    """Parse the body of a class (between { and })."""
    while pos < len(tokens) and tokens[pos] != "}":
        token = tokens[pos]

        if token == "class":
            # class ClassName [: Parent] { ... };
            pos += 1
            class_name = tokens[pos]
            pos += 1
            inherits = ""
            if tokens[pos] == ":":
                pos += 1
                inherits = tokens[pos]
                pos += 1
            if tokens[pos] == "{":
                pos += 1
                sub = ConfigClass(class_name, inherits)
                pos = parse_class_body(tokens, pos, sub)
                cls.add_class(sub)
                if pos < len(tokens) and tokens[pos] == "}":
                    pos += 1
                if pos < len(tokens) and tokens[pos] == ";":
                    pos += 1
            continue

        # Check for array: name[] = { ... };
        if pos + 1 < len(tokens) and tokens[pos + 1] == "[":
            prop_name = token
            pos += 2  # skip name, [
            if tokens[pos] == "]":
                pos += 1
            if tokens[pos] == "=":
                pos += 1
            if tokens[pos] == "{":
                pos += 1
                elements = []
                while tokens[pos] != "}":
                    val = tokens[pos]
                    if val == ",":
                        pos += 1
                        continue
                    if val.startswith('"'):
                        elements.append(("string", val[1:-1]))
                    else:
                        try:
                            elements.append(("int", int(val)))
                        except ValueError:
                            try:
                                elements.append(("float", float(val)))
                            except ValueError:
                                elements.append(("string", val))
                    pos += 1
                pos += 1  # skip }
                if pos < len(tokens) and tokens[pos] == ";":
                    pos += 1
                cls.add_array(prop_name, elements)
            continue

        # Property: name = value;
        if pos + 1 < len(tokens) and tokens[pos + 1] == "=":
            prop_name = token
            pos += 2  # skip name, =
            val = tokens[pos]
            pos += 1
            if pos < len(tokens) and tokens[pos] == ";":
                pos += 1

            if val.startswith('"'):
                cls.add_string(prop_name, val[1:-1])
            else:
                try:
                    if "." in val:
                        cls.add_float(prop_name, val)
                    else:
                        cls.add_int(prop_name, val)
                except ValueError:
                    cls.add_string(prop_name, val)
            continue

        # Skip unknown token
        pos += 1

    return pos


def parse_config(text):
    """Parse a config.cpp file into a ConfigClass tree."""
    tokens = tokenize(text)
    root = ConfigClass("", "")
    parse_class_body(tokens, 0, root)
    return root


# ---- Rapify (binary serialization) ----

def rapify_class_body(cls):
    """
    Serialize a class body. Returns (header_bytes, deferred_classes).
    Deferred classes need their offsets filled in later.
    """
    buf = bytearray()

    # Inherited class name
    buf = write_asciiz(buf, cls.inherits)

    # Number of entries
    buf = write_compressed_int(buf, len(cls.entries))

    # Track where class offset placeholders are, and which class they refer to
    deferred = []  # (offset_in_buf, ConfigClass)

    for entry in cls.entries:
        if entry[0] == "class":
            sub = entry[1]
            buf += struct.pack("B", 0)  # type = class
            buf = write_asciiz(buf, sub.name)
            deferred.append((len(buf), sub))
            buf += struct.pack("<I", 0)  # placeholder for offset

        elif entry[0] == "string":
            _, name, value = entry
            buf += struct.pack("B", 1)  # type = value
            buf += struct.pack("B", 0)  # subtype = string
            buf = write_asciiz(buf, name)
            buf = write_asciiz(buf, value)

        elif entry[0] == "float":
            _, name, value = entry
            buf += struct.pack("B", 1)  # type = value
            buf += struct.pack("B", 1)  # subtype = float
            buf = write_asciiz(buf, name)
            buf += struct.pack("<f", value)

        elif entry[0] == "int":
            _, name, value = entry
            buf += struct.pack("B", 1)  # type = value
            buf += struct.pack("B", 2)  # subtype = int
            buf = write_asciiz(buf, name)
            buf += struct.pack("<i", value)

        elif entry[0] == "array":
            _, name, elements = entry
            buf += struct.pack("B", 2)  # type = array
            buf += struct.pack("B", 1)  # subtype (compressed array flag)
            buf = write_asciiz(buf, name)
            buf = write_compressed_int(buf, len(elements))
            for elem in elements:
                if elem[0] == "string":
                    buf += struct.pack("B", 0)
                    buf = write_asciiz(buf, elem[1])
                elif elem[0] == "float":
                    buf += struct.pack("B", 1)
                    buf += struct.pack("<f", elem[1])
                elif elem[0] == "int":
                    buf += struct.pack("B", 2)
                    buf += struct.pack("<i", elem[1])

    return buf, deferred


def rapify(root):
    """Convert a config tree to rapified binary format."""
    # Build the entire file
    # Header: magic(4) + always_0(4) + always_8(4) + enums_offset(4) = 16 bytes
    header = b"\x00raP"
    header += struct.pack("<I", 0)   # always 0
    header += struct.pack("<I", 8)   # always 8 (version/type indicator)
    header += struct.pack("<I", 0)   # enums offset (0 = no enums, patched later)

    # We need to serialize all class bodies and resolve offsets
    # Strategy: serialize root body, then all sub-classes depth-first
    # Keep track of absolute file offsets

    # First, serialize root class body
    root_body, root_deferred = rapify_class_body(root)

    # Now we need to serialize all deferred sub-classes
    # and fill in their offsets
    all_parts = [header, root_body]
    pending = [(root_body, root_deferred, 1)]  # (parent_buf, deferred_list, part_index)

    while pending:
        parent_buf, deferred_list, parent_idx = pending.pop(0)

        for offset_in_parent, sub_cls in deferred_list:
            # Calculate absolute offset where this class body will be
            abs_offset = sum(len(p) for p in all_parts)

            # Fill in the offset in the parent buffer
            # The offset is relative to start of file
            struct.pack_into("<I", all_parts[parent_idx], offset_in_parent, abs_offset)

            # Serialize this class body
            sub_body, sub_deferred = rapify_class_body(sub_cls)
            sub_idx = len(all_parts)
            all_parts.append(sub_body)

            if sub_deferred:
                pending.append((sub_body, sub_deferred, sub_idx))

    return b"".join(bytes(p) for p in all_parts)


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.cpp> <output.bin>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, "r") as f:
        text = f.read()

    root = parse_config(text)
    binary = rapify(root)

    with open(output_path, "wb") as f:
        f.write(binary)

    print(f"Rapified {input_path} -> {output_path} ({len(binary)} bytes)")


if __name__ == "__main__":
    main()
