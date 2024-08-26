<a href="https://zerodha.tech"><img src="https://zerodha.tech/static/images/github-badge.svg" align="right" /></a>

# DuckDB fnv partitioner extension

Implements duckdb user defined functions for FNV hash and partitioning. It exposes two functions:

- fnv() - Takes a string and returns the FNV hash of the string.
- fnv_partition() - Takes a string and max partitions and returns the partition number.


## Usage

```
$ wget https://github.com/joeirimpan/duckdb-fnv/releases/download/v0.0.1/fnv.duckdb_extension -O /path/to/file/fnv.duckdb_extension
$ duckdb -unsigned
v1.0.0 1f98600c2c
Enter ".help" for usage hints.
D LOAD '/path/to/file/fnv.duckdb_extension';
D select fnv('joeirimpan');
┌───────────────────┐
│ fnv('joeirimpan') │
│       int32       │
├───────────────────┤
│        1875971319 │
└───────────────────┘
D select fnv_partition('joeirimpan', 420);
┌──────────────────────────────────┐
│ fnv_partition('joeirimpan', 420) │
│              int32               │
├──────────────────────────────────┤
│                              159 │
└──────────────────────────────────
```

## Development

To build the extension, follow these steps:

1. Run the command `make` in the terminal.

The main binaries that will be built are:

- `./build/release/duckdb`: The binary for the duckdb shell with the extension code automatically loaded.
- `./build/release/test/unittest`: The test runner of duckdb. The extension is already linked into the binary.
- `./build/release/extension/fnv/fnv.duckdb_extension`: The loadable binary as it would be distributed.

### Building for `linux_amd64_gcc4` platform

To build the extension for `linux_amd64_gcc4`, follow these steps:

1. `docker build -t duckdb_fnv .`
2. `docker run --rm duckdb_fnv cat /build/release/extension/fnv/fnv.duckdb_extension > fnv.duckdb_extension`

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
