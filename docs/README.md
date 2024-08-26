# DuckDB fnv partitioner extension

Implements duckdb user defined functions for FNV hash and partitioning. It exposes two functions:

- fnv() - Takes a string and returns the FNV hash of the string.
- fnv_partition() - Takes a string and max partitions and returns the partition number.

## Build steps

To build the extension, follow these steps:

1. Run the command `make` in the terminal.

The main binaries that will be built are:

- `./build/release/duckdb`: The binary for the duckdb shell with the extension code automatically loaded.
- `./build/release/test/unittest`: The test runner of duckdb. The extension is already linked into the binary.
- `./build/release/extension/fnv/fnv.duckdb_extension`: The loadable binary as it would be distributed.

## Running the extension

To run the extension code, follow these steps:

1. Start the shell with `./build/release/duckdb`. This shell will have the extension pre-loaded.

Now you can use the features from the extension directly in DuckDB. The template contains a scalar function `fnv()` that takes a string argument and max partitions, and returns an integer.

Example usage:

```sql
SELECT fnv('joeirimpan') AS hash;
```

This will return:

```
┌───────────────┐
│     hash      │
│     int       │
├───────────────┤
│  1875971319   │
└───────────────┘
```

```sql
SELECT fnv_partition('joeirimpan', 32) AS partition;
```

This will return:

```
┌───────────────┐
│   partition   │
│     int       │
├───────────────┤
│      23       │
└───────────────┘
```
